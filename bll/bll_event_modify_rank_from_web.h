/*
 *  bll_event_modify_rank_from_web.h
 *
 *  To do：
 *  Created on: 	2012-4-11
 *  Author: 		luocj
 */

#ifndef BLL_EVENT_MODIFY_RANK_FROM_WEB_H_
#define BLL_EVENT_MODIFY_RANK_FROM_WEB_H_


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

//后台批量修改玩家权限通知
class CModifyRankNotiEvent : public CBllBase
{
public:
	CModifyRankNotiEvent()
	{
	}
	virtual ~CModifyRankNotiEvent()
	{
	}
public:
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);
private:
    int32_t ModifyRoleRank(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
    		const uint16_t nOptionLen, const void *pOptionData);
    int32_t ModifyPlayerRoleRank(RoomID pRoom,RoleID nDestRoleID,RoleRank nRoleRank);

};

#define CREATE_MODIFYRANKNOTIEVENT_INSTANCE	    CSingleton<CModifyRankNotiEvent>::CreateInstance
#define GET_MODIFYRANKNOTIEVENT_INSTANCE		CSingleton<CModifyRankNotiEvent>::GetInstance
#define DESTROY_MODIFYRANKNOTIEVENT_INSTANCE	CSingleton<CModifyRankNotiEvent>::DestroyInstance


FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_MODIFY_RANK_FROM_WEB_H_ */
