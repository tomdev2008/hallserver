/*
 * bll_event_setselfinfo.h
 *
 *  Created on: 2012-3-21
 *      Author: zhurw
 */

#ifndef BLL_EVENT_SETSELFINFO_H_
#define BLL_EVENT_SETSELFINFO_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "client_message_define.h"
#include "dal/to_server_message.h"
#include "ctl/server_datacenter.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"
#include "config/someconfig.h"
#include "def/dal_def.h"
#include "hallserver_message_define.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

class CSetSelfInfoEvent: public CBllBase
{
public:
	CSetSelfInfoEvent()
	{
	}
	virtual ~CSetSelfInfoEvent()
	{
	}
public:
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	virtual int32_t OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody, CFrameSession* pSession,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	virtual int32_t OnSessionTimeOut(CFrameSession *pSession);

private:
	int32_t OnSetSelfInfoEvent(MessageHeadSS* pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void* pOptionData = NULL);

	class CSetSelfInfoSessionData
	{
	public:
		MessageHeadSS m_sMsgHead;
		uint16_t m_nOptionLen;
		char m_arrOptionData[MaxOptionDataSize];
		CSetSelfInfoReq m_pMsgBody;
		CSetSelfInfoSessionData()
		{
//			memset(&m_sMsgHead, 0 , sizeof(m_sMsgHead));
			m_nOptionLen = 0;
//			memset(m_arrOptionData, 0, sizeof(m_arrOptionData));
//			memset(&m_pMsgBody, 0, sizeof(m_pMsgBody));
		}
	};
};

#define CREATE_SETSELFINFOEVENT_INSTANCE	CSingleton<CSetSelfInfoEvent>::CreateInstance
#define GET_SETSELFINFOEVENT_INSTANCE		CSingleton<CSetSelfInfoEvent>::GetInstance
#define DESTROY_SETSELFINFOEVENT_INSTANCE	CSingleton<CSetSelfInfoEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_SETSELFINFO_H_ */
