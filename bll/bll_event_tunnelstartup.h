/*
 * bll_event_tunnelstartup.h
 *
 *  Created on: 2012-5-25
 *      Author: fuz
 */

#ifndef BLL_EVENT_TUNNELSTARTUP_H_
#define BLL_EVENT_TUNNELSTARTUP_H_

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

class CTunnelStartMessageEvent : public CBllBase
{
public:
	CTunnelStartMessageEvent()
	{
	}

	virtual ~CTunnelStartMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

};

#define	CREATE_TUNNELSTARTUPEVENT_INSTANCE	CSingleton<CTunnelStartMessageEvent>::CreateInstance
#define	GET_TUNNELSTARTUPEVENT_INSTANCE		CSingleton<CTunnelStartMessageEvent>::GetInstance
#define	DESTROY_TUNNELSTARTUPEVENT_INSTANCE	CSingleton<CTunnelStartMessageEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END
#endif /* BLL_EVENT_TUNNELSTARTUP_H_ */
