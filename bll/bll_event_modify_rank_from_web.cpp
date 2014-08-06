/*
 *  bll_event_modify_rank_from_web.cpp
 *
 *  To do：
 *  Created on: 	2012-4-11
 *  Author: 		luocj
 */

#include "bll_event_modify_rank_from_web.h"
#include "client_message_define.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CModifyRankNotiEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		return E_NULLPOINTER;
	}

	int32_t nRet = S_OK;
	if(pMsgHead == NULL || pMsgBody == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	switch(pMsgHead->nMessageID)
	{
		case MSGID_WEBOT_BATCH_MODIFY_ROLERANK_NOTIFY:
		{
			nRet = ModifyRoleRank(pMsgHead,pMsgBody,nOptionLen,pOptionData);
			if(0 > nRet)
			{
				WRITE_ERROR_LOG("modify RoleRank error!{nRoleID=%d}\n", pMsgHead->nRoleID);
				return E_UNKNOWN;
			}
		}
		break;
		default:
		WRITE_ERROR_LOG("modify role rank:get wrong message!{nMessageID=0x%08x}\n", pMsgHead->nMessageID);
		break;
	}
	return S_OK;
}

int32_t CModifyRankNotiEvent::ModifyRoleRank(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,const uint16_t nOptionLen, const void *pOptionData)
{
	int32_t nRet = S_OK;
	if(pMsgHead == NULL || pMsgBody == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	CBatchModifyRoomRoleRankNotify *pBatchModifyRoomRoleRankNotify = dynamic_cast<CBatchModifyRoomRoleRankNotify*>(pMsgBody);
	if(pBatchModifyRoomRoleRankNotify == NULL)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CBatchModifyRoomRoleRankNotify class failed!{nMessageID=0x%08x, nRoleID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}
	WRITE_INFO_LOG("get modify role rank request!{nRoomID=%d, nUserCount=%d}\n",
			pBatchModifyRoomRoleRankNotify->nRoomID, pBatchModifyRoomRoleRankNotify->nUserCount);

	//获取房间对象
	RoomID nRoomID = pBatchModifyRoomRoleRankNotify->nRoomID;

	//处理被修改的玩家的数据
	int32_t nModifyCount = pBatchModifyRoomRoleRankNotify->nUserCount;
	for(int32_t i = 0;i< nModifyCount;i++)
	{
		RoleID nRoleID = pBatchModifyRoomRoleRankNotify->arrRoleID[i];
		RoleRank nRoleRank = pBatchModifyRoomRoleRankNotify->arrNowRoleRank[i];
		//发送通知到 RoomServer和ItemServer
		CModifyTitleNoti stModifyTitleNoti;
		stModifyTitleNoti.nRoleID = nRoleID;
		stModifyTitleNoti.nRoomID = nRoomID;
		stModifyTitleNoti.nRoleRank = nRoleRank;
		//		SendMessageNotifyToServer(MSGID_HSTA_MODIFY_TITLE_NOTIFY,
		//				&stModifyTitleNoti, nRoomID, enmTransType_P2P, nRoleID,
		//				enmEntityType_Room, pRoom->GetRoomServerID(), 0, NULL,
		//				"send modify title notify to room server");
		SendMessageNotifyToServer(MSGID_HSTA_MODIFY_TITLE_NOTIFY,
				&stModifyTitleNoti, nRoomID, enmTransType_Broadcast,
				nRoleID, enmEntityType_Room, 0, 0, NULL,
				"send modify title notify to room server");
		SendMessageNotifyToServer(MSGID_HSTA_MODIFY_TITLE_NOTIFY,
				&stModifyTitleNoti, nRoomID, enmTransType_Broadcast,
				nRoleID, enmEntityType_Item, 0, 0, NULL,
				"send modify title notify to item server");
		//玩家不再房间的时候直接处理下一个
		//		if(!pRoom->IsPlayerInRoom(nRoleID))
		//		{
		//			WRITE_DEBUG_LOG("modify role rank, player is not in room!{nRoleID=%d, nRoomID=%d}\n",nRoleID,nRoomID);
		//			continue;
		//		}
		CPlayer* pPlayer = NULL;
		nRet = g_PlayerMgt.GetPlayer(nRoleID, pPlayer);
		if(nRet < 0 || pPlayer == NULL)
		{
			WRITE_DEBUG_LOG("modifyrolerank:player is not online");
			continue;
		}
		ModifyPlayerRoleRank(nRoomID, nRoleID, nRoleRank);
	}
	return S_OK;
}

int32_t CModifyRankNotiEvent::ModifyPlayerRoleRank(RoomID nRoomID,RoleID nDestRoleID,RoleRank nRoleRank)
{

	int32_t nRet = S_OK;
	//获取目标玩家
	CPlayer* pDestPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(nDestRoleID, pDestPlayer);
	if(0 > nRet || pDestPlayer == NULL)
	{
		WRITE_ERROR_LOG("modify role rank, cannot get role object!{nDestRoleID=%d}\n", nDestRoleID);
		return nRet;
	}
	RoleRank nRoleRankTemp = pDestPlayer->GetAdminRoleRank(nRoomID);
	bool isDel=false;
	if(nRoleRank==enmRoleRank_None)
	{
		WRITE_DEBUG_LOG("modify role rank, delete role rank from web!{nDestRoleID=%d, nRoomID=%d}\n",
				nDestRoleID, nRoomID);
		isDel = true;
	}
	//更新内存中的玩家权限
	RoleRank nPreRoleRank = enmRoleRank_None;
	nRet = pDestPlayer->SetAdminRoleRank(nRoomID, nRoleRank,nPreRoleRank);
	if(S_OK != nRet)
	{
		WRITE_ERROR_LOG("modify role rank, failed to set admin role rank!{nDestRoleID=%d, nRoomID=%d, nRoleRank=%d}\n",
				nDestRoleID, nRoomID, nRoleRank);
	}

	CRoom *pRoom = NULL;
	nRet = g_RoomMgt.GetRoom(nRoomID, pRoom);
	if(0 > nRet || pRoom == NULL)
	{
		WRITE_ERROR_LOG("get room object error!{nRoomID=%d, nRet=0x%08x}\n", nRoomID, nRet);
		return nRet;
	}

	if(nPreRoleRank == enmRoleRank_TempAdmin)
	{
		int32_t TemAdminCount = pRoom->GetTempAdminCount();
		if( TemAdminCount >= 1)
		{
			pRoom->SetTempAdminCount(--TemAdminCount);
		}
	}

	WRITE_DEBUG_LOG("succeed to modify title!{nDestRoleID=%d, nRoomID=%d, nRoleRank=%d}\n",
			nDestRoleID,nRoomID, nRoleRank);
	if(isDel)
	{
		CDelTitleNotify stDelTitleNotify;
		stDelTitleNotify.nDelRoleRank = nRoleRankTemp;
		stDelTitleNotify.nDestRoleID = nDestRoleID;
		stDelTitleNotify.nSrcRoleID = nDestRoleID;
		SendMessageNotifyToClient(MSGID_HSTA_DEL_TITLE_NOTIFY, &stDelTitleNotify, nRoomID,
				enmBroadcastType_All, nDestRoleID, 0, NULL, "notify other player in the room");
		return S_OK;
	}
	//发送通知到客户端
	CAddtitleNotify stAddTitleNotify;
	stAddTitleNotify.nAddRoleRank = nRoleRank;
	stAddTitleNotify.nDestRoleID = nDestRoleID;
	stAddTitleNotify.nSrcRoleID = nDestRoleID;
	SendMessageNotifyToClient(MSGID_HSCL_ADD_TITLE_NOTI, &stAddTitleNotify, nRoomID,
			enmBroadcastType_All, nDestRoleID, 0, NULL, "notify other player in the room");
	return S_OK;
}
FRAME_HALLSERVER_NAMESPACE_END
