/*
 * bll_event_builddatanum.h
 *
 *  Created on: 2012-5-24
 *      Author: fuz
 */

#ifndef BLL_EVENT_BUILDDATANUM_H_
#define BLL_EVENT_BUILDDATANUM_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "client_message_define.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

//通知需要恢复的数据个数，包括房间数据和玩家数据
class CBuildDataNumMessageEvent : public CBllBase
{
public:
	CBuildDataNumMessageEvent()
	{
	}

	virtual ~CBuildDataNumMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

};

#define	CREATE_BUILDDATANUMEVENT_INSTANCE	CSingleton<CBuildDataNumMessageEvent>::CreateInstance
#define	GET_BUILDDATANUMEVENT_INSTANCE		CSingleton<CBuildDataNumMessageEvent>::GetInstance
#define	DESTROY_BUILDDATANUMEVENT_INSTANCE	CSingleton<CBuildDataNumMessageEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END
#endif /* BLL_EVENT_BUILDDATANUM_H_ */
