/*
 * bll_event_getroleglobalinfo.cpp
 *
 *  Created on: 2012-2-15
 *      Author: jimm
 */

#include "bll_event_getroleglobalinfo.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "roomserver_message_define.h"
#include "dal/dal_media.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CGetRoleGlobalInfoMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}
	// check the server status
	CGetRoleGlobalInfoResp stResp;
	stResp.nResult = enmGetGlobalPlayerInfoResult_Unknown;
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		SendMessageResponse(MSGID_HSRS_GET_ROLE_GLOBALINFO_RESP, pMsgHead, &stResp, enmTransType_P2P);
		return E_NULLPOINTER;
	}

	CGetRoleGlobalInfoReq *pGetRoleGlobalInfoReq = dynamic_cast<CGetRoleGlobalInfoReq *>(pMsgBody);
	if(NULL == pGetRoleGlobalInfoReq)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CGetRoleGlobalInfoReq class failed[nMessageID=0x%08x, nRoleID=%d]\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);

		stResp.nResult = E_NULLPOINTER;
		SendMessageResponse(MSGID_HSRS_GET_ROLE_GLOBALINFO_RESP, pMsgHead, &stResp, enmTransType_P2P, nOptionLen, (char *)pOptionData);

		return E_NULLPOINTER;
	}
	WRITE_INFO_LOG("receive room server get role global info request!{nRoleID=%d, nRoomID=%d}\n",
			pMsgHead->nRoleID, pMsgHead->nRoomID);

	CPlayer *pPlayer = NULL;
	int32_t ret = g_PlayerMgt.GetPlayer(pMsgHead->nRoleID, pPlayer);
	if(ret < 0 || pPlayer == NULL)
	{
		WRITE_ERROR_LOG("get role global info request, but failed to get player object!{RoleID=%d, ret=0x%08x}\n",
				pMsgHead->nRoleID, E_NULLPOINTER);

		stResp.nResult = E_NULLPOINTER;
		SendMessageResponse(MSGID_HSRS_GET_ROLE_GLOBALINFO_RESP, pMsgHead, &stResp, enmTransType_P2P, nOptionLen, (char *)pOptionData);
		return E_NULLPOINTER;
	}

	stResp.nResult = 0;
	stResp.nEnterCount = pPlayer->GetCurEnterRoomCount();
	stResp.nPlayerState = pPlayer->GetPlayerState();
	stResp.nEnterNewPlayerRoomCount = pPlayer->GetNewPlayerRoomCount();

	SendMessageResponse(MSGID_HSRS_GET_ROLE_GLOBALINFO_RESP, pMsgHead, &stResp, enmTransType_P2P, nOptionLen, (char *)pOptionData);

	return S_OK;
}


FRAME_HALLSERVER_NAMESPACE_END


