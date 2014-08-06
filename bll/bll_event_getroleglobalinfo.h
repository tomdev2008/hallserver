/*
 * bll_event_getroleglobalinfo.h
 *
 *  Created on: 2012-2-15
 *      Author: jimm
 */

#ifndef BLL_EVENT_GETROLEGLOBALINFO_H_
#define BLL_EVENT_GETROLEGLOBALINFO_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "dal/from_server_message.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

class CGetRoleGlobalInfoMessageEvent : public CBllBase
{
public:
	CGetRoleGlobalInfoMessageEvent()
	{
	}

	virtual ~CGetRoleGlobalInfoMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

};

#define	CREATE_GETROLEGLOBALINFOEVENT_INSTANCE	CSingleton<CGetRoleGlobalInfoMessageEvent>::CreateInstance
#define	GET_GETROLEGLOBALINFOEVENT_INSTANCE		CSingleton<CGetRoleGlobalInfoMessageEvent>::GetInstance
#define	DESTROY_GETROLEGLOBALINFOEVENT_INSTANCE	CSingleton<CGetRoleGlobalInfoMessageEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_GETROLEGLOBALINFO_H_ */
