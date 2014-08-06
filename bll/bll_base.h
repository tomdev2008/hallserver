/*
 * bll_base.h
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#ifndef BLL_BASE_H_
#define BLL_BASE_H_

#include "main_frame.h"
#include "public_typedef.h"
#include "lightframe_impl.h"
#include "common/common_datetime.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

class CBllBase  : public IMessageEventSS, public ISessionEvent
{
public:
	CBllBase()
	{

	}
	virtual ~CBllBase()
	{

	}
public:
/*
消息事件接口
*/
	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL)
	{return S_OK;};

public:
/*
会话事件接口
*/
	/*
		会话事件激活
	*/
	virtual int32_t OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			CFrameSession* pSession, const uint16_t nOptionLen = 0, const void *pOptionData = NULL)
	{return S_OK;};
	/*
		会话超时
	*/
	virtual int32_t OnSessionTimeOut(CFrameSession *pSession) {return S_OK;};

public:
	/*
		发送请求消息
	*/
	void SendMessageRequest(uint32_t nMsgID, IMsgBody* pMsgBody, const RoomID nRoomID, TransType nTransType,
			RoleID nRoleID, EntityType nDestType, const int32_t nDestID, const SessionIndex nSessionIndex,
			const uint16_t nOptionLen = 0, const char *pOptionData = NULL,
			const char *szDumpContent="send request message");
	/*
		发送相应消息
	*/
	void SendMessageResponse(uint32_t nMsgID,MessageHeadSS * pMsgHead,IMsgBody* pMsgBody,
			TransType nTransType=enmTransType_P2P,
			const uint16_t nOptionLen = 0, const char *pOptionData = NULL,
			const char *szDumpContent="send response message");

	void SendMessageNotifyToServer(uint32_t nMsgID,IMsgBody* pMsgBody,const RoomID nRoomID,
			const TransType transType,const RoleID nRoleID,
			const EntityType destType,const int32_t nDestID = 0,
			const uint16_t nOptionLen = 0, const char *pOptionData = NULL,
			const char *szDumpContent="send notify message");

	//发送广播消息给房间内某些玩家(一个或者部分或者所有)
	void SendMessageNotifyToClient(uint32_t nMsgID, IMsgBody* pMsgBody, RoomID nRoomID, BroadcastType nType,
			RoleID nRoleID, const uint16_t nOptionLen = 0,const char *pOptionData = NULL,
			const char *szDumpContent="send notify message");

	/*
		发送通知消息
	*/
	void SendMessageNotify(const RoleID destID, IMsgBody* pMsgBody, uint32_t nMsgID,
			const char *szDumpContent="send notify message", const RoomID nRoomID = enmInvalidRoomID,
			const uint16_t nOptionLen = 0, const char *pOptionData = NULL);

	void SendMessageNotify(const RoleID destID,const EntityType destType,const  TransType transType,
			IMsgBody* pMsgBody,uint32_t nMsgID,const char *szDumpContent="send notify message",
			const RoomID nRoomID = enmInvalidRoomID,
			const uint16_t nOptionLen = 0, const char *pOptionData = NULL);

public:
//	void    SendOfficialMail(RoleID nRoleID, const char* szMsg, int32_t nClientID);
	/*
		打印接受，发送的消息
	*/
	void DumpMessage(const char* szContent, MessageHeadSS *pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const char *pOptionData = NULL);
	int32_t UpdatePlayerDataInfo();
	int32_t UpdateRoomDataInfo(UpdateDataType nUpdateType = enmUpdateDataType_Room);

	int64_t GetLeftSecondsBeforeTomorrow(CDateTime currentTime);
private:
		uint8_t					m_arrProtocolBuf[32];
};


#define	CREATE_BLLBASE_INSTANCE		CSingleton<CBllBase>::CreateInstance
#define	GET_BLLBASE_INSTANCE		CSingleton<CBllBase>::GetInstance
#define	DESTROY_BLLBASE_INSTANCE	CSingleton<CBllBase>::DestroyInstance

#define g_BllBase GET_BLLBASE_INSTANCE()

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_BASE_H_ */
