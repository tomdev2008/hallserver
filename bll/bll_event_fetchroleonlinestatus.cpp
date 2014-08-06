/*
 * bll_event_fetchroleonlinestatus.cpp
 *
 *  Created on: 2012-8-10
 *      Author: zhurw
 */
#include "bll_event_fetchroleonlinestatus.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "dal/from_server_message.h"
#include "webagent_message_define.h"
#include "public_typedef.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CFetchRoleOnlineStatusEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody == NULL || pMsgHead == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}
	CGetOnlineStatusResp stResp;
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		stResp.nResult = enmFetchRoleOnlineStatusResult_Unknown;
		SendMessageResponse(MSGID_GET_ONLINE_STATUS_RESP, pMsgHead, &stResp, enmTransType_P2P);
		return E_NULLPOINTER;
	}
	CGetOnlineStatusReq* pGetOnlineStatusReq = dynamic_cast<CGetOnlineStatusReq*>(pMsgBody);
	if(pGetOnlineStatusReq == NULL)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CGetOnlineStatusReq class failed![nMessageID=0x%08x, nSourceType=%d, nSourceID=%d]\n",
				pMsgHead->nMessageID, pMsgHead->nSourceType, pMsgHead->nSourceID);

		stResp.nResult = E_NULLPOINTER;
		SendMessageResponse(MSGID_GET_ONLINE_STATUS_RESP, pMsgHead, &stResp, enmTransType_P2P);

		return E_NULLPOINTER;
	}

	int32_t nFetchCount = pGetOnlineStatusReq->nPlayerCount;
	stResp.nResult = S_OK;

	WRITE_DEBUG_LOG("fetch online status:web get role online status!{nPlayerCount=%d}",nFetchCount);

	for(int i = 0; i < nFetchCount; i++)
	{
		RoleID nRoleID = enmInvalidRoleID;
		CPlayer* pPlayer = NULL;
		nRoleID = pGetOnlineStatusReq->arrRoleID[i];
		stResp.arrRoleID[i] = nRoleID;
		stResp.nPlayerCount++;
		int ret = g_PlayerMgt.GetPlayer(nRoleID, pPlayer);
		if(ret < 0 || pPlayer == NULL)
		{
			stResp.arrStatus[i] = 0;
		}
		else
		{
			stResp.arrStatus[i] = 1;
		}
		if(i >= MaxGetPlayerOnlineStatusNum - 1)
		{
			break;
		}
	}

	SendMessageResponse(MSGID_GET_ONLINE_STATUS_RESP, pMsgHead, &stResp, enmTransType_P2P);
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
