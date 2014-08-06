/*
 * bll_event_default.cpp
 *
 *  Created on: 2012-11-6
 *      Author: jimm
 */

#include "bll_event_default.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CDefaultMessageHandler::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody == NULL || pMsgHead == NULL || pSession == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x, pSession=0x%08x}\n", pMsgHead, pMsgBody, pSession);
		return E_NULLPOINTER;
	}

	switch(pSession->GetSessionType())
	{
	default:
		break;
	}

	return S_OK;
}

int32_t CDefaultMessageHandler::OnSessionTimeOut(CFrameSession *pSession)
{
	if(pSession == NULL)
	{
		WRITE_ERROR_LOG("null pointer:pSession!\n");
		return E_NULLPOINTER;
	}

	switch(pSession->GetSessionType())
	{
	default:
		break;
	}

	return S_OK;
}

int32_t CDefaultMessageHandler::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	WRITE_DEBUG_LOG("default message handler!\n");

//	CCreateRoomNotice *pCreateRoomNotice = dynamic_cast<CCreateRoomNotice *>(pMsgBody);
//	if(NULL == pCreateRoomNotice)
//	{
//		WRITE_ERROR_LOG("null pointer:pMsgBody transform child failed!\n");
//		return E_NULLPOINTER;
//	}

	CTestSession stSession;

	stSession.SetSessionData(111, 123, 0, this);

	g_Frame.SendMessageRequest(0x110401, pMsgBody, &stSession);

	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
