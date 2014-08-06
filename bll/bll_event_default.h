/*
 * bll_event_default.h
 *
 *  Created on: 2012-11-6
 *      Author: jimm
 */

#ifndef BLL_EVENT_DEFAULT_H_
#define BLL_EVENT_DEFAULT_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

class CDefaultMessageHandler : public IMessageEventSS, public ISessionEvent
{
public:
	CDefaultMessageHandler()
	{
	}

	virtual ~CDefaultMessageHandler()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	//会话事件激活
	virtual int32_t OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			CFrameSession* pSession, const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	int32_t OnSessionTimeOut(CFrameSession *pSession);

private:

	SESSION_DATA_BEGIN(CTestSession)
	SESSION_DATA_MEMBER(int32_t, m_nTestInt)
	SESSION_DATA_END()
};

#define	CREATE_DEFAULTMESSAGEHANDLER_INSTANCE	CSingleton<CDefaultMessageHandler>::CreateInstance
#define	g_DefaultMessageHandler					CSingleton<CDefaultMessageHandler>::GetInstance()
#define	DESTROY_DEFAULTMESSAGEHANDLER_INSTANCE	CSingleton<CDefaultMessageHandler>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_DEFAULT_H_ */
