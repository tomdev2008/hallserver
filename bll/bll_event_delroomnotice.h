/*
 * bll_event_delroomnotice.h
 *
 *  Created on: 2012-7-24
 *      Author: zhurw
 */

#ifndef BLL_EVENT_DELROOMNOTICE_H_
#define BLL_EVENT_DELROOMNOTICE_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

class CDelRoomNoticeEvent : public CBllBase
{
public:
	CDelRoomNoticeEvent()
	{
	}

	virtual ~CDelRoomNoticeEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:

};

#define	CREATE_DELROOMNOTICEEVENT_INSTANCE	CSingleton<CDelRoomNoticeEvent>::CreateInstance
#define	GET_DELROOMNOTICEEVENT_INSTANCE		CSingleton<CDelRoomNoticeEvent>::GetInstance
#define	DESTROY_DELROOMNOTICEEVENT_INSTANCE	CSingleton<CDelRoomNoticeEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_DELROOMNOTICE_H_ */
