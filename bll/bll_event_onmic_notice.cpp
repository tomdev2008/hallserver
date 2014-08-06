/*
 *  bll_event_onmic_notice.cpp
 *
 *  To do：
 *  Created on: 	2012-5-8
 *  Author: 		luocj
 */

#include "bll_event_onmic_notice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "dal/from_server_message.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t COnMicNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
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

	COnMicNoti *pOnMicNoti = dynamic_cast<COnMicNoti *>(pMsgBody);
	if(NULL == pOnMicNoti)
	{
		WRITE_ERROR_LOG("null pointer: pMsgBody transform to COnMicNoti class failed!{nMessageID=0x%08x, nRoleID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

	WRITE_INFO_LOG("receive on microphone notify!{nOnMicRoleID=%d}\n",  pOnMicNoti->nOnMicRoleID);

	CPlayer *pPlayer = NULL;
	int32_t ret = g_PlayerMgt.GetPlayer(pOnMicNoti->nOnMicRoleID, pPlayer);
	if(ret < 0 || pPlayer == NULL)
	{
		WRITE_ERROR_LOG("on microphone notify, cannot find player object!{nOnMicRoleID=%d}\n",  pOnMicNoti->nOnMicRoleID);
		return E_NULLPOINTER;
	}
	PlayerState nPlayerState = pPlayer->GetPlayerState();
	if(nPlayerState==enmPlayerState_PublicMic||nPlayerState==enmPlayerState_PrivateMic||nPlayerState==enmPlayerState_P2PMic)
	{
		WRITE_ERROR_LOG("room server and hall server's player state got error!{nPlayerState=%d}\n", nPlayerState);
		//发送同步消息(两遍)
		for(int32_t i = 0;i<2;i++ )
		{
			CHallUpdateGlobalStateNoti stHallUpdateGlobalStateNoti;
			stHallUpdateGlobalStateNoti.nRoleID = pOnMicNoti->nOnMicRoleID;
			stHallUpdateGlobalStateNoti.nPlayerState= nPlayerState;
			SendMessageNotifyToServer(MSGID_HSRS_UPDATEGLOBALSTATE_NOTI, &stHallUpdateGlobalStateNoti, enmInvalidRoomID,
					enmTransType_Broadcast, pOnMicNoti->nOnMicRoleID, enmEntityType_Room);
		}
        return S_OK;
	}
	//处理玩家的状态
	switch(pOnMicNoti->nMicType)
	{
		case enmMicType_PublicMic:
		{
			pPlayer->SetPlayerState(enmPlayerState_PublicMic);
		}
		break;
		case enmMicType_PublicMicQueue:
		{

		}
		break;
		case enmMicType_PrivateMic:
		{
			pPlayer->SetPlayerState(enmPlayerState_PrivateMic);
		}
		break;
		case enmMicType_P2PMic:
		{
			pPlayer->SetPlayerState(enmPlayerState_P2PMic);
		}
		break;
		default:
		break;
	}

	//发送更新玩家的信息到roomserver
	CHallUpdateGlobalStateNoti stHallUpdateGlobalStateNoti;
	stHallUpdateGlobalStateNoti.nRoleID = pOnMicNoti->nOnMicRoleID;
	stHallUpdateGlobalStateNoti.nPlayerState= pPlayer->GetPlayerState();
	SendMessageNotifyToServer(MSGID_HSRS_UPDATEGLOBALSTATE_NOTI, &stHallUpdateGlobalStateNoti, enmInvalidRoomID,
			enmTransType_Broadcast, pOnMicNoti->nOnMicRoleID, enmEntityType_Room);
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
