/*
 * bll_event_updateidentifynotice.h
 *
 *  Created on: 2012-9-19
 *      Author: liufl
 */

#ifndef BLL_EVENT_UPDATEIDENTIFYNOTICE_H_
#define BLL_EVENT_UPDATEIDENTIFYNOTICE_H_
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
#include "itemserver_message_define.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN
class CUpdateIdentityNotiEvent : public CBllBase
{
public:
	CUpdateIdentityNotiEvent()
	{
	}
	virtual ~CUpdateIdentityNotiEvent()
	{
	}
public:
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);



};

#define CREATE_UPDATEIDENTITYNOTIEVENT_INSTANCE	CSingleton<CUpdateIdentityNotiEvent>::CreateInstance
#define GET_UPDATEIDENTITYNOTIEVENT_INSTANCE		CSingleton<CUpdateIdentityNotiEvent>::GetInstance
#define DESTROY_UPDATEIDENTITYNOTIEVENT_INSTANCE	CSingleton<CUpdateIdentityNotiEvent>::DestroyInstance


FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_UPDATEIDENTIFYNOTICE_H_ */
