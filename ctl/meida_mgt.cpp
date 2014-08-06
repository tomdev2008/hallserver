/*
 * meida_mgt.cpp
 *
 *  Created on: 2012-1-11
 *      Author: jimm
 */

#include "frame_errordef.h"
#include "media_mgt.h"
#include "room_mgt.h"
#include "server_datacenter.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

CMediaMgt::CMediaMgt()
{
}
CMediaMgt::~CMediaMgt()
{

}

//初始化玩家管理器
int32_t CMediaMgt::Initialize()
{
	m_sMediaPool.Initialize();
	m_sMediaList.Initialize();

	return S_OK;
}

//恢复玩家管理器
int32_t CMediaMgt::Resume()
{
	m_sMediaPool.Resume();
	m_sMediaList.Resume();

	return S_OK;
}

//注销玩家管理器
int32_t CMediaMgt::Uninitialize()
{
	m_sMediaPool.Uninitialize();
	m_sMediaList.Uninitialize();

	return S_OK;
}

int32_t CMediaMgt::CreateMedia(const ServerID nServerID, MediaPool::CIndex *&pIndex)
{
	pIndex = m_sMediaPool.CreateObject();
	if(NULL == pIndex)
	{
		return E_LIBF_CREATEROOM;
	}

	MediaList::Key key = MakeKey(nServerID);
	MediaList::CIndex *pListIndex = m_sMediaList.Insert(key, pIndex->Index());
	if (NULL == pListIndex)
	{
		m_sMediaPool.DestroyObject(pIndex);
		pIndex = NULL;
		return E_LIBF_CREATEROOMINDEX;
	}

	//将索引保存到附加数据表中
	int32_t ret = pIndex->SetAdditionalData(enmAdditionalIndex_HashListIndex, (uint32_t)pListIndex->Index());
	if (0 > ret)
	{
		m_sMediaList.Erase(pListIndex);
		m_sMediaPool.DestroyObject(pIndex);
		pIndex = NULL;
		return ret;
	}

	return S_OK;
}

int32_t CMediaMgt::CreateMedia(const ServerID nServerID, CMedia *&pMedia)
{
	MediaPool::CIndex* pIndex = NULL;

	int32_t ret = CreateMedia(nServerID, pIndex);
	if (0 > ret)
	{
		return ret;
	}

	pMedia = pIndex->ObjectPtr();
	pMedia->SetServerID(nServerID);

	return S_OK;
}

int32_t CMediaMgt::GetMedia(const ServerID nServerID, MediaPool::CIndex*& pIndex)
{
	//根据索引查找
	MediaList::Key key = MakeKey(nServerID);
	MediaList::CIndex* pListIndex = m_sMediaList.Find(key);
	if (NULL == pListIndex)
	{
		return E_LIBF_ROOMNOTFOUND;
	}

	pIndex = m_sMediaPool.GetIndex(pListIndex->GetObject());
	if (NULL == pIndex)
	{
		return E_LIBF_ROOMNOTFOUND;
	}

	return S_OK;
}

int32_t CMediaMgt::GetMedia(const ServerID nServerID, CMedia *&pMedia)
{
	MediaPool::CIndex* pIndex = NULL;

	int32_t ret = GetMedia(nServerID, pIndex);
	if (0 > ret)
	{
		return ret;
	}

	pMedia = pIndex->ObjectPtr();

	return S_OK;
}

int32_t CMediaMgt::DestoryMedia(const ServerID nServerID)
{
	//根据索引查找
	MediaList::Key key = MakeKey(nServerID);
	MediaList::CIndex* pListIndex = m_sMediaList.Find(key);
	if (NULL == pListIndex)
	{
		return E_LIBF_ROOMNOTEXIST;
	}

	MediaIndex playerIndex = pListIndex->GetObject();

	//将玩家从对象池及索引表中删除
	m_sMediaList.Erase(pListIndex);
	m_sMediaPool.DestroyObject(playerIndex);

	return S_OK;
}

int32_t CMediaMgt::Clear()
{
	m_sMediaPool.Clear();
	m_sMediaList.Clear();
	return S_OK;
}

//获取对象池容量
uint32_t CMediaMgt::GetCapacity() const
{
	return m_sMediaPool.Capacity();
}

//获取对象池中对象数量
uint32_t CMediaMgt::GetMediaCount() const
{
	return m_sMediaPool.ObjectCount();
}

//对象池是否为空
bool CMediaMgt::IsEmpty() const
{
	return m_sMediaPool.IsEmpty();
}

//对象池是否已满
bool CMediaMgt::IsFull() const
{
	return m_sMediaPool.IsFull();
}

int32_t CMediaMgt::GetAllMedia(CMedia *arrMediaObject[], const int32_t arrSize, int32_t &nMediaObjectCount)
{
	nMediaObjectCount = 0;
	MediaPool::CIndex *pIndex = m_sMediaPool.First();

	while(NULL != pIndex && nMediaObjectCount < arrSize)
	{
		arrMediaObject[nMediaObjectCount++] = pIndex->ObjectPtr();
		pIndex = pIndex->Next();
	}

	return S_OK;
}


MediaList::Key CMediaMgt::MakeKey(const ServerID nServerID) const
{
	MediaList::Key key = { 0 };
	key.nKeyLength = (uint32_t)sizeof(ServerID);
	*(ServerID*)key.arrKey = nServerID;

	return key;
}

int32_t CMediaMgt::GetMediaServerPlayerCount(ServerID nServerID)
{
	int32_t nPlayerCount = 0;

	CMedia *pMedia = NULL;
	int32_t ret = GetMedia(nServerID, pMedia);
	if(ret < 0)
	{
		return 0;
	}

	RoomID arrRoomID[MaxRoomCountPerMedia];
	int32_t nRoomCount;
	pMedia->GetAllRooms(arrRoomID, MaxRoomCountPerMedia, nRoomCount);

	for(int32_t i = 0; i < nRoomCount; ++i)
	{
		CRoom *pRoom = NULL;
		ret = g_RoomMgt.GetRoom(arrRoomID[i], pRoom);
		if(ret < 0 || pRoom == NULL)
		{
			continue;
		}
		nPlayerCount += pRoom->GetCurUserCount();
	}

	return nPlayerCount;
}

FRAME_HALLSERVER_NAMESPACE_END

