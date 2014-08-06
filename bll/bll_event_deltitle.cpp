/*
 * bll_event_deltitle.cpp
 *
 *  Created on: 2012-3-7
 *      Author: zhurw
 */

#include "bll_event_deltitle.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CDelTitleEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	// check the server status
	CDelTitleResp stDelTitleResp;
	stDelTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
	if(g_ServerStatus != enmServerStatus_Running)
	{
		SendMessageResponse(MSGID_HSCL_DEL_TITLE_RESP, pMsgHead, &stDelTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		WRITE_ERROR_LOG("server is not ready!\n");
		return E_NULLPOINTER;
	}

	if(pMsgHead == NULL || pMsgBody == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}
	switch(pMsgHead->nMessageID)
	{
		case MSGID_CLHS_DEL_TITLE_REQ:
		OnDelTitleEvent(pMsgHead, pMsgBody, nOptionLen, pOptionData);
		break;
		default:
		WRITE_ERROR_LOG("delete title request, get wrong message id!{nMessageID=0x%08x}\n", pMsgHead->nMessageID);
		break;
	}

	return S_OK;
}

int32_t CDelTitleEvent::OnDelTitleEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgHead == NULL || pMsgBody == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	CDelTitleReq* pDelTitleReq = dynamic_cast<CDelTitleReq*>(pMsgBody);
	switch(pDelTitleReq->nDelRoleRank)
	{
		case enmRoleRank_MinorHost:
		//To Do:处理删除副室主操作
			break;
		case enmRoleRank_Admin:
		//To Do:处理删除管理员操作
			break;
		case enmRoleRank_TempAdmin:
		//To Do:处理删除临时管理员操作
		{
			DelTempAdmin(pMsgHead, pDelTitleReq, nOptionLen, pOptionData);
		}
			break;
		default:
			WRITE_ERROR_LOG("delete title request, got wrong title!{nDelRoleRank=%d}", pDelTitleReq->nDelRoleRank);
			break;
	}

	return S_OK;
}

int32_t CDelTitleEvent::DelTempAdmin(MessageHeadSS * pMsgHead, CDelTitleReq* pDelTitleReq,
		const uint16_t nOptionLen, const void *pOptionData)
{
	int32_t nRet = S_OK;
	if(pMsgHead == NULL || pDelTitleReq == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pDelTitleReq=0x%08x}\n", pMsgHead, pDelTitleReq);
		return E_NULLPOINTER;
	}
	WRITE_INFO_LOG("receive delete title event!{nSourceRoleID=%d, nDestRoleID=%d, nDelRoleRank=%d}\n",
			pMsgHead->nRoleID, pDelTitleReq->nDestRoleID, pDelTitleReq->nDelRoleRank);

	CDelTitleResp stDelTitleResp;
	//获取请求者玩家，并判断是否具有删除临管权限
	CPlayer* pSrcPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(pMsgHead->nRoleID, pSrcPlayer);
	if(0 > nRet || pSrcPlayer == NULL)
	{
		WRITE_ERROR_LOG("delete title event, failed to get source player object!{nRoleID=%d}\n",
				pMsgHead->nRoleID);
		stDelTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_DEL_TITLE_RESP, pMsgHead, &stDelTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return nRet;
	}
	RoleRank nRoleRank = pSrcPlayer->GetAdminRoleRank(pMsgHead->nRoomID);
	bool isOfficePlayer = pSrcPlayer->IsOfficialPlayer();
	if(nRoleRank < enmRoleRank_Admin&&!isOfficePlayer)
	{
		WRITE_ERROR_LOG("delete title event, source player has no permission to delete tempadmin!{nRoleID=%d}\n",
				pMsgHead->nRoleID);
		stDelTitleResp.nChangeResult = enmChangeResult_NO_PERMISSION;
		SendMessageResponse(MSGID_HSCL_DEL_TITLE_RESP, pMsgHead, &stDelTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return nRet;
	}

	//获取目标玩家，判断他是否为该房间临管
	CPlayer* pDestPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(pDelTitleReq->nDestRoleID, pDestPlayer);
	if(0 > nRet || pDestPlayer == NULL)
	{
		WRITE_ERROR_LOG("delete title event, failed to get desti player object!{nDestRoleID=%d}\n",
				pDelTitleReq->nDestRoleID);
		stDelTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_DEL_TITLE_RESP, pMsgHead, &stDelTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return nRet;
	}
	RoleRank nDestPlayerRoleRank = pDestPlayer->GetAdminRoleRank(pMsgHead->nRoomID);
	if(nDestPlayerRoleRank != enmRoleRank_TempAdmin)
	{
		WRITE_ERROR_LOG("delete title event, desti player is not tempadmin!{nDestRoleID=%d, nDestPlayerRoleRank=%d}",
				pDelTitleReq->nDestRoleID, nDestPlayerRoleRank);
		stDelTitleResp.nChangeResult = enmChangeResult_NO_PERMISSION;
		SendMessageResponse(MSGID_HSCL_DEL_TITLE_RESP, pMsgHead, &stDelTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return nRet;
	}
	nRet = pDestPlayer->SetAdminRoleRank(pMsgHead->nRoomID, enmRoleRank_None);
	if(S_OK != nRet)
	{
		WRITE_ERROR_LOG("delete title event, desti player failed to delete tempadmin!{nRet=0x%08x}\n", nRet);
		stDelTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_DEL_TITLE_RESP, pMsgHead, &stDelTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return nRet;
	}

	CRoom *pRoom = NULL;
	nRet = g_RoomMgt.GetRoom(pMsgHead->nRoomID, pRoom);
	if(0 > nRet || pRoom == NULL)
	{
		WRITE_ERROR_LOG("delete title event, failed to get room object!{nRoomID=%d}\n", pMsgHead->nRoomID);
		stDelTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_DEL_TITLE_RESP, pMsgHead, &stDelTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return nRet;
	}
	int32_t nCurTempAdminCount = pRoom->GetTempAdminCount();
	if(nCurTempAdminCount >= 1)
	{
		pRoom->SetTempAdminCount(--nCurTempAdminCount);
	}
	SendRespAndNotify(pMsgHead, pDelTitleReq, nOptionLen, pOptionData);

	return S_OK;
}

int32_t CDelTitleEvent::SendRespAndNotify(MessageHeadSS * pMsgHead, CDelTitleReq* pDelTitleReq,
		const uint16_t nOptionLen, const void *pOptionData)
{
	int32_t nRet = S_OK;
	CDelTitleResp stDelTitleResp;
	stDelTitleResp.nChangeResult = enmChangeResult_OK;
	WRITE_DEBUG_LOG("delete title event, send response to client!{nChangeResult=%d}\n", enmChangeResult_OK);
	SendMessageResponse(MSGID_HSCL_DEL_TITLE_RESP, pMsgHead, &stDelTitleResp,
			enmTransType_P2P, nOptionLen, (char*)pOptionData);

	CDelTitleNotify stDelTitleNotify;
	stDelTitleNotify.nDelRoleRank = pDelTitleReq->nDelRoleRank;
	stDelTitleNotify.nDestRoleID = pDelTitleReq->nDestRoleID;
	stDelTitleNotify.nSrcRoleID = pMsgHead->nRoleID;
	WRITE_DEBUG_LOG("delete title event, send notify to client, room and item server!{nChangeResult=%d}\n", enmChangeResult_OK);
	SendMessageNotifyToClient(MSGID_HSTA_DEL_TITLE_NOTIFY, &stDelTitleNotify, pMsgHead->nRoomID,
			enmBroadcastType_ExceptPlayr, pMsgHead->nRoleID, 0, NULL, "notify other player");

	CModifyTitleNoti stModifyTitleNoti;
	stModifyTitleNoti.nRoleID = pDelTitleReq->nDestRoleID;
	stModifyTitleNoti.nRoleRank = enmRoleRank_None;
	stModifyTitleNoti.nRoomID = pMsgHead->nRoomID;
	CRoom* pRoom = NULL;
	nRet = g_RoomMgt.GetRoom(pMsgHead->nRoomID, pRoom);
	if(0 > nRet)
	{
		WRITE_ERROR_LOG("delete title event, failed to get room object!{nRoomID=%d}\n", pMsgHead->nRoomID);
		return nRet;
	}

//	WRITE_DEBUG_LOG("delete title event, send notify to room and item server!{nChangeResult=%d}\n", enmChangeResult_OK);
//	SendMessageNotifyToServer(MSGID_HSTA_MODIFY_TITLE_NOTIFY, &stModifyTitleNoti, pMsgHead->nRoomID,
//			enmTransType_P2P, pDelTitleReq->nDestRoleID, enmEntityType_Room, pRoom->GetRoomServerID(),
//			nOptionLen, (char*)pOptionData, "send delete title notify");
	SendMessageNotifyToServer(MSGID_HSTA_MODIFY_TITLE_NOTIFY, &stModifyTitleNoti, pMsgHead->nRoomID,
			enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_Room, 0, nOptionLen,
			(char*)pOptionData, "send delete title notify");

//	WRITE_DEBUG_LOG("delete title event, send notify to item server!{nChangeResult=%d}\n", enmChangeResult_OK);
	SendMessageNotifyToServer(MSGID_HSTA_MODIFY_TITLE_NOTIFY, &stModifyTitleNoti, pMsgHead->nRoomID,
			enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_Item, 0, nOptionLen,
			(char*)pOptionData, "send delete title notify");
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
