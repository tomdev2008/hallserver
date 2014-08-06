/*
 *  bll_event_offmic_notice.h
 *
 *  To do：
 *  Created on: 	2012-5-8
 *  Author: 		luocj
 */

#ifndef BLL_EVENT_OFFMIC_NOTICE_H_
#define BLL_EVENT_OFFMIC_NOTICE_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

class COffMicNoticeMessageEvent : public CBllBase
{
public:
	COffMicNoticeMessageEvent()
	{
	}

	virtual ~COffMicNoticeMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:

};

#define	CREATE_OFFMICNOTICEEVENT_INSTANCE	CSingleton<COffMicNoticeMessageEvent>::CreateInstance
#define	GET_OFFMICNOTICEEVENT_INSTANCE		CSingleton<COffMicNoticeMessageEvent>::GetInstance
#define	DESTROY_OFFMICNOTICEEVENT_INSTANCE	CSingleton<COffMicNoticeMessageEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END
#endif /* BLL_EVENT_OFFMIC_NOTICE_H_ */
