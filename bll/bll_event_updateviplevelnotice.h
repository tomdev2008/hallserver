/*
 * bll_event_updateviplevelnotice.h
 *
 *  Created on: 2012-3-15
 *      Author: zhurw
 */

#ifndef BLL_EVENT_UPDATEVIPLEVELNOTICE_H_
#define BLL_EVENT_UPDATEVIPLEVELNOTICE_H_

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
class CUpdateVipNotiEvent : public CBllBase
{
public:
	CUpdateVipNotiEvent()
	{
	}
	virtual ~CUpdateVipNotiEvent()
	{
	}
public:
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:
	int32_t UpdateMemVipLevel(RoleID nRoleID, VipLevel nVipLevel);

};

#define CREATE_UPDATEVIPNOTIEVENT_INSTANCE	CSingleton<CUpdateVipNotiEvent>::CreateInstance
#define GET_UPDATEVIPNOTIEVENT_INSTANCE		CSingleton<CUpdateVipNotiEvent>::GetInstance
#define DESTROY_UPDATEVIPNOTIEVENT_INSTANCE	CSingleton<CUpdateVipNotiEvent>::DestroyInstance


FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_UPDATEVIPLEVELNOTICE_H_ */
