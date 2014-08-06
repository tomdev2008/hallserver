/*
 * bll_event_lauchroomnotice.h
 *
 *  Created on: 2012-8-6
 *      Author: zhurw
 */

#ifndef BLL_EVENT_LAUCHROOMNOTICE_H_
#define BLL_EVENT_LAUCHROOMNOTICE_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

class CLauchRoomNoticeEvent : public CBllBase
{
public:
	CLauchRoomNoticeEvent()
	{
	}

	virtual ~CLauchRoomNoticeEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:

};

#define	CREATE_LAUCHROOMNOTICEEVENT_INSTANCE	CSingleton<CLauchRoomNoticeEvent>::CreateInstance
#define	GET_LAUCHROOMNOTICEEVENT_INSTANCE		CSingleton<CLauchRoomNoticeEvent>::GetInstance
#define	DESTROY_LAUCHROOMNOTICEEVENT_INSTANCE	CSingleton<CLauchRoomNoticeEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_LAUCHROOMNOTICE_H_ */
