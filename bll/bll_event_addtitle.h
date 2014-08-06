/*
 * bll_event_addtitle.h
 *
 *  Created on: 2012-2-28
 *      Author: zhurw
 */

#ifndef BLL_EVENT_ADDTITLE_H_
#define BLL_EVENT_ADDTITLE_H_

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


FRAME_HALLSERVER_NAMESPACE_BEGIN

class CAddTitleEvent: public CBllBase
{
public:
	CAddTitleEvent()
	{
	}
	virtual ~CAddTitleEvent()
	{
	}

public:
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
				const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	virtual int32_t OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody, CFrameSession* pSession,
				const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	virtual int32_t OnSessionTimeOut(CFrameSession *pSession);

private:
	int32_t OnAddTitleEvent(MessageHeadSS* pMsgHead, IMsgBody* pMsgBody,
				const uint16_t nOptionLen = 0, const void *pOptionData = NULL);
	int32_t OnSessionAddTitle(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody, CFrameSession* pSession,
				const uint16_t nOptionLen = 0, const void *pOptionData = NULL);
	int32_t SendReqToDB(MessageHeadSS* pMsgHead, IMsgBody* pMsgBody, const uint16_t nOptionLen = 0,
				const void* pOptionData = NULL, CPlayer* pDestPlayer = NULL, CRoom* pRoom = NULL);
	int32_t AddRoleRank(MessageHeadSS* pMsgHead, CAddTitleReq* pAddTitleReq, RoleID nDestRoleID,
				RoleRank nRoleRank, const uint16_t nOptionLen = 0, const char* pOptionData = NULL);
	int32_t PreAddRank(MessageHeadSS* pMsgHead, CAddTitleReq* pAddTitleReq, CPlayer* pSrcPlayer, CRoom* pRoom,
				const uint16_t nOptionLen = 0, const void* pOptionData = NULL);

//保持会话数据
	class CSetTitleSessionData
	{
	public:
		MessageHeadSS m_sMsgHead;
		uint16_t m_nOptionLen;
		char m_arrOptionData[MaxOptionDataSize];
		CAddTitleReq m_pMsgBody;
		CRoom *m_pRoom;
		CPlayer *m_pPlayer;

		CSetTitleSessionData()
		{
//			memset(&m_sMsgHead, 0, sizeof(m_sMsgHead));
			m_nOptionLen = 0;
//			memset(m_arrOptionData, 0, sizeof(m_arrOptionData));
//			memset(&m_pMsgBody, 0, sizeof(m_pMsgBody));
			m_pRoom = NULL;
			m_pPlayer = NULL;
		}
	};
	class CUpdateRankSessionData
	{
	public:
		MessageHeadSS m_sMsgHead;
		uint16_t m_nOptionLen;
		char m_arrOptionData[MaxOptionDataSize];
		CAddTitleReq m_pMsgBody;
		RoleID nDestRoleID;
		RoleID nSrcRoleID ;
		RoleRank nRoleRank;
		CUpdateRankSessionData()
		{
//			memset(&m_sMsgHead, 0, sizeof(m_sMsgHead));
			m_nOptionLen = 0;
//			memset(m_arrOptionData, 0, sizeof(m_arrOptionData));
//			memset(&m_pMsgBody, 0, sizeof(m_pMsgBody));
			nDestRoleID = enmInvalidRoleID;
			nSrcRoleID = enmInvalidRoleID;
			nRoleRank = enmRoleRank_None;
		}
	};
};


#define CREATE_ADDTITLEEVENT_INSTANCE 	CSingleton<CAddTitleEvent>::CreateInstance
#define GET_ADDTITLEEVENT_INSTANCE    	CSingleton<CAddTitleEvent>::GetInstance
#define DESTROY_ADDTITLEEVENT_INSTANCE 	CSingleton<CAddTitleEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_ADDTITLE_H_ */
