/*
 * bll_event_builddatafinish.cpp
 *
 *  Created on: 2013-1-18
 *      Author: liufl
 */

#include "bll_event_builddatafinish.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CBuilddatafinishMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen , const void *pOptionData )
{
	g_ServerStatus = enmServerStatus_Running;
	//ªÿ”¶halldatacenter
	CHallServerIsRuningNoti CRuningNoti;
	SendMessageNotifyToServer(MSGID_HSTT_HALLSERVERISRUNING_NOTI, &CRuningNoti, 0, enmTransType_Broadcast,
			0, enmEntityType_Tunnel, 0, 0, NULL, "notify halldatacenter hallserver is runing!");
	return S_OK;
}



FRAME_HALLSERVER_NAMESPACE_END
