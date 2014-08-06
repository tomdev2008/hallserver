/*
 *  bll_event_onmic_notice.h
 *
 *  To do：
 *  Created on: 	2012-5-8
 *  Author: 		luocj
 */

#ifndef BLL_EVENT_ONMIC_NOTICE_H_
#define BLL_EVENT_ONMIC_NOTICE_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

class COnMicNoticeMessageEvent : public CBllBase
{
public:
	COnMicNoticeMessageEvent()
	{
	}

	virtual ~COnMicNoticeMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:

};

#define	CREATE_ONMICNOTICEEVENT_INSTANCE	CSingleton<COnMicNoticeMessageEvent>::CreateInstance
#define	GET_ONMICNOTICEEVENT_INSTANCE		CSingleton<COnMicNoticeMessageEvent>::GetInstance
#define	DESTROY_ONMICNOTICEEVENT_INSTANCE	CSingleton<COnMicNoticeMessageEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_ONMIC_NOTICE_H_ */
