/*
 * room_mgt.cpp
 *
 *  Created on: 2011-12-5
 *      Author: jimm
 */

#include "frame_errordef.h"
#include "room_mgt.h"
#include "server_datacenter.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

CRoomMgt::CRoomMgt()
{
}
CRoomMgt::~CRoomMgt()
{

}

//初始化玩家管理器
int32_t CRoomMgt::Initialize()
{
	m_sRoomPool.Initialize();
	m_sRoomList.Initialize();

	return S_OK;
}

//恢复玩家管理器
int32_t CRoomMgt::Resume()
{
	m_sRoomPool.Resume();
	m_sRoomList.Resume();

	return S_OK;
}

//注销玩家管理器
int32_t CRoomMgt::Uninitialize()
{
	m_sRoomPool.Uninitialize();
	m_sRoomList.Uninitialize();

	return S_OK;
}

int32_t CRoomMgt::CreateRoom(const RoomID nRoomID, RoomPool::CIndex *&pIndex)
{
	pIndex = m_sRoomPool.CreateObject();
	if(NULL == pIndex)
	{
		return E_LIBF_CREATEROOM;
	}

	RoomList::Key key = MakeRoomKey(nRoomID);
	RoomList::CIndex *pListIndex = m_sRoomList.Insert(key, pIndex->Index());
	if (NULL == pListIndex)
	{
		m_sRoomPool.DestroyObject(pIndex);
		pIndex = NULL;
		return E_LIBF_CREATEROOMINDEX;
	}

	//将索引保存到附加数据表中
	int32_t ret = pIndex->SetAdditionalData(enmAdditionalIndex_HashListIndex, (uint32_t)pListIndex->Index());
	if (0 > ret)
	{
		m_sRoomList.Erase(pListIndex);
		m_sRoomPool.DestroyObject(pIndex);
		pIndex = NULL;
		return ret;
	}

	return S_OK;
}

int32_t CRoomMgt::CreateRoom(const RoomID nRoomID, CRoom *&pRoom)
{
	RoomPool::CIndex* pIndex = NULL;

	int32_t ret = CreateRoom(nRoomID, pIndex);
	if (0 > ret)
	{
		return ret;
	}

	pRoom = pIndex->ObjectPtr();
	pRoom->SetRoomID(nRoomID);

	return S_OK;
}

int32_t CRoomMgt::GetRoom(const RoomID nRoomID, RoomPool::CIndex*& pIndex)
{
	//根据索引查找
	RoomList::Key key = MakeRoomKey(nRoomID);
	RoomList::CIndex* pListIndex = m_sRoomList.Find(key);
	if (NULL == pListIndex)
	{
		return E_LIBF_ROOMNOTFOUND;
	}

	pIndex = m_sRoomPool.GetIndex(pListIndex->GetObject());
	if (NULL == pIndex)
	{
		return E_LIBF_ROOMNOTFOUND;
	}

	return S_OK;
}

int32_t CRoomMgt::GetRoom(const RoomID nRoomID, CRoom *&pRoom)
{
	RoomPool::CIndex* pIndex = NULL;

	int32_t ret = GetRoom(nRoomID, pIndex);
	if (0 > ret)
	{
		return ret;
	}

	pRoom = pIndex->ObjectPtr();

	return S_OK;
}

int32_t CRoomMgt::DestoryRoom(const RoomID nRoomID)
{
	//根据索引查找
	RoomList::Key key = MakeRoomKey(nRoomID);
	RoomList::CIndex* pListIndex = m_sRoomList.Find(key);
	if (NULL == pListIndex)
	{
		return E_LIBF_ROOMNOTEXIST;
	}

	DeleteAllPlayersInRoom(nRoomID);

	RoomIndex playerIndex = pListIndex->GetObject();

	//将玩家从对象池及索引表中删除
	m_sRoomList.Erase(pListIndex);
	m_sRoomPool.DestroyObject(playerIndex);

	return S_OK;
}

int32_t CRoomMgt::Clear()
{
	m_sRoomPool.Clear();
	m_sRoomList.Clear();
	return S_OK;
}

//获取对象池容量
uint32_t CRoomMgt::GetCapacity() const
{
	return m_sRoomPool.Capacity();
}

//获取对象池中对象数量
uint32_t CRoomMgt::GetRoomCount() const
{
	return m_sRoomPool.ObjectCount();
}

//对象池是否为空
bool CRoomMgt::IsEmpty() const
{
	return m_sRoomPool.IsEmpty();
}

//对象池是否已满
bool CRoomMgt::IsFull() const
{
	return m_sRoomPool.IsFull();
}

int32_t CRoomMgt::GetAllRoom(CRoom *arrRoomObject[], const int32_t arrSize, int32_t &nRoomObjectCount)
{
	nRoomObjectCount = 0;
	RoomPool::CIndex *pIndex = m_sRoomPool.First();

	while(NULL != pIndex && nRoomObjectCount < arrSize)
	{
		arrRoomObject[nRoomObjectCount++] = pIndex->ObjectPtr();
		pIndex = pIndex->Next();
	}

	return S_OK;
}

int32_t CRoomMgt::DeleteAllPlayersInRoom(RoomID nRoomID)
{
	CRoom *pRoom = NULL;
	int32_t ret = GetRoom(nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		return ret;
	}

	RoleID arrRoleID[MaxUserCountPerRoom];
	int32_t nPlayerCount = 0;
	pRoom->GetAllPlayers(arrRoleID, MaxUserCountPerRoom, nPlayerCount);

	for(int32_t i = 0; i < nPlayerCount; ++i)
	{
		CPlayer *pPlayer = NULL;
		ret = g_PlayerMgt.GetPlayer(arrRoleID[i], pPlayer);
		if(ret < 0 || pPlayer == NULL)
		{
			continue;
		}

		pPlayer->ExitRoom(nRoomID);
	}

	pRoom->DeleteAllPlayers();

	return S_OK;
}

int32_t CRoomMgt::AddPlayer(RoomID nRoomID, RoleID nRoleID)
{
	CRoom *pRoom = NULL;
	int32_t ret = GetRoom(nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		ret = g_RoomMgt.CreateRoom(nRoomID, pRoom);
		if(ret < 0 || pRoom == NULL)
		{
			return ret;
		}
	}

	CPlayer *pPlayer = NULL;
	ret = g_PlayerMgt.GetPlayer(nRoleID, pPlayer);
	if(ret < 0 || pPlayer == NULL)
	{
		ret = g_PlayerMgt.CreatePlayer(nRoleID, pPlayer);
		if(ret < 0 || pRoom == NULL)
		{
			return ret;
		}
	}

	pRoom->AddPlayer(nRoleID);

	pPlayer->EnterRoom(nRoomID, pRoom->GetRoomServerID());

	return S_OK;
}

int32_t CRoomMgt::DeletePlayer(RoomID nRoomID, RoleID nRoleID)
{
	CRoom *pRoom = NULL;
	int32_t ret = GetRoom(nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		return ret;
	}

	CPlayer *pPlayer = NULL;
	ret = g_PlayerMgt.GetPlayer(nRoleID, pPlayer);
	if(ret < 0 || pPlayer == NULL)
	{
		return ret;
	}

	pRoom->DeletePlayer(nRoleID);

	pPlayer->ExitRoom(nRoomID);

	return S_OK;
}

RoomList::Key CRoomMgt::MakeRoomKey(const RoomID nRoomID) const
{
	RoomList::Key key = { 0 };
	key.nKeyLength = (uint32_t)sizeof(RoomID);
	*(RoomID*)key.arrKey = nRoomID;

	return key;
}

FRAME_HALLSERVER_NAMESPACE_END

