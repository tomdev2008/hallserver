/*
 * bll_event_checkdatanum.cpp
 *
 *  Created on: 2012-5-24
 *      Author: fuz
 */

#include "bll_event_checkdatanum.h"
#include "ctl/server_datacenter.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CCheckDataNumEvent::OnTimerEvent(CFrameTimer *pTimer)
{
	WRITE_DEBUG_LOG("-----------on check data number timer----------\n");

	if(g_ServerStatus != enmServerStatus_Running)
	{
//		CBeginBuildPlayerNoti stBeginBuildPlayerNoti;
//		GET_BLLBASE_INSTANCE().SendMessageNotifyToServer(MSGID_HSTT_BEGINBUILDPLAYER_NOTI, &stBeginBuildPlayerNoti, enmInvalidRoomID,
//							enmTransType_Broadcast, enmInvalidRoomID, enmEntityType_Tunnel, enmInvalidServerID,
//							0, NULL, "begin rebuild player notify");
		WRITE_DEBUG_LOG("not all tunnel finished, but timeout, set server status to be running!{timeout=%ds}\n", enmCheckDataNumTimePeriod);
		g_ServerStatus = enmServerStatus_Running;
		return S_OK;
	}

	WRITE_DEBUG_LOG("all tunnel finished!\n");
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
