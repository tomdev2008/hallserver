/*
 * bll_event_login.h
 *
 *  Created on: 2011-12-3
 *      Author: jimm
 */
#ifndef BLL_EVENT_LOGIN_H_
#define BLL_EVENT_LOGIN_H_

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
#include "ctl/ipmacmap_mgt.h"
#include "def/dal_def.h"
#include "hallserver_message_define.h"
#include "dal/dal_player.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

class CLoginMessageEvent : public CBllBase
{
public:
	CLoginMessageEvent()
	{
	}

	virtual ~CLoginMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	//会话事件激活
	virtual int32_t OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			CFrameSession* pSession, const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	int32_t OnSessionTimeOut(CFrameSession *pSession);

private:
	int32_t GetRoleInfoResp(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody, CFrameSession* pSession);

	int32_t GetRoleInfoTimeout(CFrameSession* pSession);

	int32_t SendOnlineNotiToServer(COnlineNoti* pNoti, CPlayer* pPlayer, const uint16_t nOptionLen, const void *pOptionData);

	//保存登陆请求的数据到会话中
	class CGetRoleInfoSessionData
	{
	public:
		MessageHeadSS			stMsgHead;
		uint16_t				m_nOptionLen;
		char					m_arrOptionData[MaxOptionDataSize];
		CLoginReq				stLoginReq;

		CGetRoleInfoSessionData()
		{
//			memset(&stMsgHead, 0, sizeof(stMsgHead));
			m_nOptionLen = 0;
//			memset(&m_arrOptionData, 0, sizeof(m_arrOptionData));
		}
	};

};

#define	CREATE_LOGINEVENT_INSTANCE	CSingleton<CLoginMessageEvent>::CreateInstance
#define	GET_LOGINEVENT_INSTANCE		CSingleton<CLoginMessageEvent>::GetInstance
#define	DESTROY_LOGINEVENT_INSTANCE	CSingleton<CLoginMessageEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_LOGIN_H_ */
