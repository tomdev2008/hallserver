/*
 * bll_event_buildroomdatatimeout.cpp
 *
 *  Created on: 2012-8-6
 *      Author: zhurw
 */
#include "bll_event_buildroomdatatimeout.h"
FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CBuildRoomDataTimeoutEvent::OnTimerEvent(CFrameTimer *pTimer)
{
	WRITE_DEBUG_LOG("-----build room data time out-----");
	CBeginBuildPlayerNoti stBeginBuildPlayerNoti;
	GET_BLLBASE_INSTANCE().SendMessageNotifyToServer(MSGID_HSTT_BEGINBUILDPLAYER_NOTI, &stBeginBuildPlayerNoti, enmInvalidRoomID,
						enmTransType_Broadcast, enmInvalidRoomID, enmEntityType_Tunnel, enmInvalidServerID,
						0, NULL, "begin rebuild player notify");
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
