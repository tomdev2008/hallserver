/*
 * bll_event_regandfillindatanotice.cpp
 *
 *  Created on: 2012-5-25
 *      Author: jimm
 */

#include "client_message_define.h"
#include "ctl/server_datacenter.h"
#include "public_message_define.h"
#include "bll_event_regandfillindatanotice.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CRegAndFillInDataNotiEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	int32_t nRet = S_OK;
	// check the server status
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

	CRegAndFillInDataNotify* pRegAndFillInDataNoti = dynamic_cast<CRegAndFillInDataNotify*>(pMsgBody);
	if(pRegAndFillInDataNoti == NULL)
	{
		WRITE_ERROR_LOG("null pointer: pMsgBody transform to CRegAndFillInDataNotify class failed!{nMessageID=0x%08x, nRoleID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}
	WRITE_INFO_LOG("receive register and fill in data notify!{nRoleID=%d, strRoleName=%s, nGender=%d, nAccountType=%d}\n",
			pRegAndFillInDataNoti->nRoleID, pRegAndFillInDataNoti->strRoleName.GetString(),
			pRegAndFillInDataNoti->nGender, pRegAndFillInDataNoti->nAccountType);

	CPlayer *pPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(pRegAndFillInDataNoti->nRoleID, pPlayer);
	if(nRet < 0 || pPlayer == NULL)
	{
		WRITE_ERROR_LOG("register and fill in data notify, cannot find role object!{nRoleID=%d, ret=0x%08x}\n",
				pRegAndFillInDataNoti->nRoleID, nRet);
		return E_NULLPOINTER;
	}

	pPlayer->SetRoleName(pRegAndFillInDataNoti->strRoleName.GetString());
	pPlayer->SetPlayerGender(pRegAndFillInDataNoti->nGender);

	CUpdateUserInfoNotify stNoti;
	stNoti.strRoleName = pRegAndFillInDataNoti->strRoleName;
	stNoti.nGender = pRegAndFillInDataNoti->nGender;

	SendMessageNotifyToClient(MSGID_HSCL_UPDATEUSERINFONOTI, &stNoti, enmInvalidRoomID, enmBroadcastType_ExpectPlayr, pRegAndFillInDataNoti->nRoleID);

	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
