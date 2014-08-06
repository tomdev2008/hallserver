/*
 * bll_event_buildroomdata.h
 *
 *  Created on: 2012-5-17
 *      Author: fuz
 */

#ifndef BLL_EVENT_BUILDROOMDATA_H_
#define BLL_EVENT_BUILDROOMDATA_H_

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

//恢复房间数据，先恢复房间数据，然后恢复玩家数据
class CBuildRoomDataMessageEvent : public CBllBase
{
public:
	CBuildRoomDataMessageEvent()
	{
	}

	virtual ~CBuildRoomDataMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

};

#define	CREATE_BUILDROOMEVENT_INSTANCE	CSingleton<CBuildRoomDataMessageEvent>::CreateInstance
#define	GET_BUILDROOMEVENT_INSTANCE		CSingleton<CBuildRoomDataMessageEvent>::GetInstance
#define	DESTROY_BUILDROOMEVENT_INSTANCE	CSingleton<CBuildRoomDataMessageEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END
#endif /* BLL_EVENT_BUILDROOMDATA_H_ */
