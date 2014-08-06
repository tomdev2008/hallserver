/*
 * bll_event_getonlineplayerinfo.cpp
 *
 *  Created on: 2012-5-18
 *      Author: jimm
 */

#include "bll_event_getonlineplayerinfo.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "dal/from_server_message.h"
#include "itemserver_message_define.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CGetOnlinePlayerInfoMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}
	// check the server status
	CGetOnlinePlayerInfoResp stResp;
	stResp.nResult = enmGetOnlinePlayerInfoResult_Unknown;
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		SendMessageResponse(MSGID_HSIS_GETONLINEPLAYERINFOINFO_RESP, pMsgHead, &stResp, enmTransType_P2P);
		return E_NULLPOINTER;
	}

	CGetOnlinePlayerInfoReq *pGetOnlinePlayerInfoReq = dynamic_cast<CGetOnlinePlayerInfoReq *>(pMsgBody);
	if(NULL == pGetOnlinePlayerInfoReq)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CGetOnlinePlayerInfoReq class failed![nMessageID=0x%08x, nSourceType=%d, nSourceID=%d]\n",
				pMsgHead->nMessageID, pMsgHead->nSourceType, pMsgHead->nSourceID);

		stResp.nResult = E_NULLPOINTER;
		SendMessageResponse(MSGID_HSIS_GETONLINEPLAYERINFOINFO_RESP, pMsgHead, &stResp, enmTransType_P2P, nOptionLen, (char *)pOptionData);

		return E_NULLPOINTER;
	}

	WRITE_DEBUG_LOG("receive get online player info request!{nSourceType=%d, nSourceID=%d, nWantCount=%d}\n",
			pMsgHead->nSourceType, pMsgHead->nSourceID, pGetOnlinePlayerInfoReq->nWantCount);

	//表示第一次开始请求获取在线玩家数据
	if(pGetOnlinePlayerInfoReq->nSeq == 0)
	{
		int32_t nTotalPlayerCount = 0;
		RoleID arrRoleID[MaxOnlinePlayerCount];

		g_PlayerMgt.GetAllPlayer(arrRoleID, MaxOnlinePlayerCount, nTotalPlayerCount, false);

		//保存当前在线玩家列表
		g_DataCenter.PushPlayers(pMsgHead->nSourceType, pMsgHead->nSourceID, arrRoleID, MaxOnlinePlayerCount, nTotalPlayerCount);
		WRITE_DEBUG_LOG("item get player info {count = %d }", nTotalPlayerCount);
	}

	int32_t nWantCount = pGetOnlinePlayerInfoReq->nWantCount;

	int32_t nPlayerCount = 0;
	CPlayer *arrPlayer[MaxGetOnlinePlayerCount];

	g_DataCenter.PopPlayers(pMsgHead->nSourceType, pMsgHead->nSourceID, arrPlayer, nWantCount, nPlayerCount);

	for(int32_t i = 0; i < nPlayerCount; ++i)
	{
		if(arrPlayer[i] == NULL)
		{
			continue;
		}

		COnlineNoti stNoti;
		stNoti.nRoleID = arrPlayer[i]->GetRoleID();
		stNoti.strRoleName = arrPlayer[i]->GetRoleName();
		stNoti.nAccountID = arrPlayer[i]->GetAccountID();
		stNoti.nVipLevel = arrPlayer[i]->GetVipLevel();
		stNoti.nLeftMoney = arrPlayer[i]->GetLeftMoney();
		stNoti.nLoginTime = arrPlayer[i]->GetLoginTime();
		stNoti.nConnUin = arrPlayer[i]->GetConnInfo();
		stNoti.nGender = arrPlayer[i]->GetPlayerGender();

		stResp.arrOnlinePlayer[i] = stNoti;
	}

	stResp.nResult = S_OK;
	stResp.nSeq = pGetOnlinePlayerInfoReq->nSeq;
	stResp.nPlayerCount = nPlayerCount;

	SendMessageResponse(MSGID_HSIS_GETONLINEPLAYERINFOINFO_RESP, pMsgHead, &stResp, enmTransType_P2P, nOptionLen, (char *)pOptionData);

	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END

