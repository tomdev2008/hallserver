/*
 * bll_event_getonlineplayerinfo.h
 *
 *  Created on: 2012-5-18
 *      Author: jimm
 */

#ifndef BLL_EVENT_GETONLINEPLAYERINFO_H_
#define BLL_EVENT_GETONLINEPLAYERINFO_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

//ItemServer分批获取在线玩家信息
class CGetOnlinePlayerInfoMessageEvent : public CBllBase
{
public:
	CGetOnlinePlayerInfoMessageEvent()
	{
	}

	virtual ~CGetOnlinePlayerInfoMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:

};

#define	CREATE_GETONLINEPLAYERINFOEVENT_INSTANCE	CSingleton<CGetOnlinePlayerInfoMessageEvent>::CreateInstance
#define	GET_GETONLINEPLAYERINFOEVENT_INSTANCE		CSingleton<CGetOnlinePlayerInfoMessageEvent>::GetInstance
#define	DESTROY_GETONLINEPLAYERINFOEVENT_INSTANCE	CSingleton<CGetOnlinePlayerInfoMessageEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_GETONLINEPLAYERINFO_H_ */
