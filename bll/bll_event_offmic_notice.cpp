/*
 *  bll_event_offmic_notice.cpp
 *
 *  To do：
 *  Created on: 	2012-5-8
 *  Author: 		luocj
 */

#include "bll_event_offmic_notice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "dal/from_server_message.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t COffMicNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
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

	COffMicNoti *pOffMicNoti = dynamic_cast<COffMicNoti *>(pMsgBody);
	if(NULL == pOffMicNoti)
	{
		WRITE_ERROR_LOG("null pointer: pMsgBody transform to COffMicNoti class failed!{nMessageID=0x%08x, nRoleID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

	WRITE_INFO_LOG("receive off microphone notify!{nOffMicRoleID=%d}\n",  pOffMicNoti->nOffMicRoleID);

	CPlayer *pPlayer = NULL;
	int32_t ret = g_PlayerMgt.GetPlayer(pOffMicNoti->nOffMicRoleID, pPlayer);
	if(ret < 0 || pPlayer == NULL)
	{
		WRITE_WARNING_LOG("off microphone notify, cannot find player object!{nOffMicRoleID=%d}\n", pOffMicNoti->nOffMicRoleID);
		return E_NULLPOINTER;
	}
	PlayerState nPlayerState = pPlayer->GetPlayerState();
	if(nPlayerState != enmPlayerState_PublicMic && nPlayerState != enmPlayerState_PrivateMic && nPlayerState != enmPlayerState_P2PMic)
	{
		WRITE_WARNING_LOG("room server and hall server's player state got error!{nPlayerState=%d}\n", nPlayerState);
	}
	//处理玩家的状态
	switch(pOffMicNoti->nMicType)
	{
		case enmMicType_PublicMic:
		{
			pPlayer->SetPlayerState(enmPlayerState_InRoom);
		}
		break;
		case enmMicType_PublicMicQueue:
		{

		}
		break;
		case enmMicType_PrivateMic:
		{
			pPlayer->SetPlayerState(enmPlayerState_InRoom);
		}
		break;
		case enmMicType_P2PMic:
		{
			pPlayer->SetPlayerState(enmPlayerState_InRoom);
		}
		break;
		default:
		break;
	}

	//发送更新玩家的信息到roomserver
	CHallUpdateGlobalStateNoti stHallUpdateGlobalStateNoti;
	stHallUpdateGlobalStateNoti.nRoleID = pOffMicNoti->nOffMicRoleID;
	stHallUpdateGlobalStateNoti.nPlayerState= pPlayer->GetPlayerState();
	SendMessageNotifyToServer(MSGID_HSRS_UPDATEGLOBALSTATE_NOTI, &stHallUpdateGlobalStateNoti, enmInvalidRoomID,
			enmTransType_Broadcast, pOffMicNoti->nOffMicRoleID, enmEntityType_Room);
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
