/*
 * bll_event_statuschangenoti_from_web.cpp
 *
 *  Created on: 2012-8-28
 *      Author: zhurw
 */
#include "bll_event_statuschangenoti_from_web.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CStatusChangeNotiFromWeb::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen, const void *pOptionData)
{
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		return E_NULLPOINTER;
	}


	if(pMsgHead == NULL || pMsgBody == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	CPwdProStatusChangeNoti* pPwdStatusChangeNoti = dynamic_cast<CPwdProStatusChangeNoti*>(pMsgBody);
	if(pPwdStatusChangeNoti == NULL)
	{
		WRITE_ERROR_LOG("null pointer: pMsgBody transform to CPwdProStatusChangeNoti class failed!"
				"{nMessageID=0x%08x, nRoleID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

	WRITE_DEBUG_LOG("status change notify from web!{nRoleID=%d,PwdProStatus=%d}", pPwdStatusChangeNoti->nRoleID, pPwdStatusChangeNoti->nCurPwdProStatus);

	RoleID nRoleID = pPwdStatusChangeNoti->nRoleID;
	CPlayer* pPlayer = NULL;
	int32_t nRet = S_OK;
	nRet = g_PlayerMgt.GetPlayer(nRoleID, pPlayer);
	if(nRet < 0 || pPlayer == NULL)
	{
		WRITE_DEBUG_LOG("statuschange:player is not online!{nRoleID=%d}", nRoleID);
		return nRet;
	}

	CUpdatePWDStatusNotify stUpdatePwdStatus;
	SendMessageNotifyToClient(MSGID_SSCL_UPDATEPWDSTATUS_NOTI, &stUpdatePwdStatus, enmInvalidRoomID, enmBroadcastType_ExpectPlayr, nRoleID);
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END


