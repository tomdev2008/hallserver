/*
 * bll_event_delroomnotice.cpp
 *
 *  Created on: 2012-7-24
 *      Author: zhurw
 */
#include "bll_event_delroomnotice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "dal/from_server_message.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CDelRoomNoticeEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	// check the server status
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		return E_NULLPOINTER;
	}

	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	CDestoryRoomNotice* pDestoryRoomNotice = dynamic_cast<CDestoryRoomNotice*>(pMsgBody);
	if(NULL == pDestoryRoomNotice)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CDestoryRoomNotice_Public class failed![nMessageID=0x%08x, nRoleID=%d]\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

	WRITE_DEBUG_LOG("destory room notice!{nRoomID=%d, nServerID=%d}", pDestoryRoomNotice->nRoomID, pDestoryRoomNotice->nServerID);
	g_RoomMgt.DestoryRoom(pDestoryRoomNotice->nRoomID);
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
