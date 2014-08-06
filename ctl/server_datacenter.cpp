/*
 * hallserver_datacenter.cpp
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#include "common/common_datetime.h"
#include "frame_errordef.h"
#include "lightframe_impl.h"
#include "server_datacenter.h"
#include "def/dal_def.h"
#include "auth/md5.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN


CServerDataCenter::CServerDataCenter()
{
	m_pPlayerMgt = NULL;
	m_pRegionMgt = NULL;
	m_pRoomMgt = NULL;
	m_pMediaMgt = NULL;
	m_nTunnelServerNum = 0;
	for(uint32_t i = 0; i < MaxTunnelNum; ++i)
	{
		m_arrTunnelServerInfo[i].Initialize();
	}
	m_nServerStatus = enmServerStatus_Initializing;

}

CServerDataCenter::~CServerDataCenter()
{

}

size_t CServerDataCenter::GetSize()
{
	return sizeof(CPlayerMgt) + sizeof(CRegionMgt) + sizeof(CRoomMgt) + sizeof(CMediaMgt);

}
const char* CServerDataCenter::GetName()
{
	return SERVER_NAME_STRING"DataCenter";
}
//初始化数据中心
int32_t CServerDataCenter::Initialize()
{
	m_pPlayerMgt = new CPlayerMgt();
	if(NULL == m_pPlayerMgt)
	{
		return E_HS_CREATEPLAYERMGTERROR;
	}

	int32_t nRet = m_pPlayerMgt->Initialize();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pRegionMgt = new CRegionMgt();
	if(NULL == m_pRegionMgt)
	{
		return E_HS_CREATEREGIONMGTERROR;
	}

	nRet = m_pRegionMgt->Initialize();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pRoomMgt = new CRoomMgt();
	if(NULL == m_pRoomMgt)
	{
		return E_HS_CREATEROOMMGTERROR;
	}

	nRet = m_pRoomMgt->Initialize();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pMediaMgt = new CMediaMgt();
	if(NULL == m_pMediaMgt)
	{
		return E_HS_CREATEROOMMGTERROR;
	}

	nRet = m_pMediaMgt->Initialize();
	if(0 > nRet)
	{
		return nRet;
	}

	return S_OK;
}

//恢复数据中心
int32_t CServerDataCenter::Resume()
{
	m_pPlayerMgt = new CPlayerMgt();
	if(NULL == m_pPlayerMgt)
	{
		return E_HS_CREATEPLAYERMGTERROR;
	}

	int32_t nRet = m_pPlayerMgt->Resume();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pRegionMgt = new CRegionMgt();
	if(NULL == m_pRegionMgt)
	{
		return E_HS_CREATEREGIONMGTERROR;
	}

	nRet = m_pRegionMgt->Resume();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pRoomMgt = new CRoomMgt();
	if(NULL == m_pRoomMgt)
	{
		return E_HS_CREATEROOMMGTERROR;
	}

	nRet = m_pRoomMgt->Resume();
	if(0 > nRet)
	{
		return nRet;
	}

	m_pMediaMgt = new CMediaMgt();
	if(NULL == m_pMediaMgt)
	{
		return E_HS_CREATEROOMMGTERROR;
	}

	nRet = m_pMediaMgt->Resume();
	if(0 > nRet)
	{
		return nRet;
	}

	return S_OK;
}

//注销数据中心
int32_t CServerDataCenter::Uninitialize()
{
	if(m_pPlayerMgt != NULL)
	{
		delete m_pPlayerMgt;
		m_pPlayerMgt = NULL;
	}

	if(m_pRegionMgt != NULL)
	{
		delete m_pRegionMgt;
		m_pRegionMgt = NULL;
	}

	if(m_pRoomMgt != NULL)
	{
		delete m_pRoomMgt;
		m_pRoomMgt = NULL;
	}

	if(m_pMediaMgt != NULL)
	{
		delete m_pMediaMgt;
		m_pMediaMgt = NULL;
	}

	return S_OK;
}


int32_t CServerDataCenter::GetAllPlayers(const RoomID nRoomID, CPlayer *arrPlayer[],
		const int32_t arrSize, int32_t &nPlayerCount)
{
	int32_t ret = 0;
	CRoom *pRoom = NULL;
	ret = g_RoomMgt.GetRoom(nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		return ret;
	}

	return GetAllPlayers(pRoom, arrPlayer, arrSize, nPlayerCount);
}

int32_t CServerDataCenter::GetAllPlayers(CRoom *pRoom, CPlayer *arrPlayer[],
		const int32_t arrSize, int32_t &nPlayerCount)
{
	nPlayerCount = 0;
	if(pRoom == NULL)
	{
		return S_OK;
	}

	RoleID arrRoleID[MaxUserCountPerRoom] = { enmInvalidRoleID };
	pRoom->GetAllPlayers(arrRoleID, MaxUserCountPerRoom, nPlayerCount);

	int32_t nCurPlayerCount = nPlayerCount;
	for(int32_t i = 0; i < nCurPlayerCount; ++i)
	{
		CPlayer *pPlayer = NULL;
		g_PlayerMgt.GetPlayer(arrRoleID[i], pPlayer);
		if(pPlayer == NULL)
		{
			if(nPlayerCount > 0)
			{
				--nPlayerCount;
			}
			continue;
		}

		arrPlayer[i] = pPlayer;
	}

	return S_OK;
}

int32_t CServerDataCenter::PopPlayers(EntityType nEntityType, ServerID nServerID, CPlayer *arrPlayer[],
		int32_t arrSize, int32_t &nPlayerCount)
{
	int32_t ret = S_OK;

	nPlayerCount = 0;

	int32_t nIndex = -1;
	for(int32_t i = 0; i < MaxServerInfoCount; ++i)
	{
		if(m_arrServerInfo[i].nEntityType != nEntityType)
		{
			continue;
		}

		if(m_arrServerInfo[i].nServerID != nServerID)
		{
			continue;
		}

		nIndex = i;
		break;
	}

	if(nIndex < 0)
	{
		return ret;
	}

	int32_t nCurPlayerCount = m_arrServerInfo[nIndex].nCurPlayerCount;
	int32_t nCurPlayerCountTemp = nCurPlayerCount;
	for(int32_t i = 0; i < nCurPlayerCountTemp; ++i)
	{
		RoleID nRoleID = m_arrServerInfo[nIndex].arrRoleID[nCurPlayerCount - 1];

		m_arrServerInfo[nIndex].arrRoleID[nCurPlayerCount - 1] = enmInvalidRoleID;
		--nCurPlayerCount;

		CPlayer *pPlayer = NULL;
		ret = m_pPlayerMgt->GetPlayer(nRoleID, pPlayer);
		if(ret < 0 || pPlayer == NULL)
		{
			continue;
		}
		arrPlayer[nPlayerCount] = pPlayer;

		++nPlayerCount;
		if(nPlayerCount >= MaxGetOnlinePlayerCount)
		{
			break;
		}
	}

	m_arrServerInfo[nIndex].nCurPlayerCount = nCurPlayerCount;

	if(m_arrServerInfo[nIndex].nCurPlayerCount <= 0)
	{
		m_arrServerInfo[nIndex].Reset();
	}

	return S_OK;
}

int32_t CServerDataCenter::PushPlayers(EntityType nEntityType, ServerID nServerID, RoleID arrRoleID[],
		int32_t arrSize, int32_t nPlayerCount)
{
	int32_t ret = S_OK;

	//没有被占用的位置
	int32_t nIndex = -1;
	for(int32_t i = 0; i < MaxServerInfoCount; ++i)
	{
		if(m_arrServerInfo[i].nEntityType == nEntityType && m_arrServerInfo[i].nServerID == nServerID)
		{
			m_arrServerInfo[i].Reset();
			nIndex = i;
			break;
		}
		if(m_arrServerInfo[i].nEntityType != enmInvalidEntityType)
		{
			continue;
		}

		if(m_arrServerInfo[i].nServerID != enmInvalidServerID)
		{
			continue;
		}

		nIndex = i;
		break;
	}

	if(nIndex < 0)
	{
		return ret;
	}

	m_arrServerInfo[nIndex].nEntityType = nEntityType;
	m_arrServerInfo[nIndex].nServerID = nServerID;
	m_arrServerInfo[nIndex].nCurPlayerCount = nPlayerCount;
	for(int32_t i = 0; i < nPlayerCount; ++i)
	{
		m_arrServerInfo[nIndex].arrRoleID[i] = arrRoleID[i];
	}

	return S_OK;
}

int32_t CServerDataCenter::AddTunnel(ServerID nTunnelServerID, uint32_t nRoomNum, uint32_t nPlayerNum)
{
	if(m_nTunnelServerNum >= MaxTunnelNum)
		return -1;

	for(uint32_t i = 0; i < m_nTunnelServerNum; ++i)
	{
		if(m_arrTunnelServerInfo[i].nServerID == nTunnelServerID)
		{
			m_arrTunnelServerInfo[i].nPlayerTotalNum = nPlayerNum;
			m_arrTunnelServerInfo[i].nRoomTotalNum = nRoomNum;
			m_arrTunnelServerInfo[i].nRoomBuildNum = 0;
			m_arrTunnelServerInfo[i].nPlayerBuildNum = 0;
			m_arrTunnelServerInfo[i].bInitialized = false;
			return S_OK;
		}

	}

	m_arrTunnelServerInfo[m_nTunnelServerNum].nServerID = nTunnelServerID;
	m_arrTunnelServerInfo[m_nTunnelServerNum].nPlayerTotalNum = nPlayerNum;
	m_arrTunnelServerInfo[m_nTunnelServerNum].nRoomTotalNum = nRoomNum;
	m_arrTunnelServerInfo[m_nTunnelServerNum].nRoomBuildNum = 0;
	m_arrTunnelServerInfo[m_nTunnelServerNum].nPlayerBuildNum = 0;
	m_arrTunnelServerInfo[m_nTunnelServerNum].bInitialized = false;

	m_nTunnelServerNum++;

	return S_OK;
}

int32_t CServerDataCenter::AddTunnelBuildNum(ServerID nTunnelServerID, uint32_t nRoomBuildNum, uint32_t nPlayerBuildNum)
{
	for(uint32_t i = 0; i < m_nTunnelServerNum; ++i)
	{
		if(m_arrTunnelServerInfo[i].nServerID == nTunnelServerID)
		{
			m_arrTunnelServerInfo[i].nRoomBuildNum += nRoomBuildNum;
			m_arrTunnelServerInfo[i].nPlayerBuildNum += nPlayerBuildNum;
			return S_OK;
		}
	}
	return -1;
}

int32_t CServerDataCenter::DelTunnel(ServerID nTunnelServerID)
{
	for(uint32_t i = 0; i < m_nTunnelServerNum; ++i)
	{
		if(m_arrTunnelServerInfo[i].nServerID == nTunnelServerID)
		{
			m_arrTunnelServerInfo[i] = m_arrTunnelServerInfo[--m_nTunnelServerNum];
			return S_OK;
		}
	}

	return S_OK;
}

int32_t CServerDataCenter::GetTunnelID(RoomID nRoomID, ServerID& nTunnelServerID)
{
	if(m_nTunnelServerNum == 0)
		return enmInvalidServerID;

	// md5 for the room id
	char szRoomStr[11] = {0};
	snprintf(szRoomStr, 10, "%u", nRoomID);
	char szmd5_out[32] = {0};
	md5_string((unsigned char*)szRoomStr, szmd5_out, strlen(szRoomStr));

	// get first 2 characters of md5 string for key
	char szRoomKey[3] = {0};
	strncpy(szRoomKey, szmd5_out, 2);
	uint32_t nRoomKey = 0;
	sscanf(szRoomKey, "%x", &nRoomKey);

	// get the real index by key % m_nTunnelServerNum
	nTunnelServerID = m_arrTunnelServerInfo[nRoomKey % m_nTunnelServerNum].nServerID;

	return S_OK;
}

int32_t CServerDataCenter::GetTunnelServerIndex(ServerID nTunnelServerID, uint32_t& nIndex)
{
	for(uint32_t i = 0; i < m_nTunnelServerNum; ++i)
	{
		if(m_arrTunnelServerInfo[i].nServerID == nTunnelServerID)
		{
			nIndex = i;
			return S_OK;
		}
	}

	return -1;
}

//int32_t CServerDataCenter::GetTunnelTotalNum(ServerID nTunnelServerID, uint32_t& nRoomNum, uint32_t& nPlayerNum)
//{
//	for(uint32_t i = 0; i < m_nTunnelServerNum; ++i)
//	{
//		if(m_arrTunnelServerInfo[i].nServerID == nTunnelServerID)
//		{
//			nRoomNum = m_arrTunnelServerInfo[i].nRoomTotalNum;
//			nPlayerNum = m_arrTunnelServerInfo[i].nPlayerTotalNum;
//			return S_OK;
//		}
//	}
//
//	return -1;
//}
//
//int32_t CServerDataCenter::GetTunnelBuildNum(int32_t nTunnelServerID, uint32_t& nRoomBuildNum, uint32_t& nPlayerBuildNum)
//{
//	for(uint32_t i = 0; i < m_nTunnelServerNum; ++i)
//	{
//		if(m_arrTunnelServerInfo[i].nServerID == nTunnelServerID)
//		{
//			nRoomBuildNum = m_arrTunnelServerInfo[i].nRoomBuildNum;
//			nPlayerBuildNum = m_arrTunnelServerInfo[i].nPlayerBuildNum;
//			return S_OK;
//		}
//	}
//	return -1;
//}

FRAME_HALLSERVER_NAMESPACE_END


