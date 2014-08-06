/*
 * bll_event_login.cpp
 *
 *  Created on: 2011-12-3
 *      Author: jimm
 */

#include "bll_event_login.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "client_message_define.h"
#include "dal/to_server_message.h"
#include "dal/from_server_message.h"
#include "config/someconfig.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CLoginMessageEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	// check the server status
	CLoginResp stLoginResp;
	stLoginResp.nResult = enmLoginResult_Unknown;
	stLoginResp.strFailReason = "unknown error!";
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		return E_NULLPOINTER;
	}

	if(pMsgBody == NULL || pMsgHead == NULL || pSession == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x, pSession=0x%08x}\n",
				pMsgHead, pMsgBody, pSession);
		return E_NULLPOINTER;
	}

	switch(pSession->GetSessionType())
	{
		case enmSessionType_GetRoleInfo:
		GetRoleInfoResp(pMsgHead, pMsgBody, pSession);
		break;
		default:
		WRITE_ERROR_LOG("login:get wrong session type!{nSessionType=0x%04X}\n", pSession->GetSessionType());
		break;
	}

	return S_OK;
}
int32_t CLoginMessageEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	WRITE_DEBUG_LOG("login session time out!\n");
	if(pSession == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pSession=NULL, nRet=0x%08x}\n",E_NULLPOINTER);
		return E_NULLPOINTER;
	}

	switch(pSession->GetSessionType())
	{
		case enmSessionType_GetRoleInfo:
		GetRoleInfoTimeout(pSession);
		break;
		default:
		WRITE_ERROR_LOG("login:session time out!\n");
		break;
	}

	return S_OK;
}
int32_t CLoginMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}
	// check the server status
	CLoginResp stLoginResp;
	stLoginResp.nResult = enmLoginResult_Unknown;
	stLoginResp.strFailReason = "unknown error!";
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		SendMessageResponse(MSGID_HSCL_LOGIN_RESP, pMsgHead, &stLoginResp, enmTransType_P2P,
				nOptionLen, (char *)pOptionData);
		return E_NULLPOINTER;
	}

	CLoginReq *pLoginMsg = dynamic_cast<CLoginReq *>(pMsgBody);
	if(NULL == pLoginMsg)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CLoginReq class failed!{nMessageID=0x%08x, nRoleID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		SendMessageResponse(MSGID_HSCL_LOGIN_RESP, pMsgHead, &stLoginResp, enmTransType_P2P,
				nOptionLen, (char *)pOptionData);

		return E_NULLPOINTER;
	}

	CPlayer *pPlayer = NULL;
	int32_t ret = g_PlayerMgt.GetPlayer(pMsgHead->nRoleID, pPlayer);
	if(pPlayer != NULL)
	{
		WRITE_DEBUG_LOG("player already login!{nRoleID=%d}\n", pMsgHead->nRoleID);

		//锟斤拷前锟斤拷锟铰斤拷母锟斤拷叩锟�
		ConnUin stKickUserInfo = pPlayer->GetConnInfo();
		uint32_t nKickUserOffset = 0;
		uint8_t arrKickUserOptionData[MaxOptionDataSize];
		ret = stKickUserInfo.MessageEncode(arrKickUserOptionData, MaxOptionDataSize, nKickUserOffset);
		if(ret < 0)
		{
			WRITE_ERROR_LOG("kick user error, encode connuin failed!{ret=0x%08x, nRoleID=%d}\n", ret, pMsgHead->nRoleID);
			SendMessageResponse(MSGID_HSCL_LOGIN_RESP, pMsgHead, &stLoginResp, enmTransType_P2P,
					nOptionLen, (char *)pOptionData);
			return ret;
		}

		if(memcmp(arrKickUserOptionData, pOptionData, nOptionLen) == 0)
		{
			WRITE_NOTICE_LOG("it's same player login again!{nRoleID=%d, nAddress=%s, nPort=%d}\n",
					pMsgHead->nRoleID, inet_ntoa_f(pPlayer->GetConnInfo().nAddress), pPlayer->GetConnInfo().nPort);
			return S_OK;
		}

		CKickUserNoti stKickNoti;
		stKickNoti.nResult = enmKickReason_Relogin;
		stKickNoti.strReason = "already login!";
		//锟斤拷突锟斤拷朔锟斤拷锟斤拷叱锟酵ㄖ�
		SendMessageNotifyToClient(MSGID_HSCL_KICKUSER_NOTI, &stKickNoti, 0, enmBroadcastType_ExpectPlayr,
				pMsgHead->nRoleID, nKickUserOffset, (char *)arrKickUserOptionData);

		//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟酵ㄖ�
		COfflineNoti stOfflineNoti;
		stOfflineNoti.nRoleID = pMsgHead->nRoleID;
		stOfflineNoti.nLogoutTime = CDateTime::CurrentDateTime().Seconds();
		stOfflineNoti.nConnUin = pPlayer->GetConnInfo();
		stOfflineNoti.nLogoutReason = enmLogoutReason_Relogin;

//		SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stOfflineNoti, enmInvalidRoomID,
//				enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_Item);
//		SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stOfflineNoti, enmInvalidRoomID,
//				enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_Media);
//		SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stOfflineNoti, enmInvalidRoomID,
//				enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_Room);
//		SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stOfflineNoti, enmInvalidRoomID,
//				enmTransType_P2P, pMsgHead->nRoleID, enmEntityType_Tunnel, pPlayer->GetConnInfo().nServerID);
//
//		/*锟斤拷锟斤拷锟斤拷锟斤拷通知锟斤拷sysinfo server*/
//		SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stOfflineNoti, enmInvalidRoomID,
//				enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_SysInfo);

		SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stOfflineNoti, enmInvalidRoomID,
						enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_None);

		//锟剿筹拷锟斤拷医锟斤拷锟斤拷锟侥凤拷锟斤拷
		int32_t nEnterRoomCount = pPlayer->GetCurEnterRoomCount();
		WRITE_DEBUG_LOG("player current entered room info:{nRoleID=%d, nEnterRoomCount=%d}\n",
				pMsgHead->nRoleID, nEnterRoomCount);
		if(nEnterRoomCount != 0)
		{
			RoomID arrRoomID[MaxEnterRoomCount] = {enmInvalidRoomID};
			int32_t nGetCurRoomCount = 0;
			pPlayer->GetAllEnterRoom(arrRoomID, MaxEnterRoomCount, nGetCurRoomCount);
			for(int32_t i = 0; i < nGetCurRoomCount; ++i)
			{
				g_RoomMgt.DeletePlayer(arrRoomID[i], pMsgHead->nRoleID);
				WRITE_DEBUG_LOG("player is kicked from room by others!{nRoleID=%d, nRoomID=%u}\n", pMsgHead->nRoleID, arrRoomID[i]);
			}
		}
		g_PlayerMgt.DestoryPlayer(pMsgHead->nRoleID);
		uint32_t nCount = g_PlayerMgt.GetRealPlayerCount();
		if(nCount >= 1)
		{
			g_PlayerMgt.SetRealPlayerCount(--nCount);
		}
	}

	ret = g_PlayerMgt.CreatePlayer(pMsgHead->nRoleID, pPlayer);
	uint32_t nCount = g_PlayerMgt.GetRealPlayerCount();
	g_PlayerMgt.SetRealPlayerCount(++nCount);
	if(ret < 0 || pPlayer == NULL)
	{
		WRITE_ERROR_LOG("login error, failed to create player!{ret=0x%08x, nRoleID=%d}\n", ret, pMsgHead->nRoleID);
		SendMessageResponse(MSGID_HSCL_LOGIN_RESP, pMsgHead, &stLoginResp, enmTransType_P2P,
				nOptionLen, (char *)pOptionData);

		return E_NULLPOINTER;
	}

	ConnUin stInfo;
	uint32_t offset = 0;
	ret = stInfo.MessageDecode((uint8_t *)pOptionData, nOptionLen, offset);
	if(ret < 0)
	{
		WRITE_ERROR_LOG("login error, decode connuin failed!{ret=0x%08x, nRoleID=%d}\n", ret, pMsgHead->nRoleID);
		SendMessageResponse(MSGID_HSCL_LOGIN_RESP, pMsgHead, &stLoginResp, enmTransType_P2P,
				nOptionLen, (char *)pOptionData);
		//锟斤拷锟斤拷锟斤拷时注锟斤拷锟斤拷锟�
		g_PlayerMgt.DestoryPlayer(pMsgHead->nRoleID);
		uint32_t nCount = g_PlayerMgt.GetRealPlayerCount();
		if(nCount >= 1)
		{
			g_PlayerMgt.SetRealPlayerCount(--nCount);
		}
		return ret;
	}

	// for statistic
	STATISTIC_WRITE_INFO_LOG("log_notice, login, {nRoleID=%d, nAddress=%u}\n",
			pMsgHead->nRoleID, stInfo.nAddress);

	pPlayer->SetConnInfo(stInfo);
	pPlayer->SetLoginTime(CTimeValue::CurrentTime().Seconds());
	pPlayer->SetRoleID(pMsgHead->nRoleID);
	//set player mac address
	pPlayer->setPlayerMacAddr(g_IpMacMapMgt.MacCStringToLong(pLoginMsg->strMacAddr.GetString()));

	//锟斤拷锟芥本
	int32_t nVersion = (int32_t)pLoginMsg->nClientVersion;
	if(!g_AboutClientConfig.IsAllowedVersion(nVersion))
	{
		stLoginResp.nResult = enmLoginResult_VerError;
		stLoginResp.strFailReason = "client version error!";
		WRITE_NOTICE_LOG("version error:client error error!{nClientVersion=%d}\n", pLoginMsg->nClientVersion);
		SendMessageResponse(MSGID_HSCL_LOGIN_RESP, pMsgHead, &stLoginResp, enmTransType_P2P,
				nOptionLen, (char*)pOptionData);
		return S_OK;
	}

	if(pLoginMsg->nAccountType == enmAccountType_Visitor)
	{
		pPlayer->SetAccountID(pMsgHead->nRoleID);
		COnlineNoti stNoti;
		stNoti.nRoleID = pMsgHead->nRoleID;
		stNoti.strRoleName = "";
		stNoti.nAccountID = pPlayer->GetAccountID();
		stNoti.nConnUin = stInfo;
		stNoti.nVipLevel = 0;
		stNoti.nLeftMoney = 0;
		stNoti.nLoginTime = CTimeValue::CurrentTime().Seconds();
		SendOnlineNotiToServer(&stNoti, pPlayer, nOptionLen, pOptionData);

		//锟斤拷锟斤拷应锟斤拷锟酵伙拷锟斤拷
		stLoginResp.nResult = S_OK;
		stLoginResp.nLastVersion = 0;
		stLoginResp.nLastLoginTimeHigh32 = 0;
		stLoginResp.nLastLoginTimeLow32 = 0;
		stLoginResp.nSystemTimeHigh32 = 0;
		stLoginResp.nSystemTimeLow32 = 0;
		stLoginResp.nGender = enmGender_Unisex;
		stLoginResp.strRoleName = "";
		stLoginResp.nAccountID = pMsgHead->nRoleID;
		stLoginResp.nVipLevel = 0;
		stLoginResp.nUserLevel = 0;
		stLoginResp.nOnlineTime = 0;
		stLoginResp.nLeftTime = 0;
		stLoginResp.nLeftMoney = 0;
		stLoginResp.nNetType = pPlayer->GetConnInfo().nNetType;
		stLoginResp.nSelfSetting = 0;
		stLoginResp.nRmbUserType = 0;
		stLoginResp.nAccountSafeStatus = 0;
		WRITE_DEBUG_LOG("send player login event response to client!{nRoleID=%d}\n", pMsgHead->nRoleID);
		SendMessageResponse(MSGID_HSCL_LOGIN_RESP, pMsgHead, &stLoginResp, enmTransType_P2P,
				nOptionLen, (char*)pOptionData);

		//锟斤拷录IP锟斤拷RoleID锟斤拷映锟斤拷
		g_IpMacMapMgt.AddIpRoleIDItem(pPlayer->GetConnInfo().nAddress, pMsgHead->nRoleID);
		//锟斤拷录MAC锟斤拷RoleID锟斤拷映锟斤拷
		g_IpMacMapMgt.AddMacRoleIDItem(pPlayer->GetPlayerMacAddr(), pMsgHead->nRoleID);

		return S_OK;
	}
	CFrameSession *pSession = NULL;
	ret = g_Frame.CreateSession(MSGID_DBHS_GETROLELOGININFO_RESP, enmSessionType_GetRoleInfo, this, enmGetRoleInfoTimeoutPeriod, pSession);
	if(ret < 0 || pSession == NULL)
	{
		WRITE_ERROR_LOG("login error, failed to create get role info session!{nRoleID=%d}\n", pMsgHead->nRoleID);

		SendMessageResponse(MSGID_HSCL_LOGIN_RESP, pMsgHead, &stLoginResp, enmTransType_P2P,
				nOptionLen, (char *)pOptionData);
		//锟斤拷锟斤拷锟斤拷时注锟斤拷锟斤拷锟�
		g_PlayerMgt.DestoryPlayer(pMsgHead->nRoleID);
		uint32_t nCount = g_PlayerMgt.GetRealPlayerCount();
		if(nCount >= 1)
		{
			g_PlayerMgt.SetRealPlayerCount(--nCount);
		}
		return E_NULLPOINTER;
	}

	CGetRoleInfoSessionData * pData = new (pSession->GetSessionData())CGetRoleInfoSessionData();
	memcpy(&pData->stMsgHead, pMsgHead, sizeof(pData->stMsgHead));
	if(nOptionLen > 0)
	{
		pData->m_nOptionLen = nOptionLen;
		memcpy(pData->m_arrOptionData, pOptionData, nOptionLen);
	}

	memcpy(&pData->stLoginReq, pLoginMsg, sizeof(CLoginReq));

	//锟斤拷锟斤拷锟斤拷锟斤拷
	CGetRoleInfoReq stGetRoleInfoReq;
	stGetRoleInfoReq.nRoleID = pMsgHead->nRoleID;
	stGetRoleInfoReq.nIpAddr = stInfo.nAddress;
	stGetRoleInfoReq.strMacAddr = pLoginMsg->strMacAddr;
	stGetRoleInfoReq.nOSVersion = pLoginMsg->nOSVersion;
	stGetRoleInfoReq.nChannelID = pLoginMsg->nChannel;
	stGetRoleInfoReq.strAccountName = pLoginMsg->strAccountName;

	SendMessageRequest(MSGID_HSDB_GETROLELOGININFO_REQ, &stGetRoleInfoReq, enmInvalidRoomID,
			enmTransType_ByKey, pMsgHead->nRoleID, enmEntityType_DBProxy, pMsgHead->nRoleID,
			pSession->GetSessionIndex(), 0, NULL, "send get role info request");

	return S_OK;
}

/*
 * 锟斤拷要锟睫改伙拷取锟斤拷锟斤拷锟较⑿拷锟�
 * 锟斤拷锟斤拷1锟斤拷锟斤拷锟斤拷锟街段凤拷锟截憋拷锟斤拷原锟斤拷
 */
int32_t CLoginMessageEvent::GetRoleInfoResp(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody, CFrameSession* pSession)
{
	CGetRoleInfoSessionData * pData =(CGetRoleInfoSessionData *)(pSession->GetSessionData());
	if(NULL == pData)
	{
		WRITE_ERROR_LOG("null pointer:get role info session, get data failed!{nSessionIndex=%d}\n",
				pSession->GetSessionIndex());
		return E_NULLPOINTER;
	}

	CLoginResp stLoginResp;
	CGetRoleInfoResp *pGetRoleInfoResp = dynamic_cast<CGetRoleInfoResp *>(pMsgBody);
	if(NULL == pGetRoleInfoResp)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CGetRoleInfoResp class failed!{nMessageID=0x%08x, nRoleID=%d, "
				"nSessionType=0x%04X, nSessionId=%d}\n", pData->stMsgHead.nMessageID, pData->stMsgHead.nRoleID,
				pSession->GetSessionType(), pSession->GetSessionIndex());

		stLoginResp.nResult = enmLoginResult_Unknown;
		stLoginResp.strFailReason = "unknown error!";

		SendMessageResponse(MSGID_HSCL_LOGIN_RESP, &pData->stMsgHead, &stLoginResp, enmTransType_P2P,
				pData->m_nOptionLen, pData->m_arrOptionData);

		return E_NULLPOINTER;
	}
	if(pGetRoleInfoResp->nResult < 0)
	{
		WRITE_ERROR_LOG("get role info response's result < 0, {nResult=0x%08x}\n", pGetRoleInfoResp->nResult);
		stLoginResp.nResult = enmLoginResult_Unknown;
		stLoginResp.strFailReason = "unknown error!";
		SendMessageResponse(MSGID_HSCL_LOGIN_RESP, &pData->stMsgHead, &stLoginResp, enmTransType_P2P,
				pData->m_nOptionLen, pData->m_arrOptionData);
		return E_NULLPOINTER;
	}
	WRITE_DEBUG_LOG("receive get role info response!{nRoleID=%d}\n", pGetRoleInfoResp->nRoleID);

	int64_t nCurTime = CTimeValue::CurrentTime().Seconds();
//	WRITE_DEBUG_LOG("currenttime:{nCurTime=%ld}", nCurTime);

	//锟斤拷锟斤拷欠锟斤拷诒锟斤拷锟阶刺�
	if(((int64_t)pGetRoleInfoResp->nTimeUnlock != 0) && (nCurTime <= (int64_t)pGetRoleInfoResp->nTimeUnlock))
	{
		WRITE_DEBUG_LOG("player try to login while was locked!{nRoleID=%d}\n", pData->stMsgHead.nRoleID);
		stLoginResp.nResult = enmLoginResult_AccountLock;
		stLoginResp.strFailReason = pGetRoleInfoResp->strAccountLockReason;
		stLoginResp.nKickDays = (pGetRoleInfoResp->nTimeUnlock - pGetRoleInfoResp->nTimelock)/SECOND_PER_DAY;
		CDateTime nUnlockTime(pGetRoleInfoResp->nTimeUnlock * US_PER_SECOND);
		char tEndtime[enmMaxDateTimeStringLength];
		memset(tEndtime, '\0', enmMaxDateTimeStringLength);
		nUnlockTime.ToDateString(tEndtime);
		CString<enmMaxDateTimeStringLength> strUnlockDate(tEndtime);
		stLoginResp.strLockEndDate = strUnlockDate;
		WRITE_DEBUG_LOG("send player was locked response to client!{nRoleID=%d}\n", pMsgHead->nRoleID);
		SendMessageResponse(MSGID_HSCL_LOGIN_RESP, &pData->stMsgHead, &stLoginResp, enmTransType_P2P,
				pData->m_nOptionLen, pData->m_arrOptionData);
		return S_OK;
	}
	if(pGetRoleInfoResp->nIpAddr != 0)
	{
		if((pGetRoleInfoResp->nLockIPEndTime != 0) && ((uint64_t)nCurTime <= pGetRoleInfoResp->nLockIPEndTime))
		{
			WRITE_DEBUG_LOG("ip of player is locked!{ip=%s}\n",inet_ntoa_f(pGetRoleInfoResp->nIpAddr));
			stLoginResp.nResult = enmLoginResult_IpLock;
			stLoginResp.strFailReason = pGetRoleInfoResp->strLockIpReason;
			stLoginResp.nKickDays = (pGetRoleInfoResp->nLockIPEndTime - pGetRoleInfoResp->nLockIPStartTime)/SECOND_PER_DAY;
			CDateTime nLockIPEndTime(pGetRoleInfoResp->nLockIPEndTime * US_PER_SECOND);
			char tEndtime[enmMaxDateTimeStringLength];
			memset(tEndtime, '\0', enmMaxDateTimeStringLength);
			nLockIPEndTime.ToDateString(tEndtime);
			CString<enmMaxDateTimeStringLength> strLockIPEndDate = tEndtime;
			stLoginResp.strLockEndDate = strLockIPEndDate;
			WRITE_DEBUG_LOG("send ip was locked response to client!{nRoleID=%d}\n", pMsgHead->nRoleID);
			SendMessageResponse(MSGID_HSCL_LOGIN_RESP, &pData->stMsgHead, &stLoginResp, enmTransType_P2P,
					pData->m_nOptionLen, pData->m_arrOptionData);
			return S_OK;
		}
	}
	if(pGetRoleInfoResp->strMacAddr.GetStringLength() != 0)
	{
		if((pGetRoleInfoResp->nLockMacEndTime != 0) && ((uint64_t)nCurTime <= pGetRoleInfoResp->nLockMacEndTime))
		{

			WRITE_DEBUG_LOG("mac of player is locked!{mac=%s}\n", pGetRoleInfoResp->strMacAddr.GetString());
			stLoginResp.nResult = enmLoginResult_MacLock;
			stLoginResp.strFailReason = pGetRoleInfoResp->strLockMacReason;
			stLoginResp.nKickDays = (pGetRoleInfoResp->nLockMacEndTime - pGetRoleInfoResp->nLockMacStartTime)/SECOND_PER_DAY;
			CDateTime nLockMacEndTime(pGetRoleInfoResp->nLockMacEndTime * US_PER_SECOND);
			char tEndtime[enmMaxDateTimeStringLength];
			memset(tEndtime, '\0', enmMaxDateTimeStringLength);
			nLockMacEndTime.ToDateString(tEndtime);
			CString<enmMaxDateTimeStringLength> strLockMacEndDate(tEndtime);
			stLoginResp.strLockEndDate = strLockMacEndDate;
			WRITE_DEBUG_LOG("send mac was locked response to client!{nRoleID=%d}\n", pMsgHead->nRoleID);
			SendMessageResponse(MSGID_HSCL_LOGIN_RESP, &pData->stMsgHead, &stLoginResp, enmTransType_P2P,
					pData->m_nOptionLen, pData->m_arrOptionData);
			return S_OK;
		}
	}
//	WRITE_DEBUG_LOG("stringlen=%d,endtime=%lu,nCurTime=%lu", pGetRoleInfoResp->strMacAddr.GetStringLength(),
//			pGetRoleInfoResp->nLockMacEndTime, nCurTime);

	CPlayer *pPlayer = NULL;
	int32_t ret = g_PlayerMgt.GetPlayer(pData->stMsgHead.nRoleID, pPlayer);
	if(pPlayer == NULL)
	{
		WRITE_DEBUG_LOG("cannot find role object!{RoleID=%d, ret=0x%08x}\n", pData->stMsgHead.nRoleID, ret);

		stLoginResp.nResult = enmLoginResult_Unknown;
		stLoginResp.strFailReason = "unknown error!";
	}
	else if(pGetRoleInfoResp->nResult < 0)
	{
		stLoginResp.nResult = enmLoginResult_Unknown;
		stLoginResp.strFailReason = "unknown error!";
	}
	else
	{
		ConnInfo stSessionConnInfo;
		uint32_t nOffset = 0;
		ret = stSessionConnInfo.MessageDecode((uint8_t *)pData->m_arrOptionData, pData->m_nOptionLen, nOffset);
		if(ret < 0)
		{
			WRITE_ERROR_LOG("decode session conn info failed!{RoleID=%d, ret=0x%08x}\n", pMsgHead->nRoleID, ret);
			return ret;
		}

		ConnInfo stPlayerConnInfo = pPlayer->GetConnInfo();
		if(!stPlayerConnInfo.Compare(&stSessionConnInfo))
		{
			WRITE_WARNING_LOG("this response must be discard!{RoleID=%d, Address=%s, Port=%d}\n",
					pMsgHead->nRoleID, inet_ntoa_f(stSessionConnInfo.nAddress), stSessionConnInfo.nPort);
			return S_OK;
		}

		stLoginResp.nResult = S_OK;
		stLoginResp.nLastVersion = pGetRoleInfoResp->nLastVersion;
		stLoginResp.nLastLoginTimeHigh32 = pGetRoleInfoResp->nLastLoginTime & 0xffffffff00000000;
		stLoginResp.nLastLoginTimeLow32 = pGetRoleInfoResp->nLastLoginTime & 0x00000000ffffffff;
		stLoginResp.nSystemTimeHigh32 = pGetRoleInfoResp->nSystemTime & 0xffffffff00000000;
		stLoginResp.nSystemTimeLow32 = pGetRoleInfoResp->nSystemTime & 0x00000000ffffffff;
		stLoginResp.nGender = pGetRoleInfoResp->nGender;
		stLoginResp.strRoleName = pGetRoleInfoResp->strRoleName;
		stLoginResp.nAccountID = pGetRoleInfoResp->nAccountID;
		stLoginResp.nVipLevel = pGetRoleInfoResp->nVipLevel;
		stLoginResp.nUserLevel = pGetRoleInfoResp->nUserLevel;
		stLoginResp.nOnlineTime = pGetRoleInfoResp->nOnlineTime;
		stLoginResp.nLeftTime = pGetRoleInfoResp->nLeftTime;
		stLoginResp.nLeftMoney = pGetRoleInfoResp->nLeftMoney;
		stLoginResp.nNetType = pPlayer->GetConnInfo().nNetType;
		stLoginResp.nSelfSetting = pGetRoleInfoResp->nUserInfoSelfSet;
		stLoginResp.nRmbUserType = pGetRoleInfoResp->nRmbUserType;
		stLoginResp.nAccountSafeStatus = pGetRoleInfoResp->nAccountSafeType;
		stLoginResp.nMagnateLevel = pGetRoleInfoResp->nMagnatelevel;
		if(pGetRoleInfoResp->nNextlevelMoney < pGetRoleInfoResp->nConsume )
		{
			stLoginResp.nNextLevelNeedMoney = 0;
		}
		else
		{
			stLoginResp.nNextLevelNeedMoney =uint32_t( pGetRoleInfoResp->nNextlevelMoney - pGetRoleInfoResp->nConsume );
		}
		stLoginResp.nSelf_business = pGetRoleInfoResp->uRoleIdentity;

		pPlayer->SetLoginTime(nCurTime);

		pPlayer->SetLastVersion(pGetRoleInfoResp->nLastVersion);
		pPlayer->SetPlayerGender(pGetRoleInfoResp->nGender);
		pPlayer->SetRoleName(pGetRoleInfoResp->strRoleName.GetString());
		pPlayer->SetAccountID(pGetRoleInfoResp->nAccountID);
		pPlayer->SetVipLevel(pGetRoleInfoResp->nVipLevel);
		pPlayer->SetUserLevel(pGetRoleInfoResp->nUserLevel);
		pPlayer->SetOnlineTime(pGetRoleInfoResp->nOnlineTime);
		pPlayer->SetLeftMoney(pGetRoleInfoResp->nLeftMoney);
		pPlayer->SetExperience(pGetRoleInfoResp->nExperience);
		pPlayer->SetIdentityType(pGetRoleInfoResp->uRoleIdentity);
		pPlayer->SetMagnateLevel( pGetRoleInfoResp->nMagnatelevel );
		pPlayer->SetConsume( pGetRoleInfoResp->nConsume );
		pPlayer->SetNextLevelMoney( pGetRoleInfoResp->nNextlevelMoney );

		/*锟斤拷锟铰硷拷锟街段革拷值*/
		for(int32_t i = 0; i < pGetRoleInfoResp->nAdminCount; ++i)
		{
			pPlayer->SetAdminRoleRank(pGetRoleInfoResp->arrRoomIDS[i], pGetRoleInfoResp->arrRoleRank[i]);
		}

		COnlineNoti stNoti;
		stNoti.nRoleID = pMsgHead->nRoleID;
		stNoti.strRoleName = pGetRoleInfoResp->strRoleName;
		stNoti.nAccountID = pPlayer->GetAccountID();
		stNoti.nConnUin = pPlayer->GetConnInfo();
		stNoti.nVipLevel = pGetRoleInfoResp->nVipLevel;
		stNoti.nLeftMoney = pGetRoleInfoResp->nLeftMoney;
		stNoti.nLoginTime = nCurTime;
		stNoti.nMagnatelevel = pGetRoleInfoResp->nMagnatelevel;
		stNoti.nConsume = pGetRoleInfoResp->nConsume;
		stNoti.nNextlevelMoney = pGetRoleInfoResp->nNextlevelMoney;
		stNoti.nGender = pGetRoleInfoResp->nGender;

		SendOnlineNotiToServer(&stNoti, pPlayer, pData->m_nOptionLen, pData->m_arrOptionData);

		CSetLoginInfoNoti stSetLoginInfoNoti;
		stSetLoginInfoNoti.nRoleID = pMsgHead->nRoleID;
		stSetLoginInfoNoti.nLoginTime = nCurTime;
		stSetLoginInfoNoti.nIPAddr = pPlayer->GetConnInfo().nAddress;
		stSetLoginInfoNoti.nOSVersion = pData->stLoginReq.nOSVersion;
		stSetLoginInfoNoti.strMacAddr = pData->stLoginReq.strMacAddr;
		if(pData->stLoginReq.nClientVersion >= 1000500)
		{
			stSetLoginInfoNoti.nChannel = pData->stLoginReq.nChannel;
		}else
		{
			stSetLoginInfoNoti.nChannel = 0;
		}

		SendMessageNotifyToServer(MSGID_HSDB_SETLOGININFO_NOTI, &stSetLoginInfoNoti, enmInvalidRoomID,
				enmTransType_ByKey, pMsgHead->nRoleID, enmEntityType_DBProxy, pMsgHead->nRoleID);

		//锟斤拷录IP锟斤拷RoleID锟斤拷映锟斤拷
		g_IpMacMapMgt.AddIpRoleIDItem(pPlayer->GetConnInfo().nAddress, pMsgHead->nRoleID);
		//锟斤拷录MAC锟斤拷RoleID锟斤拷映锟斤拷
		g_IpMacMapMgt.AddMacRoleIDItem(pPlayer->GetPlayerMacAddr(), pMsgHead->nRoleID);

	}

	WRITE_DEBUG_LOG("send player login event response to client!{nRoleID=%d}\n", pMsgHead->nRoleID);
	SendMessageResponse(MSGID_HSCL_LOGIN_RESP, &pData->stMsgHead, &stLoginResp, enmTransType_P2P,
			pData->m_nOptionLen, pData->m_arrOptionData);

	return S_OK;
}

int32_t CLoginMessageEvent::GetRoleInfoTimeout(CFrameSession* pSession)
{
	CGetRoleInfoSessionData * pData =(CGetRoleInfoSessionData *)(pSession->GetSessionData());
	if(NULL == pData)
	{
		WRITE_ERROR_LOG("null pointer:get role info session timeout, and get data failed!"
				"{nSessionIndex=%d}\n", pSession->GetSessionIndex());
		return S_OK;
	}
	WRITE_ERROR_LOG("get role info session timeout!{nMessageID=0x%08x, nRoleID=%d, nSessionIndex=%d}\n",
			pData->stMsgHead.nMessageID, pData->stMsgHead.nRoleID, pSession->GetSessionIndex());

	CPlayer *pPlayer = NULL;
	int32_t ret = g_PlayerMgt.GetPlayer(pData->stMsgHead.nRoleID, pPlayer);
	if(0 <= ret && pPlayer != NULL)
	{
		//锟斤拷锟斤拷锟斤拷时注锟斤拷锟斤拷锟�
		WRITE_DEBUG_LOG("destroy role object while getting role info session timeout!{nRoleID=%d, nSessionIndex=%d}\n",
				pData->stMsgHead.nRoleID, pSession->GetSessionIndex());
		g_PlayerMgt.DestoryPlayer(pData->stMsgHead.nRoleID);
		uint32_t nCount = g_PlayerMgt.GetRealPlayerCount();
		if(nCount >= 1)
		{
			g_PlayerMgt.SetRealPlayerCount(--nCount);
		}
	}

	CLoginResp stLoginResp;
	stLoginResp.nResult = enmLoginResult_Timeout;
	stLoginResp.strFailReason = "time out";
	SendMessageResponse(MSGID_HSCL_LOGIN_RESP, &pData->stMsgHead, &stLoginResp, enmTransType_P2P,
			pData->m_nOptionLen, pData->m_arrOptionData);

	return S_OK;
}

int32_t CLoginMessageEvent::SendOnlineNotiToServer(COnlineNoti* pNoti, CPlayer* pPlayer,
		const uint16_t nOptionLen, const void *pOptionData)
{
	RoleID nRoleID = pPlayer->GetRoleID();
	WRITE_DEBUG_LOG("send player login event notify to other servers!{nRoleID=%d}\n", nRoleID);
	/*锟斤拷锟斤拷online通知锟斤拷sysinfo server*/
//	SendMessageNotifyToServer(MSGID_HSTA_ONLINE_NOTI, pNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, nRoleID, enmEntityType_SysInfo);
//
//	SendMessageNotifyToServer(MSGID_HSTA_ONLINE_NOTI, pNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, nRoleID, enmEntityType_Item);
//	SendMessageNotifyToServer(MSGID_HSTA_ONLINE_NOTI, pNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, nRoleID, enmEntityType_Media);
//	SendMessageNotifyToServer(MSGID_HSTA_ONLINE_NOTI, pNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, nRoleID, enmEntityType_Room);
//	SendMessageNotifyToServer(MSGID_HSTA_ONLINE_NOTI, pNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, nRoleID, enmEntityType_RoomDispatcher);
//
//	SendMessageNotifyToServer(MSGID_HSTA_ONLINE_NOTI, pNoti, enmInvalidRoomID,
//			enmTransType_P2P, nRoleID, enmEntityType_Tunnel, pPlayer->GetConnInfo().nServerID);
	SendMessageNotifyToServer(MSGID_HSTA_ONLINE_NOTI, pNoti, enmInvalidRoomID,
					enmTransType_Broadcast, nRoleID, enmEntityType_None, 0, nOptionLen,
					(const char*)pOptionData);

	return S_OK;
}
FRAME_HALLSERVER_NAMESPACE_END

