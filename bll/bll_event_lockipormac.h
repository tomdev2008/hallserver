/*
 * bll_event_lockipormac.h
 *
 *  Created on: 2012-7-4
 *      Author: zhurw
 */

#ifndef BLL_EVENT_LOCKIPORMAC_H_
#define BLL_EVENT_LOCKIPORMAC_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
//#include "def/def_message_body.h"
#include "client_message_define.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"
#include "public_message_define.h"
#include "webagent_message_define.h"
#include "ctl/server_datacenter.h"
#include "public_typedef.h"
#include "hallserver_message_define.h"
#include "ctl/ipmacmap_mgt.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

/* 处理数据库发送来的封IP/MAC通知
 * 发送踢人通知给客户端踢出被封玩家(前提是玩家在线)
 * 发送下线通知给各个服务器(前提是玩家在线)
 */
class CLockIPOrMacEvent : public CBllBase
{
public:
	CLockIPOrMacEvent(){}

	virtual ~CLockIPOrMacEvent(){}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);
private:
	int32_t OnLockIpEvent(MessageHeadSS * pMsgHead, CLockNotice* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);
	int32_t OnLockMacEvent(MessageHeadSS * pMsgHead, CLockNotice* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);
	int32_t OnLockPlayerEvent(MessageHeadSS * pMsgHead, CLockNotice* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);
	int32_t KickOffPlayer(RoleID nRoleID, uint16_t nKickDays, KickReason nKickReason,
			CString<enmMaxDateTimeStringLength>& strLockEndDate,
			CString<MaxLockIPReasonLength>& sLockIPReason);
};

#define	CREATE_LOCKIPMACEVENT_INSTANCE	CSingleton<CLockIPOrMacEvent>::CreateInstance
#define	GET_LOCKIPMACEVENT_INSTANCE		CSingleton<CLockIPOrMacEvent>::GetInstance
#define	DESTROY_LOCKIPMACEVENT_INSTANCE	CSingleton<CLockIPOrMacEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_LOCKIPORMAC_H_ */
