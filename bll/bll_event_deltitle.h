/*
 * bll_event_deltitle.h
 *
 *  Created on: 2012-3-7
 *      Author: zhurw
 */

#ifndef BLL_EVENT_DELTITLE_H_
#define BLL_EVENT_DELTITLE_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "main_frame.h"
#include "bll_base.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "client_message_define.h"
#include "dal/to_server_message.h"
#include "ctl/server_datacenter.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

class CDelTitleEvent: public CBllBase
{
public:
	CDelTitleEvent()
	{
	}
	virtual ~CDelTitleEvent()
	{
	}
public:
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
				const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:
	int32_t OnDelTitleEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
					const uint16_t nOptionLen = 0, const void *pOptionData = NULL);
	int32_t DelTempAdmin(MessageHeadSS * pMsgHead, CDelTitleReq* pDelTitleReq,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);
	int32_t SendRespAndNotify(MessageHeadSS * pMsgHead, CDelTitleReq* pDelTitleReq,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

};

#define CREATE_DELTITLEEVENT_INSTANCE	CSingleton<CDelTitleEvent>::CreateInstance
#define GET_DELTITLEEVENT_INSTANCE		CSingleton<CDelTitleEvent>::GetInstance
#define DESTORY_DELTITLEEVENT_INSTANCE	CSingleton<CDelTitleEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_DELTITLE_H_ */
