/*
 * bll_event_createroomnotice.h
 *
 *  Created on: 2012-2-12
 *      Author: jimm
 */

#ifndef BLL_EVENT_CREATEROOMNOTICE_H_
#define BLL_EVENT_CREATEROOMNOTICE_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

class CCreateRoomNoticeMessageEvent : public CBllBase
{
public:
	CCreateRoomNoticeMessageEvent()
	{
	}

	virtual ~CCreateRoomNoticeMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:

};

#define	CREATE_CREATEROOMNOTICEEVENT_INSTANCE	CSingleton<CCreateRoomNoticeMessageEvent>::CreateInstance
#define	GET_CREATEROOMNOTICEEVENT_INSTANCE		CSingleton<CCreateRoomNoticeMessageEvent>::GetInstance
#define	DESTROY_CREATEROOMNOTICEEVENT_INSTANCE	CSingleton<CCreateRoomNoticeMessageEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_CREATEROOMNOTICE_H_ */
