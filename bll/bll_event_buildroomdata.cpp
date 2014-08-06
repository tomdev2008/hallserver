/*
 * bll_event_buildroomdata.cpp
 *
 *  Created on: 2012-5-17
 *      Author: fuz
 */

#include "bll_event_buildroomdata.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "hallserver_message_define.h"
#include "client_message_define.h"
#include "dal/to_server_message.h"
#include "dal/from_server_message.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

//static uint32_t nFinishBuildRoomTunnelCount = 0;
int32_t CBuildRoomDataMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody == NULL || pMsgHead == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}
	CRecoverHallRoomNoti *pNoti = dynamic_cast<CRecoverHallRoomNoti *>(pMsgBody);
	if(pNoti == NULL)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CRecoverHallRoomNoti class failed!{nMessageID=0x%08x, nSourceID=%d}\n",
				pMsgHead->nMessageID,  pMsgHead->nSourceID);
		return E_NULLPOINTER;
	}

	WRITE_INFO_LOG("got build room data message notify!{nRoomCount=%d}\n", pNoti->nRoomCount);


	for(int32_t i = 0; i < pNoti->nRoomCount; ++i)
	{
		CRoom* pRoom = NULL;
		int32_t ret = g_RoomMgt.GetRoom(pNoti->arrRoomID[i], pRoom);
		if(ret < 0 || pRoom == NULL)
		{
			// build a room by room cache
			ret = g_RoomMgt.CreateRoom(pNoti->arrRoomID[i], pRoom);
			if(ret < 0 || pRoom == NULL)
			{
				continue;
			}

			uint32_t nStartPos = pRoom->GetStartPos();
			uint32_t nEndPos = pRoom->GetEndPos();
			uint32_t nRoomDataSize = nEndPos - nStartPos;
			if(nRoomDataSize >= pNoti->arrRoomDataSize[i])
			{
				memcpy(((uint8_t *)pRoom) + nStartPos, pNoti->arrRoomData[i],
						pNoti->arrRoomDataSize[i]);
			}
			pRoom->SetTunnelServerID(pMsgHead->nSourceID);
			WRITE_DEBUG_LOG("build room data!{nRoomID=%d, nCurUserCount=%d, nTunnelServerID=%d}\n",
					pRoom->GetRoomID(), pRoom->GetCurUserCount(), pRoom->GetTunnelServerID());
		}
	}



	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
