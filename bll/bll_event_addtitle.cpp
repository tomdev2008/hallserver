/*
 * bll_event_addtitle.cpp
 *
 *  Created on: 2012-2-28
 *      Author: zhurw
 */

#include "bll_event_addtitle.h"
#include "hallserver_message_define.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CAddTitleEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	// TODO Auto-generated constructor stub
	if(pMsgHead == NULL || pMsgBody == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}
	// check the server status
	CAddTitleResp stAddTitleResp;
	stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
	if(g_ServerStatus != enmServerStatus_Running)
	{
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		WRITE_ERROR_LOG("server is not ready!\n");
		return E_NULLPOINTER;
	}

	switch(pMsgHead->nMessageID)
	{
		case MSGID_CLHS_ADD_TITLE_REQ:
			{
				WRITE_DEBUG_LOG("receive add title request!{nRoleID=%d, nRoomID=%d}\n", pMsgHead->nRoleID, pMsgHead->nRoomID);
				OnAddTitleEvent(pMsgHead, pMsgBody, nOptionLen, pOptionData);
			}
			break;
		default:
			WRITE_ERROR_LOG("add title request, get wrong message id!{nMessageID=0x%08x}\n", pMsgHead->nMessageID);
			break;
	}

	return S_OK;

}

int32_t CAddTitleEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	// check the server status
	CAddTitleResp stAddTitleResp;
	stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		return E_NULLPOINTER;
	}

	// TODO Auto-generated destructor stub
	int32_t nRet = S_OK;

	if(pMsgHead == NULL || pMsgBody == NULL || pSession == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x, pSession=0x%08x}\n", pMsgHead, pMsgBody, pSession);
		return E_NULLPOINTER;
	}

	//	int32_t nSessionWaitMsg = pSession->GetSessionWaitMsg();
	switch(pSession->GetSessionType())
	{
		case enmSessionType_UpdateTitle:
		{
			WRITE_DEBUG_LOG("receive update title session from db\n");
			CHallSetTitleResp* pHallSetTitleResp = dynamic_cast<CHallSetTitleResp*>(pMsgBody);
			if(NULL == pHallSetTitleResp)
			{
				WRITE_ERROR_LOG("null pointer:pMsgBody transform to CHallSetTitleResp class failed!{nSessionType=0x%04X, nSessionId=%d}\n",
						pSession->GetSessionType(), pSession->GetSessionIndex());
				return E_UNKNOWN;
			}
			CUpdateRankSessionData* pUpdateSessionData = (CUpdateRankSessionData*)(pSession->GetSessionData());
			if(NULL == pUpdateSessionData)
			{
				WRITE_ERROR_LOG("null pointer:get pUpdateSessionData failed!{nSessionType=0x%04X, nSessionId=%d}\n",
						pSession->GetSessionType(), pSession->GetSessionIndex());
				return E_UNKNOWN;
			}
			if(0 > pHallSetTitleResp->nResult)
			{
				CAddTitleResp stAddTitleResp;
				WRITE_ERROR_LOG("set title response failed!{nResult=0x%08x}\n", pHallSetTitleResp->nResult);
				stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
				SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, &pUpdateSessionData->m_sMsgHead, &stAddTitleResp,
						enmTransType_P2P, pUpdateSessionData->m_nOptionLen, pUpdateSessionData->m_arrOptionData);
				return E_UNKNOWN;
			}
			else if(S_OK == pHallSetTitleResp->nResult)
			{
				nRet = AddRoleRank(&pUpdateSessionData->m_sMsgHead, &pUpdateSessionData->m_pMsgBody, pUpdateSessionData->m_pMsgBody.nDestRoleID,
						pUpdateSessionData->m_pMsgBody.nAddRoleRank, pUpdateSessionData->m_nOptionLen, pUpdateSessionData->m_arrOptionData);
				return nRet;
			}
		}
		break;
		case enmSessionType_GetAdminCount:
		{
			WRITE_DEBUG_LOG("receive get admin count session from db!\n");
			OnSessionAddTitle(pMsgHead, pMsgBody, pSession, nOptionLen,pOptionData);
		}
		break;
		default:
		WRITE_ERROR_LOG("add title session, wrong session type!{nSessionType=0x%04X}\n", pSession->GetSessionType());
		break;
	}

	return S_OK;
}

int32_t CAddTitleEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	WRITE_ERROR_LOG("session timeout:add title session time out\n");
	if(pSession == NULL)
	{
		WRITE_ERROR_LOG("session timeout:null pointer:add title timeout and pSession is NULL!{ret=0x%08x}\n", E_NULLPOINTER);
		return E_NULLPOINTER;
	}
	CAddTitleResp stAddTitleResp;
	switch(pSession->GetSessionType())
	{
		case enmSessionType_GetAdminCount:
		{
			CSetTitleSessionData * pSessionData = (CSetTitleSessionData *)(pSession->GetSessionData());
			if(NULL== pSessionData)
			{
				WRITE_ERROR_LOG("session timeout:null pointer:get admin count session timeout and pSessionData is null!{nSessionIndex=%d, ret=0x%08x}\n",
						pSession->GetSessionIndex(), E_NULLPOINTER);
				return E_NULLPOINTER;
			}
			//发送失败回应
			stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
			SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, &pSessionData->m_sMsgHead, &stAddTitleResp,
					enmTransType_P2P, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);
			WRITE_ERROR_LOG("session timeout:add title timeout, get admin count failed!{nChangeResult=0x%08x}\n", enmChangeResult_UNKNOWN);
		}
		break;
		case enmSessionType_UpdateTitle:
		{
			CUpdateRankSessionData* pUpdateSessionData = (CUpdateRankSessionData*)(pSession->GetSessionData());
			if(NULL== pUpdateSessionData)
			{
				WRITE_ERROR_LOG("session timeout:null pointer:update title session time out and pUpdateSessionData is null!{nSessionIndex=%d, ret=0x%08x}\n",
						 pSession->GetSessionIndex(), E_NULLPOINTER);
				return E_NULLPOINTER;
			}
			stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
			SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, &pUpdateSessionData->m_sMsgHead, &stAddTitleResp,
					enmTransType_P2P, pUpdateSessionData->m_nOptionLen, pUpdateSessionData->m_arrOptionData);
			WRITE_ERROR_LOG("session timeout:update title failed, session timeout!{nChangeResult=0x%08x}\n", enmChangeResult_UNKNOWN);
		}
		break;
		default:
		WRITE_ERROR_LOG("session timeout:add title event, wrong session type!{nSessionType=0x%04X}\n", pSession->GetSessionType());
		break;
	}
	return S_OK;
}

int32_t CAddTitleEvent::OnAddTitleEvent(MessageHeadSS* pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	int32_t nRet = S_OK;
	CAddTitleResp stAddTitleResp;

	if(pMsgHead == NULL || pMsgBody == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	WRITE_NOTICE_LOG("receive player add title request!{nRoleID=%d}\n",pMsgHead->nRoleID);

	CPlayer* pSrcPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(pMsgHead->nRoleID, pSrcPlayer);
	//判断发请求的玩家是否存在
	if(0 > nRet || pSrcPlayer == NULL)
	{
		WRITE_ERROR_LOG("add title event, failed to get source player object!{nRoleID=%d}\n",
				pMsgHead->nRoleID);
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return nRet;
	}
	//获取房间
	CRoom* pRoom = NULL;
	nRet = g_RoomMgt.GetRoom(pMsgHead->nRoomID, pRoom);
	if(0 > nRet || pRoom == NULL)
	{
		WRITE_ERROR_LOG("add title event, failed to get room object!{nRoomID=%d}\n", pMsgHead->nRoomID);
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return nRet;
	}
	//判断玩家是否在房间nRoomID中
	if(!(pRoom->IsPlayerInRoom(pMsgHead->nRoleID)))
	{
		WRITE_ERROR_LOG("add title event, source player is not in the room!{nRoomID=%d, nRoleID=%d}\n",
				pMsgHead->nRoomID, pMsgHead->nRoleID);
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return E_HS_PLAYRENOTINROOM;
	}

	CAddTitleReq* pAddTitleReq = dynamic_cast<CAddTitleReq*>(pMsgBody);
	if(pAddTitleReq == NULL)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CAddTitleReq class failed!{nMessageID=0x%08x, nRoleID=%d}\n",
				pMsgHead->nMessageID,  pMsgHead->nRoleID);

		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return E_NULLPOINTER;
	}

	//获取目标玩家
	CPlayer* pDestPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(pAddTitleReq->nDestRoleID, pDestPlayer);
	if(0 > nRet || pDestPlayer == NULL)
	{
		WRITE_ERROR_LOG("add title event, failed to get desti player object!{nDestRoleID=%d}\n",
				pAddTitleReq->nDestRoleID);
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return nRet;
	}

	//是否为机器人，机器人返回enmChangeResult_Is_AdminInOther(如果是加临管则不管)
	if(pDestPlayer->IsRobot()&&(pAddTitleReq->nAddRoleRank!=enmRoleRank_TempAdmin))
	{
		WRITE_DEBUG_LOG("add title event, player is robot:{nDestRoleID=%d, nChangeResult=%d}\n",
				pDestPlayer->GetRoleID(), enmChangeResult_Is_AdminInOther);
		stAddTitleResp.nChangeResult = enmChangeResult_Is_AdminInOther;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return E_HS_NOPERMISSIONS;
	}

	if(!(pRoom->IsPlayerInRoom(pDestPlayer->GetRoleID())))
	{
		WRITE_ERROR_LOG("add title event, desti player is not in the room!{nDestRoleID=%d}\n", pDestPlayer->GetRoleID());
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return E_HS_PLAYRENOTINROOM;
	}

	//测试是否能添加
	nRet = PreAddRank(pMsgHead, pAddTitleReq, pSrcPlayer, pRoom, nOptionLen, pOptionData);
	//	PreAddRank已经响应了客户端，不需要重复处理
	//	if(0 > nRet)
	//	{
	//		WRITE_ERROR_LOG("add title player %d add %d title failed\n", pMsgHead->nRoleID, pAddTitleReq->nDestRoleID);
	//		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
	//		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
	//				enmTransType_P2P, nOptionLen, (char*)pOptionData);
	//		return nRet;
	//	}
	return nRet;
}

//处理会话中的添加权限
int32_t CAddTitleEvent::OnSessionAddTitle(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void* pOptionData)
{
	int32_t nRet = S_OK;
	CAddTitleResp stAddTitleResp;
	//获取会话消息体
	CHallGetRoomAdminCountResp* pGetAdminInfo = dynamic_cast<CHallGetRoomAdminCountResp*>(pMsgBody);

	//获取会话数据
	CSetTitleSessionData* pSessionData = (CSetTitleSessionData*)(pSession->GetSessionData());
	if(pSessionData == NULL || pGetAdminInfo == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pSessionData=0x%08x, pGetAdminInfo=0x%08x}\n", pSessionData, pGetAdminInfo);
		return E_NULLPOINTER;
	}
	if(0 > pGetAdminInfo->nResult)
	{
		WRITE_ERROR_LOG("get room admin count session failed!{nResult=0x%08x}\n", pGetAdminInfo->nResult);
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, &pSessionData->m_sMsgHead, &stAddTitleResp,
				enmTransType_P2P, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);
		return pGetAdminInfo->nResult;
	}
	CPlayer* pDestPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(pSessionData->m_pMsgBody.nDestRoleID, pDestPlayer);
	//未获取到玩家
	if(0 > nRet || pDestPlayer == NULL)
	{
		WRITE_ERROR_LOG("get room admin count session, failed to find role object!{nRoleID=%d}\n", pSessionData->m_pMsgBody.nDestRoleID);
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, &pSessionData->m_sMsgHead, &stAddTitleResp,
				enmTransType_P2P, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);
		return nRet;
	}
	//	判断是否有资格任命
	//	nRet = pDestPlayer->IsCanAddTitle(pSessionData->m_pRoom->GetRoomID(), pSessionData->m_pMsgBody.nAddRoleRank);
	//	if(S_OK != nRet)
	//	{
	//		if((int32_t)E_HS_ISOTHERROOMADMIN == nRet)
	//		{
	//			WRITE_ERROR_LOG("destplayer %d is other room admin\n", pDestPlayer->GetRoleID());
	//			stAddTitleResp.nChangeResult = enmChangeResult_Is_AdminInOther;
	//			SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, &pSessionData->m_sMsgHead, &stAddTitleResp,
	//					enmTransType_P2P, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);
	//			return E_HS_ISOTHERROOMADMIN;
	//		}
	//		WRITE_ERROR_LOG("player %d has no permission\n", pDestPlayer->GetRoleID());
	//		stAddTitleResp.nChangeResult = enmChangeResult_NO_PERMISSION;
	//		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, &pSessionData->m_sMsgHead, &stAddTitleResp,
	//				enmTransType_P2P, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);
	//		return E_HS_NOPERMISSIONS;
	//	}
	//判断管理员是否已满
	uint16_t nMaxAdminCount = 0;
	//房间显示容量
	uint16_t nMaxPlayercount = pGetAdminInfo->nRoomShowCapacity;
	switch(pSessionData->m_pMsgBody.nAddRoleRank)
	{
		case enmRoleRank_MinorHost:
		nMaxAdminCount = g_AboutRoomConfig.GetRoomTypeInfo(nMaxPlayercount).nMaxSecHostCount;
		break;
		case enmRoleRank_Admin:
		nMaxAdminCount = g_AboutRoomConfig.GetRoomTypeInfo(nMaxPlayercount).nMaxAdmCount;
		break;
		case enmRoleRank_TempAdmin:
		nMaxAdminCount = g_AboutRoomConfig.GetRoomTypeInfo(nMaxPlayercount).nMaxTempAdmCount;
		break;
		default:
		WRITE_ERROR_LOG("get room admin count session, get nAddRoleRank error!{nAddRoleRank=%d}\n", pSessionData->m_pMsgBody.nAddRoleRank);
		break;
	}
	//处理临管，临时管理员不需要存数据库
	if(pSessionData->m_pMsgBody.nAddRoleRank == enmRoleRank_TempAdmin)
	{
		CRoom *pRoom = NULL;
		nRet = g_RoomMgt.GetRoom(pSessionData->m_sMsgHead.nRoomID, pRoom);
		if(0 > nRet || pRoom == NULL)
		{
			WRITE_ERROR_LOG("get room admin count session, get room object error!{nRoomID=%d, nRet=0x%08x}\n", pSessionData->m_sMsgHead.nRoomID, nRet);
			stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
			SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, &pSessionData->m_sMsgHead, &stAddTitleResp,
					enmTransType_P2P, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);
			return nRet;
		}
		if(!pRoom->IsPlayerInRoom(pSessionData->m_pMsgBody.nDestRoleID))
		{
			WRITE_DEBUG_LOG("add title failed ,player already exist room!{RoomID = %d ,RoleID = %d}",
					pSessionData->m_sMsgHead.nRoomID ,pSessionData->m_pMsgBody.nDestRoleID );
			return S_OK;
		}
		//数据库中没有房间临管数量
		int32_t nCurTempAdminCount = pRoom->GetTempAdminCount();
		WRITE_DEBUG_LOG("addtitle:add temp admin!{nCurTempAdminCount=%d,nCurUserCount=%d}\n", nCurTempAdminCount, pRoom->GetCurUserCount());
		if(nCurTempAdminCount >= nMaxAdminCount)
		{
			WRITE_DEBUG_LOG("get room admin count session, temp admin count over max!{nTempAdminCount=%d, nMaxAdminCount=%d}\n",
					pRoom->GetTempAdminCount(), nMaxAdminCount);
			stAddTitleResp.nChangeResult = enmChangeResult_Is_Full;
			SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, &pSessionData->m_sMsgHead, &stAddTitleResp,
					enmTransType_P2P, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);
			return E_HS_SECHOSTISFULL;
		}
		else
		{
			nRet = AddRoleRank(&pSessionData->m_sMsgHead, &pSessionData->m_pMsgBody, pSessionData->m_pMsgBody.nDestRoleID,
					pSessionData->m_pMsgBody.nAddRoleRank, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);
			if(nRet == S_OK)
			{
				pRoom->SetTempAdminCount(++nCurTempAdminCount);
			}
			return nRet;
		}
	}
	//处理非临管
	if(pGetAdminInfo->nAdminCount >= nMaxAdminCount/*最大管理员数*/)
	{
		WRITE_DEBUG_LOG("get room admin count session, admin count over max!{nAdminCount=%d, nMaxAdminCount=%d}\n",
				pGetAdminInfo->nAdminCount, nMaxAdminCount);
		stAddTitleResp.nChangeResult = enmChangeResult_Is_Full;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, &pSessionData->m_sMsgHead, &stAddTitleResp,
				enmTransType_P2P, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);
		return E_HS_SECHOSTISFULL;
	}

	CHallSetTitleReq stTitleReq;
	stTitleReq.nDestRoleID = pSessionData->m_pMsgBody.nDestRoleID;
	stTitleReq.nRoleRank = pSessionData->m_pMsgBody.nAddRoleRank;
	stTitleReq.nRoomID = pSessionData->m_sMsgHead.nRoomID;
	stTitleReq.nSrcRoleID = pSessionData->m_sMsgHead.nRoleID;

	CFrameSession* pSession = NULL;
	nRet = g_Frame.CreateSession(MSGID_DBHS_SET_TITLE_RESP, enmSessionType_UpdateTitle,
			this, enmGetRoomAdminCountTimeoutPerios, pSession);
	if(0 > nRet || pSession == NULL)
	{
		WRITE_ERROR_LOG("create update title session failed!{nRoleID=%d}\n", pSessionData->m_pPlayer->GetRoleID());
		CAddTitleResp stAddTitleResp;
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, &pSessionData->m_sMsgHead, &stAddTitleResp,
				enmTransType_P2P, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);
		return nRet;
	}
	CUpdateRankSessionData* pUpdateData = new(pSession->GetSessionData())CUpdateRankSessionData();
	memcpy(&pUpdateData->m_sMsgHead, &pSessionData->m_sMsgHead, sizeof(pUpdateData->m_sMsgHead));
	if(pSessionData->m_nOptionLen > 0)
	{
		pUpdateData->m_nOptionLen = pSessionData->m_nOptionLen;
		memcpy(pUpdateData->m_arrOptionData, pSessionData->m_arrOptionData, pSessionData->m_nOptionLen);
	}
	pUpdateData->nSrcRoleID = pSessionData->m_sMsgHead.nRoleID;
	pUpdateData->nRoleRank = pSessionData->m_pMsgBody.nAddRoleRank;
	pUpdateData->nDestRoleID = pSessionData->m_pMsgBody.nDestRoleID;
	memcpy(&pUpdateData->m_pMsgBody, &pSessionData->m_pMsgBody, sizeof(pUpdateData->m_pMsgBody));
	SendMessageRequest(MSGID_HSDB_SET_TITLE_REQ, &stTitleReq, pSessionData->m_sMsgHead.nRoomID,
			enmTransType_ByKey, pSessionData->m_sMsgHead.nRoleID, enmEntityType_DBProxy,
			pSessionData->m_sMsgHead.nRoleID, pSession->GetSessionIndex(), 0, NULL, "send update request");
	return S_OK;
}

//判断玩家是否具有任命资格
int32_t CAddTitleEvent::PreAddRank(MessageHeadSS* pMsgHead, CAddTitleReq* pAddTitleReq, CPlayer* pSrcPlayer, CRoom* pRoom,
		const uint16_t nOptionLen, const void* pOptionData)
{
	int32_t nRet = S_OK;
	CAddTitleResp stAddTitleResp;
	if(pMsgHead == NULL || pAddTitleReq == NULL || pSrcPlayer == NULL || pRoom == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pAddTitleReq=0x%08x, pSrcPlayer=0x%08x, pRoom=0x%08x}\n",
				pMsgHead, pAddTitleReq, pSrcPlayer, pRoom);
		return E_NULLPOINTER;
	}
	//获取目标玩家
	CPlayer* pDestPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(pAddTitleReq->nDestRoleID, pDestPlayer);
	if(0 > nRet || pDestPlayer == NULL)
	{
		WRITE_ERROR_LOG("add title, failed to find desti role object!{nDestRoleID=%d}\n", pAddTitleReq->nDestRoleID);
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return nRet;
	}
	RoleRank nRoleRank = pSrcPlayer->GetAdminRoleRank(pMsgHead->nRoomID);
	bool isOfficePlayer = pSrcPlayer->IsOfficialPlayer();
	WRITE_DEBUG_LOG("add title, {nAddRoleRank=%d}\n", pAddTitleReq->nAddRoleRank);
	switch(pAddTitleReq->nAddRoleRank)
	{
		case enmRoleRank_TempAdmin:
		//判断请求者是否有权限,只有管理员以上可以任命
		if(nRoleRank < enmRoleRank_Admin&&!isOfficePlayer)
		{
			WRITE_DEBUG_LOG("source player has no permission to add temp admin!{nRoleID=%d, nRoleRank=%d}\n",
					pMsgHead->nRoleID, nRoleRank);
			stAddTitleResp.nChangeResult = enmChangeResult_NO_PERMISSION;
			SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
					enmTransType_P2P, nOptionLen, (char*)pOptionData);
			return E_HS_NOPERMISSIONS;
		}

		//目标玩家是否具有任命为临时管理员的资格,不考虑12小时时限的问题
		nRet = pDestPlayer->IsCanAddTitle(pMsgHead->nRoomID, enmRoleRank_TempAdmin);
		if(S_OK != nRet)
		{
			if((int32_t)E_HS_ISOTHERROOMADMIN == nRet)
			{
				WRITE_DEBUG_LOG("desti player is another room's admin!{nRoleID=%d}\n", pDestPlayer->GetRoleID());
				stAddTitleResp.nChangeResult = enmChangeResult_Is_AdminInOther;
				SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
						enmTransType_P2P, nOptionLen, (char*)pOptionData);
				return E_HS_ISOTHERROOMADMIN;
			}
			WRITE_DEBUG_LOG("desti player is temp admin or guest!{nRoleID=%d}\n", pDestPlayer->GetRoleID());
			stAddTitleResp.nChangeResult = enmChangeResult_NO_PERMISSION;
			SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
					enmTransType_P2P, nOptionLen, (char*)pOptionData);
			return E_HS_NOPERMISSIONS;
		}

		break;
		case enmRoleRank_Admin:
		//判断请求者是否有权限,只有室主和副室主可以任命
		if(nRoleRank < enmRoleRank_MinorHost&&!isOfficePlayer)
		{
			WRITE_DEBUG_LOG("source player has no permission to add admin!{nRoleID=%d, nRoleRank=%d}\n",
					pMsgHead->nRoleID, nRoleRank);
			stAddTitleResp.nChangeResult = enmChangeResult_NO_PERMISSION;
			SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
					enmTransType_P2P, nOptionLen, (char*)pOptionData);
			return E_HS_NOPERMISSIONS;
		}
		//目标玩家是否具有任命为管理员的资格
		nRet = pDestPlayer->IsCanAddTitle(pMsgHead->nRoomID, enmRoleRank_Admin);
		if(S_OK != nRet)
		{
			if((int32_t)E_HS_ISOTHERROOMADMIN == nRet)
			{
				WRITE_DEBUG_LOG("desti player is another room's admin!{nRoleID=%d}\n", pDestPlayer->GetRoleID());
				stAddTitleResp.nChangeResult = enmChangeResult_Is_AdminInOther;
				SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
						enmTransType_P2P, nOptionLen, (char*)pOptionData);
				return E_HS_ISOTHERROOMADMIN;
			}

			WRITE_DEBUG_LOG("desti player is temp admin or guest!{nRoleID=%d}\n", pDestPlayer->GetRoleID());
			stAddTitleResp.nChangeResult = enmChangeResult_NO_PERMISSION;
			SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
					enmTransType_P2P, nOptionLen, (char*)pOptionData);
			return E_HS_NOPERMISSIONS;
		}
		break;
		case enmRoleRank_MinorHost:
		//判断请求者是否为室主
		if(nRoleRank < enmRoleRank_Host&&!isOfficePlayer)
		{
			WRITE_DEBUG_LOG("source player is not host to add minor host!{nRoleID=%d, nRoleRank=%d}\n",
					pSrcPlayer->GetRoleID(), nRoleRank);
			stAddTitleResp.nChangeResult = enmChangeResult_NO_PERMISSION;
			SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
					enmTransType_P2P, nOptionLen, (char*)pOptionData);
			return E_HS_NOPERMISSIONS;
		}
		//判断是否有资格任命为副室主
		nRet = pDestPlayer->IsCanAddTitle(pMsgHead->nRoomID, enmRoleRank_MinorHost);
		if(S_OK != nRet)
		{
			if((int32_t)E_HS_ISOTHERROOMADMIN == nRet)
			{
				WRITE_DEBUG_LOG("desti player is another room's admin!{nRoleID=%d}\n", pDestPlayer->GetRoleID());
				stAddTitleResp.nChangeResult = enmChangeResult_Is_AdminInOther;
				SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
						enmTransType_P2P, nOptionLen, (char*)pOptionData);
				return E_HS_ISOTHERROOMADMIN;
			}

			WRITE_DEBUG_LOG("desti player is temp admin or guest!{nRoleID=%d}\n", pDestPlayer->GetRoleID());
			stAddTitleResp.nChangeResult = enmChangeResult_NO_PERMISSION;
			SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
					enmTransType_P2P, nOptionLen, (char*)pOptionData);
			return E_HS_NOPERMISSIONS;
		}
		break;
		default:
		WRITE_ERROR_LOG("add title:get add role rank error!{nAddRoleRank=%d}\n", pAddTitleReq->nAddRoleRank);
		break;
	}

	nRet = SendReqToDB(pMsgHead, pAddTitleReq, nOptionLen, pOptionData, pSrcPlayer, pRoom);
	//	if(0 > nRet)
	//	{
	//		WRITE_ERROR_LOG("send session to db failed nRet = %d!\n", nRet);
	//		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
	//		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
	//				enmTransType_P2P, nOptionLen, (char*)pOptionData);
	//		return nRet;
	//	}

	return nRet;
}

int32_t CAddTitleEvent::SendReqToDB(MessageHeadSS* pMsgHead, IMsgBody* pMsgBody, const uint16_t nOptionLen,
		const void* pOptionData, CPlayer* pDestPlayer, CRoom* pRoom)
{
	int32_t nRet = S_OK;
	CAddTitleResp stAddTitleResp;
	CAddTitleReq* pAddTitleReq = NULL;
	pAddTitleReq = dynamic_cast<CAddTitleReq*>(pMsgBody);
	if(pMsgHead == NULL || pAddTitleReq == NULL || pDestPlayer == NULL || pRoom == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pAddTitleReq=0x%08x, pDestPlayer=0x%08x, pRoom=0x%08x}\n",
				pMsgHead, pAddTitleReq, pDestPlayer, pRoom);
		return E_NULLPOINTER;
	}
	CFrameSession* pSession = NULL;
	nRet = g_Frame.CreateSession(MSGID_DBHS_GETROOM_ADMIN_COUNT_RESP, enmSessionType_GetAdminCount,
			this, enmGetRoomAdminCountTimeoutPerios, pSession);
	if(0 > nRet || pSession == NULL)
	{
		WRITE_ERROR_LOG("create get room admin count session failed!{nRoleID=%d}\n", pDestPlayer->GetRoleID());
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return nRet;
	}

	//保持会话信息
	CSetTitleSessionData* pSessionData = new(pSession->GetSessionData())CSetTitleSessionData();
	//保持消息头MessageHeadSS* pMsgHead
	memcpy(&pSessionData->m_sMsgHead, pMsgHead, sizeof(pSessionData->m_sMsgHead));
	if(nOptionLen > 0)
	{
		pSessionData->m_nOptionLen = nOptionLen;
		memcpy(pSessionData->m_arrOptionData, pOptionData, nOptionLen);
	}
	//请求的房间
	pSessionData->m_pRoom = pRoom;
	//请求的玩家对象
	pSessionData->m_pPlayer = pDestPlayer;
	//保持消息体CAddTitleReq
	memcpy(&pSessionData->m_pMsgBody, pAddTitleReq, sizeof(pSessionData->m_pMsgBody));
	//向数据库发请求获取房间管理员数量
	CHallGetRoomAdminCountReq stGetAdminCountReq;
	stGetAdminCountReq.uRoomID = pMsgHead->nRoomID;
	stGetAdminCountReq.usRoleRank = pAddTitleReq->nAddRoleRank;
	WRITE_DEBUG_LOG("send get room admin count request to db!{nRoleID=%d, nRoomID=%d}\n",
			pMsgHead->nRoleID, pMsgHead->nRoomID);
	SendMessageRequest(MSGID_HSDB_GETROOM_ADMIN_COUNT_REQ, &stGetAdminCountReq,
			pMsgHead->nRoomID, enmTransType_ByKey, pMsgHead->nRoleID, enmEntityType_DBProxy,
			pMsgHead->nRoleID, pSession->GetSessionIndex(), 0, NULL, "send get room admin count request");

	return S_OK;
}

//更新内存，发送通知给Client、RoomServer、SysinfoServer和ItemServer
int32_t CAddTitleEvent::AddRoleRank(MessageHeadSS* pMsgHead, CAddTitleReq* pAddTitleReq, RoleID nDestRoleID,
		RoleRank nRoleRank,const uint16_t nOptionLen, const char* pOptionData)
{
	int32_t nRet = S_OK;
	CAddTitleResp stAddTitleResp;
	//获取目标玩家
	CPlayer* pDestPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(nDestRoleID, pDestPlayer);
	if(0 > nRet || pDestPlayer == NULL)
	{
		WRITE_ERROR_LOG("add role rank, cannot find the desti role object!{nDestRoleID=%d}\n",
				pAddTitleReq->nDestRoleID);
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, pOptionData);
		return nRet;
	}
	CRoom* pRoom = NULL;
	nRet = g_RoomMgt.GetRoom(pMsgHead->nRoomID, pRoom);
	if(0 > nRet || pRoom == NULL)
	{
		WRITE_ERROR_LOG("add role rank, cannot find the room object!{nRoomID=%d, nRet=0x%08x}\n",
				pAddTitleReq->nDestRoleID, nRet);
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, pOptionData);
		return nRet;
	}
	RoleRank nPreRoleRank = enmRoleRank_None;
	//更新内存中的玩家权限
	nRet = pDestPlayer->SetAdminRoleRank(pMsgHead->nRoomID, pAddTitleReq->nAddRoleRank,nPreRoleRank);
	if(S_OK != nRet)
	{
		WRITE_ERROR_LOG("add role rank, failed to set admin role rank!{nRoomID=%d, nDestRoleID=%d, nAddRoleRank=%d, nRet=0x%08x}\n",
				pMsgHead->nRoomID, pAddTitleReq->nDestRoleID, pAddTitleReq->nAddRoleRank, nRet);
		stAddTitleResp.nChangeResult = enmChangeResult_UNKNOWN;
		SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
				enmTransType_P2P, nOptionLen, pOptionData);
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

	//给客户端发响应
	WRITE_DEBUG_LOG("send response to client:{nChangeResult=%d}\n", enmChangeResult_OK);
	stAddTitleResp.nChangeResult = enmChangeResult_OK;
	SendMessageResponse(MSGID_HSCL_ADD_TITLE_RESP, pMsgHead, &stAddTitleResp,
			enmTransType_P2P, nOptionLen, pOptionData);

	//给其他用户、RoomServer和ItemServer发通知
	CAddtitleNotify stAddTitleNotify;
	stAddTitleNotify.nAddRoleRank = pAddTitleReq->nAddRoleRank;
	stAddTitleNotify.nDestRoleID = pAddTitleReq->nDestRoleID;
	stAddTitleNotify.nSrcRoleID = pMsgHead->nRoleID;
	CModifyTitleNoti stModifyTitleNoti;
	stModifyTitleNoti.nRoleID = pAddTitleReq->nDestRoleID;
	stModifyTitleNoti.nRoomID = pRoom->GetRoomID();
	stModifyTitleNoti.nRoleRank = pAddTitleReq->nAddRoleRank;
	WRITE_DEBUG_LOG("send add title notify to server and client!{nChangeResult=%d}\n", enmChangeResult_OK);
//	SendMessageNotifyToServer(MSGID_HSTA_MODIFY_TITLE_NOTIFY, &stModifyTitleNoti, pMsgHead->nRoomID,
//			enmTransType_P2P, pAddTitleReq->nDestRoleID, enmEntityType_Room, pRoom->GetRoomServerID(),
//			nOptionLen, pOptionData, "send add title notify to room server");
	SendMessageNotifyToServer(MSGID_HSTA_MODIFY_TITLE_NOTIFY, &stModifyTitleNoti, pMsgHead->nRoomID,
				enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_Room, 0, nOptionLen,
				pOptionData, "send add title notify to room server");

//	WRITE_DEBUG_LOG("send add title notify to item server! {nChangeResult=%d}\n", enmChangeResult_OK);
	SendMessageNotifyToServer(MSGID_HSTA_MODIFY_TITLE_NOTIFY, &stModifyTitleNoti, pMsgHead->nRoomID,
			enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_Item, 0, nOptionLen,
			pOptionData, "send add title notify to item server");

//	WRITE_DEBUG_LOG("send add title notify to others in the same room!{nChangeResult=%d}\n", enmChangeResult_OK);
	SendMessageNotifyToClient(MSGID_HSCL_ADD_TITLE_NOTI, &stAddTitleNotify, pMsgHead->nRoomID,
			enmBroadcastType_ExceptPlayr, pMsgHead->nRoleID, 0, NULL, "notify other player in the room");
	return S_OK;
}
FRAME_HALLSERVER_NAMESPACE_END
