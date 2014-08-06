/*
 * bll_event_createroomnotice.cpp
 *
 *  Created on: 2012-2-12
 *      Author: jimm
 */


#include "bll_event_createroomnotice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "dal/from_server_message.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CCreateRoomNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
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

	CCreateRoomNotice *pCreateRoomNotice = dynamic_cast<CCreateRoomNotice *>(pMsgBody);
	if(NULL == pCreateRoomNotice)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CCreateRoomNotice class failed!{nMessageID=0x%08x, nRoleID=%d}\n",
				pMsgHead->nMessageID,  pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

	WRITE_INFO_LOG("receive create room notice!{nRoomID=%d, nServerID=%d}\n",
			pCreateRoomNotice->nRoomID, pCreateRoomNotice->nServerID);

	CRoom *pRoom = NULL;
	int32_t ret = g_RoomMgt.GetRoom(pCreateRoomNotice->nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		WRITE_DEBUG_LOG("cannot find room object, but we'll create it!{nRoomID=%d, nServerID=%d}\n",
				pCreateRoomNotice->nRoomID, pCreateRoomNotice->nServerID);
	}
	else
	{
		WRITE_DEBUG_LOG("found room object, and we'll set room data again!{nRoomID=%d, nServerID=%d}\n",
//				pRoom->GetRoomID(), pCreateRoomNotice->nServerID);//for test
				pCreateRoomNotice->nRoomID, pCreateRoomNotice->nServerID);
		pRoom->SetRoomServerID(pCreateRoomNotice->nServerID);
		return S_OK;
	}

	ret = g_RoomMgt.CreateRoom(pCreateRoomNotice->nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		WRITE_ERROR_LOG("create room object failed!{nRoomID=%d, nServerID=%d, ret=0x%08x}\n",
				pCreateRoomNotice->nRoomID, pCreateRoomNotice->nServerID, ret);
		return E_NULLPOINTER;
	}
	pRoom->SetRegionID(pCreateRoomNotice->nRegionID);
	UpdateRoomDataInfo(enmUpdateDataType_CreateRoom);//发送一次创建房间信息给tunnel
	pRoom->SetChannelID(pCreateRoomNotice->nChannelID);
	pRoom->SetRoomType(pCreateRoomNotice->nRoomType);
	pRoom->SetRoomID(pCreateRoomNotice->nRoomID);
	pRoom->SetRoomName(pCreateRoomNotice->strRoomName.GetString());
	pRoom->SetRoomServerID(pCreateRoomNotice->nServerID);
	pRoom->SetHostID(pCreateRoomNotice->nHostID);

//	UpdateRoomDataInfo(enmUpdateDataType_CreateRoom);//发送了7条创建信息给tunnel，后面的信息会失败导致未保存数据
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END

