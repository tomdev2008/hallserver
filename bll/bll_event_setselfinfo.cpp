/*
 * bll_event_setselfinfo.cpp
 *
 *  Created on: 2012-3-21
 *      Author: zhurw
 */

#include "bll_event_setselfinfo.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CSetSelfInfoEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgHead == NULL || pMsgBody == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	switch(pMsgHead->nMessageID)
	{
		case MSGID_CLHS_SETSELFINFO_REQ:
			OnSetSelfInfoEvent(pMsgHead, pMsgBody, nOptionLen, pOptionData);
			break;
		default:
			WRITE_ERROR_LOG("set self info:get wrong message!{nMessageID=0x%08x}\n",pMsgHead->nMessageID);
			break;
	}
	return S_OK;
}

int32_t CSetSelfInfoEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody, CFrameSession* pSession,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgHead == NULL || pMsgBody == NULL || pSession == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x, pSession=0x%08x}\n",
				pMsgHead, pMsgBody, pSession);
		return E_NULLPOINTER;
	}

	switch(pSession->GetSessionType())
	{
		case enmSessionType_SetSelfInfo:
		{
			CSetSelfInfoResp stSetSelfInfoResp;
			CDBSetSelfSelfInfoResp* pDBSetSelfInfoResp = dynamic_cast<CDBSetSelfSelfInfoResp*>(pMsgBody);
			if(NULL == pDBSetSelfInfoResp)
			{
				WRITE_ERROR_LOG("null pointer: pMsgBody transform to CDBSetSelfSelfInfoResp class failed!{nSessionType=0x%04X, nSessionIndex=%d}\n",
						enmSessionType_SetSelfInfo, pSession->GetSessionIndex());
				return E_NULLPOINTER;
			}

			WRITE_DEBUG_LOG("receive set self info response session!{nResult=0x%08x}\n", pDBSetSelfInfoResp->nResult);
			CSetSelfInfoSessionData* pSessionData = (CSetSelfInfoSessionData*)(pSession->GetSessionData());
			//修改数据库失败
			if(0 > pDBSetSelfInfoResp->nResult)
			{
				WRITE_ERROR_LOG("set self info to db failed!{nResult=0x%08x}\n", pDBSetSelfInfoResp->nResult);
				stSetSelfInfoResp.nSetSelfInfoResult = (uint32_t)enmSetInfo_UNKNOWN;
//				WRITE_DEBUG_LOG("send set self info response to client!{nResult=0x%08x}\n", enmSetInfo_UNKNOWN);
				SendMessageResponse(MSGID_HSCL_SETSELFINFO_RESP, &pSessionData->m_sMsgHead, &stSetSelfInfoResp,
						enmTransType_P2P, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);
				return S_OK;
			}
			//修改数据库成功
			else if(S_OK == pDBSetSelfInfoResp->nResult)
			{
				//发送响应给客户端，userinfosetting字段在玩家登陆时返回给玩家
				stSetSelfInfoResp.nSetSelfInfoResult = (uint32_t)enmSetInfo_OK;
				WRITE_INFO_LOG("send set self info response to client!{nResult=0x%04x}\n", enmSetInfo_OK);
				SendMessageResponse(MSGID_HSCL_SETSELFINFO_RESP, &pSessionData->m_sMsgHead, &stSetSelfInfoResp,
						enmTransType_P2P, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);

				//通知房间玩家的selfsetting修改
				CHallSetSelfSettingNoti stCHallSetSelfSettingNoti;
				stCHallSetSelfSettingNoti.nRoleID = pSessionData->m_sMsgHead.nRoleID;
				stCHallSetSelfSettingNoti.nPlayerSelfSetting = pSessionData->m_pMsgBody.m_nUserInfoSelfSet;
				SendMessageNotifyToServer(MSGID_HSRS_SETSELFSETTING_NOTI, &stCHallSetSelfSettingNoti, enmInvalidRoomID,
								enmTransType_Broadcast, pSessionData->m_sMsgHead.nRoleID, enmEntityType_Room);
				return S_OK;
			}
		}
		break;
		default:
			WRITE_ERROR_LOG("set self info session:get wrong session!{nSessionType=0x%04X, nSessionIndex=%d}\n",
					pSession->GetSessionType(), pSession->GetSessionIndex());
			break;
	}
	return S_OK;
}

int32_t CSetSelfInfoEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	if(pSession == NULL)
	{
		WRITE_ERROR_LOG("set self info to db session timeout, and pSession is null!{ret=0x%08x}\n", E_NULLPOINTER);
		return E_NULLPOINTER;
	}
	WRITE_ERROR_LOG("set self info to db session time out!{nSessionType=0x%04X, nSessionIndex=%d}",
			pSession->GetSessionType(), pSession->GetSessionIndex());

	CSetSelfInfoSessionData* pSessionData = (CSetSelfInfoSessionData*)(pSession->GetSessionData());
	CSetSelfInfoResp stSetSelfInfoResp;
	stSetSelfInfoResp.nSetSelfInfoResult = (uint32_t)enmSetInfo_UNKNOWN;
//	WRITE_DEBUG_LOG("send set self info response to client!{result=%d}\n", enmSetInfo_UNKNOWN);
	SendMessageResponse(MSGID_HSCL_SETSELFINFO_RESP, &pSessionData->m_sMsgHead, &stSetSelfInfoResp,
			enmTransType_P2P, pSessionData->m_nOptionLen, pSessionData->m_arrOptionData);
	return S_OK;
}

int32_t CSetSelfInfoEvent::OnSetSelfInfoEvent(MessageHeadSS* pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void* pOptionData)
{
	int32_t nRet = S_OK;
	if(pMsgHead == NULL || pMsgBody == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	CSetSelfInfoReq* pSetSelfInfoReq = dynamic_cast<CSetSelfInfoReq*>(pMsgBody);
	if(pSetSelfInfoReq == NULL)
	{
		WRITE_ERROR_LOG("null pointer: pMsgBody transform to CSetSelfInfoReq class failed!{nMessageID=0x%08x, nRoleID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}
	WRITE_INFO_LOG("receive set self info request!{nSetInfoCount=%d, nUserInfoSelfSet=%u}\n",
			pSetSelfInfoReq->m_nSetInfoCount, pSetSelfInfoReq->m_nUserInfoSelfSet);

	CHallSetSelfInfoReq stDBSetSelfInfoReq;
	for(uint8_t i = 0; i < pSetSelfInfoReq->m_nSetInfoCount; ++i)
	{
		//处理不同类型的设置，0x81为无效类型
		if(pSetSelfInfoReq->m_arrUserInfoType[i] != (UserInfoType)enmUserInfoType_Invalid)
		{
			switch(pSetSelfInfoReq->m_arrUserInfoType[i])
			{
				//处理userinfosetting
				case enmUserInfoType_Self_Setting:
				stDBSetSelfInfoReq.nRoleID = pMsgHead->nRoleID;
				stDBSetSelfInfoReq.nUserInfoType = enmUserInfoType_Self_Setting;
				stDBSetSelfInfoReq.nUserInfoSelfSet = pSetSelfInfoReq->m_nUserInfoSelfSet;
				stDBSetSelfInfoReq.nSetInfoCount++;
				break;
				default:
					WRITE_ERROR_LOG("set self info, invalid user info type!{nUserInfoType=%d}", pSetSelfInfoReq->m_arrUserInfoType[i]);
				break;
			}
		}
	}

	//没有可以设置的字段
	if(stDBSetSelfInfoReq.nSetInfoCount == 0)
	{
		CSetSelfInfoResp stSetSelfInfoResp;
		stSetSelfInfoResp.nSetSelfInfoResult = (uint32_t)enmSetInfo_NOT_ALLOWED;
		WRITE_DEBUG_LOG("set self info response to client!{nSetSelfInfoResult=%d}\n", enmSetInfo_NOT_ALLOWED);
		SendMessageResponse(MSGID_HSCL_SETSELFINFO_RESP, pMsgHead, &stSetSelfInfoResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return S_OK;
	}
	CFrameSession* pSession = NULL;
	nRet = g_Frame.CreateSession(MSGID_DBHS_SETSELFINFO_RESP, enmSessionType_SetSelfInfo, this, enmSetSelfInfoTimeoutPerios, pSession);

	if(0 > nRet || pSession == NULL)
	{
		WRITE_ERROR_LOG("create set self info session failed!{nRet=0x%08x}\n", nRet);
		CSetSelfInfoResp stSetSelfInfoResp;
		stSetSelfInfoResp.nSetSelfInfoResult = (uint32_t)enmSetInfo_UNKNOWN;
		WRITE_DEBUG_LOG("set self info response to client!{nSetSelfInfoResult=%d}\n", enmSetInfo_UNKNOWN);
		SendMessageResponse(MSGID_HSCL_SETSELFINFO_RESP, pMsgHead, &stSetSelfInfoResp,
				enmTransType_P2P, nOptionLen, (char*)pOptionData);
		return S_OK;
	}
	CSetSelfInfoSessionData* pSessionData = new(pSession->GetSessionData())CSetSelfInfoSessionData();
	memcpy(&pSessionData->m_sMsgHead, pMsgHead, sizeof(pSessionData->m_sMsgHead));
	if(nOptionLen > 0)
	{
		pSessionData->m_nOptionLen = nOptionLen;
		memcpy(pSessionData->m_arrOptionData, pOptionData, nOptionLen);
	}
	memcpy(&pSessionData->m_pMsgBody, pSetSelfInfoReq, sizeof(pSessionData->m_pMsgBody));
	WRITE_DEBUG_LOG("send set self info request to db:{nRoleID=%d}\n", pMsgHead->nRoleID);
	//发送请求给数据库修改自身设置
	SendMessageRequest(MSGID_HSDB_SETSELFINFO_REQ, &stDBSetSelfInfoReq,
			pMsgHead->nRoomID, enmTransType_ByKey, pMsgHead->nRoleID, enmEntityType_DBProxy,
			pMsgHead->nRoleID, pSession->GetSessionIndex(), 0, NULL, "send set self info to db");

	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
