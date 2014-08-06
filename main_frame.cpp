/*
 * main_frame.cpp
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#include "frame_logengine.h"
#include "def/dal_def.h"
#include "main_frame.h"

#include "frame_eventid.h"
#include "frame_logengine.h"
#include "bll/bll_event_default.h"
#include "bll/bll_sys_connection.h"
#include "bll/bll_event_login.h"
#include "bll/bll_event_logout.h"
#include "bll/bll_event_enterroomnotice.h"
#include "bll/bll_event_exitroomnotice.h"
#include "bll/bll_event_getroleglobalinfo.h"
#include "bll/bll_event_addtitle.h"
#include "bll/bll_event_deltitle.h"
#include "bll/bll_event_updateviplevelnotice.h"
#include "bll/bll_event_setselfinfo.h"
#include "bll/bll_event_modify_rank_from_web.h"
#include "bll/bll_event_onmic_notice.h"
#include "bll/bll_event_offmic_notice.h"
#include "bll/bll_event_createroomnotice.h"
#include "bll/bll_event_buildplayerdata.h"
#include "bll/bll_event_buildroomdata.h"
#include "bll/bll_event_getonlineplayerinfo.h"
#include "bll/bll_event_builddatanum.h"
#include "bll/bll_event_tunnelstartup.h"
#include "bll/bll_event_regandfillindatanotice.h"
#include "bll/bll_event_lockipormac.h"
#include "bll/bll_event_delroomnotice.h"
#include "bll/bll_event_lauchroomnotice.h"
#include "bll/bll_event_fetchroleonlinestatus.h"
#include "bll/bll_event_statuschangenoti_from_web.h"
#include "bll/bll_event_updateidentitynotice.h"
#include "bll/bll_event_checkonlinenum.h"
#include "bll/bll_event_checkdatanum.h"
#include "bll/bll_event_checkcrossdaysplayer.h"
#include "bll/bll_event_buildroomdatatimeout.h"
#include "bll/bll_event_builddatafinish.h"

#include "bll/bll_event_default.h"

#include "halldatacenter_message_define.h"
//#include "socket/test_socket.h"

#include "frame_netthread.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

CMainFrame::CMainFrame()
:CLightFrame(SERVER_NAME_STRING)
{

}

CMainFrame::~CMainFrame()
{
}

//框架初始话
int32_t CMainFrame::Initialize()
{
	int32_t ret=S_OK;

	//添加房间相关配置
	AddConfigCenter(0, DEFAULT_ABOUTROOM_CONFIG, &g_AboutRoomConfig);

	//添加客户端相关配置
	AddConfigCenter(0, DEFAULT_ABOUTCLIENT_CONFIG, &g_AboutClientConfig);

	//添加sever自己需要的数据中心
	ret=AddDataCenter(&GET_DALDATACENTER_INSTANCE());
	if(ret<0)
	{
		g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error, "Frame(add server data center) failed! ret=%d\n",ret);
		return ret;
	}
	g_FrameLogEngine.WriteBaseLog(enmLogLevel_Notice, "Frame(add server data center) success!\n");

	ret = CLightFrame::Initialize();
	if(0 > ret)
	{
		//WriteLog
		return ret;
	}

	int32_t nCheckOnlineTimerIndex = enmInvalidTimerIndex;
	ret = g_Frame.CreateTimer(static_cast<TimerProc>(&CCheckOnlineNumEvent::OnTimerEvent), &(GET_CHECKONLINENUMEVENT_INSTANCE()), NULL, enmCheckOnlineNumTimePeriod * US_PER_MINUTE, true, nCheckOnlineTimerIndex);
	if (ret < 0)
	{
		return ret;
	}

	int32_t nUpdateDataNumTimerIndex = enmInvalidTimerIndex;
	ret = g_Frame.CreateTimer(0, &(GET_CHECKCROSSDAYSEVENT_INSTANCE()), NULL, enmCheckCrossDaysTimePeriod * US_PER_MINUTE, true, nUpdateDataNumTimerIndex);
	if (ret < 0)
	{
		return ret;
	}

	int32_t nBuildRoomDataTimerIndex = enmInvalidTimerIndex;
	ret = g_Frame.CreateTimer(0, &(GET_BUILDROOMDATATIMEOUTEVENT_INSTANCE()), NULL, enmBuildRoomDataTimeOut * US_PER_SECOND, false, nBuildRoomDataTimerIndex);
	if(ret < 0)
	{
		return ret;
	}
	int32_t nCheckDataNumTimerIndex = enmInvalidTimerIndex;
	ret = g_Frame.CreateTimer(0, &(GET_CHECKDATANUMEVENT_INSTANCE()), NULL, enmCheckDataNumTimePeriod * US_PER_SECOND, false, nCheckDataNumTimerIndex);
	if (ret < 0)
	{
		return ret;
	}

	//注册所有的消息
	RegistMsg();

	g_FrameLogEngine.RegistLog("user_online_statistic_");

//	if(RegistSocket() < 0)
//	{
//		return -1;
//	}

	return S_OK;
}

//恢复框架
int32_t CMainFrame::Resume()
{
	return CLightFrame::Resume();

}
//销毁框架
int32_t CMainFrame::Terminate()
{
	g_eServerCommand = enmServerCommand_Terminate;
	return S_OK;
}

void CMainFrame::Run()
{
	CLightFrame::Run();
}

void CMainFrame::Reload()
{
	g_eServerCommand = enmServerCommand_Reload;
}

//void CMainFrame::OnTestRequest()
//{
//	CTestMain::OnTestRequest();
//}

//int32_t CMainFrame::RegistSocket()
//{
//	NetConfig stNetConfig;
//	int32_t ret = GetNetConfigByDesc("client", stNetConfig);
//	if(ret < 0)
//	{
//		return -1;
//	}
//
//	//卸载此端口上的套接字处理器
//	UnregistSocketHandler(stNetConfig.m_nListenPort);
//
//	for(int32_t i = 0; i < stNetConfig.m_nMaxConnectNum; ++i)
//	{
//		CSocket *pSocket = new CTestSocket();
//		if(pSocket == NULL)
//		{
//			return -1;
//		}
//
//		if(CLightFrame::RegistSocketHandler(pSocket, stNetConfig.m_nListenPort) < 0)
//		{
//			return -1;
//		}
//	}
//
//	CSocket *pSocket = g_FrameSocketMgt.CreateSocket(stNetConfig.m_nListenPort);
//	if(pSocket == NULL)
//	{
//		g_FrameLogEngine.WriteNetioLog(enmLogLevel_Error, "[%s:%d]create socket object error!\n",
//			__FILE__, __LINE__);
//		return -1;
//	}
//
//	pSocket->OpenSocketAsServer(stNetConfig.m_szListenAddress, stNetConfig.m_nListenPort);
//	pSocket->SetSocketStatus(enmSocketStatus_Connected);
//	pSocket->SetCreateTime(time(NULL));
//
//	CEpoll *pEpoll = g_FrameNetThread.GetEpollObj();
//	if(pEpoll == NULL)
//	{
//		return -1;
//	}
//
//	pEpoll->Control(pSocket->GetSocketFD(), pSocket, EPOLL_CTL_ADD, EPOLLIN | EPOLLET);
//
//	pSocket->SetEpollObj(pEpoll);
//
//	g_FrameSocketMgt.AddListenSocket(pSocket);
//
//	return 0;
//}

void CMainFrame::RegistMsg()
{
	INITIALIZE_MESSAGEMAP_DECL(HallServer_from_server);
	INITIALIZE_MESSAGEMAP_DECL(client_message);
	INITIALIZE_MESSAGEMAP_DECL(public_message);
	INITIALIZE_MESSAGEMAP_DECL(roomserver);
	INITIALIZE_MESSAGEMAP_DECL(itemserver);
	INITIALIZE_MESSAGEMAP_DECL(webagent);
	INITIALIZE_MESSAGEMAP_DECL(halldatacenter);

	//注册链接断开事件通知
	RegistSysEvent(SYS_EVENT_CONN_CLIENT_CLOSED, &CREATE_SYSTEMEVENT_INSTANCE());
	RegistSysEvent(SYS_EVENT_CONN_SERVER_CLOSED, &CREATE_SYSTEMEVENT_INSTANCE());
	RegistSysEvent(SYS_EVENT_CONN_ERROR, &CREATE_SYSTEMEVENT_INSTANCE());


	RegistMsgHandler(MSGID_HDTH_REBULIDHALLFINISH_NOTI, &GET_BUILDDATADINISHEVENT_INSTANCE());
	//RegistMsgHandler(MSGID_CLHS_LOGIN_REQ, &CREATE_LOGINEVENT_INSTANCE());
	RegistMsgHandler(MSGID_CLHS_LOGIN_REQ, &CREATE_LOGINEVENT_INSTANCE(), static_cast<SS_MSG_PROC>(&CLoginMessageEvent::OnMessageEvent));
	RegistMsgHandler(MSGID_CLHS_LOGOUT_REQ, &CREATE_LOGOUTEVENT_INSTANCE());

	RegistMsgHandler(MSGID_RSMS_ENTER_ROOM_NOTICE, &CREATE_ENTERROOMNOTICEEVENT_INSTANCE());
	RegistMsgHandler(MSGID_RSMS_EXIT_ROOM_NOTICE, &CREATE_EXITROOMNOTICEEVENT_INSTANCE());
	RegistMsgHandler(MSGID_RSHS_ONMIC_NOTIFY, &CREATE_ONMICNOTICEEVENT_INSTANCE());
	RegistMsgHandler(MSGID_RSHS_OFFMIC_NOTIFY, &CREATE_OFFMICNOTICEEVENT_INSTANCE());
	RegistMsgHandler(MSGID_RSMS_CREATE_ROOM_NOTICE, &CREATE_CREATEROOMNOTICEEVENT_INSTANCE());

	RegistMsgHandler(MSGID_RSHS_GET_ROLE_GLOBALINFO_REQ, &CREATE_GETROLEGLOBALINFOEVENT_INSTANCE());

	RegistMsgHandler(MSGID_CLHS_ADD_TITLE_REQ, &CREATE_ADDTITLEEVENT_INSTANCE());

	RegistMsgHandler(MSGID_CLHS_DEL_TITLE_REQ, &CREATE_DELTITLEEVENT_INSTANCE());

	RegistMsgHandler(MSGID_WEBOT_REG_AND_FILLIN_DATA_NOTIFY, &CREATE_REGANDFILLINDATANOTIEVENT_INSTANCE());

	RegistMsgHandler(MSGID_WEBOT_LAUNCHROOM_NOTICE, &CREATE_LAUCHROOMNOTICEEVENT_INSTANCE());

	RegistMsgHandler(MSGID_SSRS_UPDATELEVEL_NOTI, &CREATE_UPDATEVIPNOTIEVENT_INSTANCE());

	RegistMsgHandler(MSGID_CLHS_SETSELFINFO_REQ, &CREATE_SETSELFINFOEVENT_INSTANCE());

//	CFrame::RegistMsgHandler(MSGID_DBHS_LOCKPLAYER_NOTIFY, &CREATE_LOCKPLAYEREVENT_INSTANCE());

	RegistMsgHandler(MSGID_WEBOT_BATCH_MODIFY_ROLERANK_NOTIFY, &CREATE_MODIFYRANKNOTIEVENT_INSTANCE());
	RegistMsgHandler(MSGID_TSHS_HALLBUILDPLAYER_NOTI, &CREATE_BUILDPLAYEREVENT_INSTANCE());
	RegistMsgHandler(MSGID_TSHS_HALLBUILDROOM_NOTI, &CREATE_BUILDROOMEVENT_INSTANCE());
	RegistMsgHandler(MSGID_TSHS_UPDATEDATANUM_NOTI, &CREATE_BUILDDATANUMEVENT_INSTANCE());
	RegistMsgHandler(MSGID_DAEMON_START_NOTIFY, &CREATE_TUNNELSTARTUPEVENT_INSTANCE());

	RegistMsgHandler(MSGID_ISHS_GETONLINEPLAYERINFOINFO_REQ, &GET_GETONLINEPLAYERINFOEVENT_INSTANCE());


	RegistMsgHandler(MSGID_WEBOT_LOCK_NOTICE, &CREATE_LOCKIPMACEVENT_INSTANCE());
	RegistMsgHandler(MSGID_GET_ONLINE_STATUS_REQ, &CREATE_FETCHROLEONLINESTATUSEVENT_INSTANCE());
	RegistMsgHandler(MSGID_WEBOT_PWDPRO_STATUSCHANGE_NOTIFY, &CREATE_STATUSCHANGENOTIEVENT_INSTANCE());

	RegistMsgHandler(MSGID_RSMS_DESTORY_ROOM_NOTICE, &CREATE_DELROOMNOTICEEVENT_INSTANCE());

	RegistMsgHandler(MSGID_ISOS_UPDATEIDENTITY_NOTI, &CREATE_UPDATEIDENTITYNOTIEVENT_INSTANCE());
}

FRAME_HALLSERVER_NAMESPACE_END

