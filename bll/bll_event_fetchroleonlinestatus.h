/*
 * bll_event_fetchroleonlinestatus.h
 *
 *  Created on: 2012-8-10
 *      Author: zhurw
 */

#ifndef BLL_EVENT_FETCHROLEONLINESTATUS_H_
#define BLL_EVENT_FETCHROLEONLINESTATUS_H_


#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

//从webagent来的获取用户是否在线的请求
class CFetchRoleOnlineStatusEvent : public CBllBase
{
public:
	CFetchRoleOnlineStatusEvent()
	{
	}

	virtual ~CFetchRoleOnlineStatusEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:

};

#define	CREATE_FETCHROLEONLINESTATUSEVENT_INSTANCE	CSingleton<CFetchRoleOnlineStatusEvent>::CreateInstance
#define	GET_FETCHROLEONLINESTATUSEVENT_INSTANCE		CSingleton<CFetchRoleOnlineStatusEvent>::GetInstance
#define	DESTROY_FETCHROLEONLINESTATUSEVENT_INSTANCE	CSingleton<CFetchRoleOnlineStatusEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_FETCHROLEONLINESTATUS_H_ */
