/*
 * bll_event_enterroomnotice.h
 *
 *  Created on: 2012-2-15
 *      Author: jimm
 */

#ifndef BLL_EVENT_ENTERROOMNOTICE_H_
#define BLL_EVENT_ENTERROOMNOTICE_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

class CEnterRoomNoticeMessageEvent : public CBllBase
{
public:
	CEnterRoomNoticeMessageEvent()
	{
	}

	virtual ~CEnterRoomNoticeMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:

};

#define	CREATE_ENTERROOMNOTICEEVENT_INSTANCE	CSingleton<CEnterRoomNoticeMessageEvent>::CreateInstance
#define	GET_ENTERROOMNOTICEEVENT_INSTANCE		CSingleton<CEnterRoomNoticeMessageEvent>::GetInstance
#define	DESTROY_ENTERROOMNOTICEEVENT_INSTANCE	CSingleton<CEnterRoomNoticeMessageEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_ENTERROOMNOTICE_H_ */
