/*
 * bll_event_regandfillindatanotice.h
 *
 *  Created on: 2012-5-25
 *      Author: jimm
 */

#ifndef BLL_EVENT_REGANDFILLINDATANOTICE_H_
#define BLL_EVENT_REGANDFILLINDATANOTICE_H_

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
class CRegAndFillInDataNotiEvent : public CBllBase
{
public:
	CRegAndFillInDataNotiEvent()
	{
	}
	virtual ~CRegAndFillInDataNotiEvent()
	{
	}
public:
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

};

#define CREATE_REGANDFILLINDATANOTIEVENT_INSTANCE	CSingleton<CRegAndFillInDataNotiEvent>::CreateInstance
#define GET_REGANDFILLINDATANOTIEVENT_INSTANCE		CSingleton<CRegAndFillInDataNotiEvent>::GetInstance
#define DESTROY_REGANDFILLINDATANOTIEVENT_INSTANCE	CSingleton<CRegAndFillInDataNotiEvent>::DestroyInstance


FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_REGANDFILLINDATANOTICE_H_ */
