/*
 * bll_event_statuschangenoti_from_web.h
 *
 *  Created on: 2012-8-28
 *      Author: zhurw
 */

#ifndef BLL_EVENT_STATUSCHANGENOTI_FROM_WEB_H_
#define BLL_EVENT_STATUSCHANGENOTI_FROM_WEB_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"
#include "dal/from_server_message.h"
#include "ctl/server_datacenter.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

class CStatusChangeNotiFromWeb : public CBllBase
{
public:
	CStatusChangeNotiFromWeb()
	{
	}
	virtual ~CStatusChangeNotiFromWeb()
	{
	}
public:
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

};

#define CREATE_STATUSCHANGENOTIEVENT_INSTANCE	CSingleton<CStatusChangeNotiFromWeb>::CreateInstance
#define GET_STATUSCHANGENOTIEVENT_INSTANCE		CSingleton<CStatusChangeNotiFromWeb>::GetInstance
#define DESTROY_STATUSCHANGENOTIEVENT_INSTANCE	CSingleton<CStatusChangeNotiFromWeb>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_STATUSCHANGENOTI_FROM_WEB_H_ */
