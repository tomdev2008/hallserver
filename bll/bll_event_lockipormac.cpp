/*
 * bll_event_lockipormac.cpp
 *
 *  Created on: 2012-7-4
 *      Author: zhurw
 */
#include <arpa/inet.h>
#include "bll_event_lockipormac.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CLockIPOrMacEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	// check the server status
	//	测试暂时注释代码
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		return E_NULLPOINTER;
	}

	//	int32_t nRet = S_OK;
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}
	switch(pMsgHead->nMessageID)
	{
		case MSGID_WEBOT_LOCK_NOTICE:
		{
//			WRITE_DEBUG_LOG("got lock player, ip or mac req{nMessageID=0x%08x}\n", MSGID_WEBOT_LOCK_NOTICE);
			CLockNotice* pLockNotice = dynamic_cast<CLockNotice*>(pMsgBody);
			if(pLockNotice == NULL)
			{
				WRITE_ERROR_LOG("null pointer:pMsgBody transform to CLockNotice class failed!{nMessageID=0x%08x, nRoleID=%d}\n",
						MSGID_WEBOT_LOCK_NOTICE, pMsgHead->nRoleID);
				return E_NULLPOINTER;
			}
			switch(pLockNotice->nLockType)
			{
				case enmLockType_IP:
				OnLockIpEvent(pMsgHead, pLockNotice, nOptionLen, pOptionData);
				break;
				case enmLockType_Mac:
				OnLockMacEvent(pMsgHead, pLockNotice, nOptionLen, pOptionData);
				break;
				case enmLockType_Player:
				OnLockPlayerEvent(pMsgHead, pLockNotice, nOptionLen, pOptionData);
				break;
				default:
				WRITE_ERROR_LOG("unknown lock type{nLockType=%d}\n", pLockNotice->nLockType);
				break;
			}
		}
		break;
		default:
		WRITE_ERROR_LOG("got wrong message id for lock ip or mac message event!{nMessageID=0x%08x}\n", pMsgHead->nMessageID);
		break;
	}
	return S_OK;
}

int32_t CLockIPOrMacEvent::OnLockIpEvent(MessageHeadSS * pMsgHead, CLockNotice* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	WRITE_DEBUG_LOG("got lock ip req{nLockCount=%d}\n", pMsgBody->nLockCount);
	for(int i = 0; i < pMsgBody->nLockCount; i++)
	{
		uint32_t nIpAddr = inet_addr(pMsgBody->arrContent[i].GetString());
//		int64_t nStartTime = atol(pMsgBody->arrStartTime[i].GetString());
//		int64_t nEndTime = atol(pMsgBody->arrEndTime[i].GetString());
		CDateTime dStartTime = CDateTime(pMsgBody->arrStartTime[i].GetString());
		CDateTime dEndTime = CDateTime(pMsgBody->arrEndTime[i].GetString());
		uint16_t nKickDays = (dEndTime.Seconds() - dStartTime.Seconds()) / SECOND_PER_DAY;
//		uint16_t nKickDays = (nEndTime - nStartTime) / SECOND_PER_DAY;
//		CDateTime nUnlockTime(nEndTime * US_PER_SECOND);
		char strEndtime[enmMaxDateTimeStringLength];
		memset(strEndtime, '\0', enmMaxDateTimeStringLength);
		dEndTime.ToDateString(strEndtime);
		CString<enmMaxDateTimeStringLength> strLockIPEndDate(strEndtime);

		RoleID arrRoleID[MAXROLEIDPERIP];
		memset(arrRoleID, '\0', MAXROLEIDPERIP * sizeof(RoleID));
		int32_t nCount = 0;
		g_IpMacMapMgt.GetIPRoleID(nIpAddr, arrRoleID, nCount);
		for(int j = 0; j < nCount; j++)
		{
			KickOffPlayer(arrRoleID[j], nKickDays, enmKickReason_IPLock, strLockIPEndDate, pMsgBody->arrLockReason[i]);
		}
	}
	return S_OK;
}

int32_t CLockIPOrMacEvent::OnLockMacEvent(MessageHeadSS * pMsgHead, CLockNotice* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	WRITE_DEBUG_LOG("got lock mac req{nLockCount=%d}\n", pMsgBody->nLockCount);
	for(int i = 0; i < pMsgBody->nLockCount; i++)
	{
		uint64_t nMacAddr = g_IpMacMapMgt.MacCStringToLong(pMsgBody->arrContent[i].GetString());
//		int64_t nStartTime = atol(pMsgBody->arrStartTime[i].GetString());
//		int64_t nEndTime = atol(pMsgBody->arrEndTime[i].GetString());
		CDateTime dStartTime = CDateTime(pMsgBody->arrStartTime[i].GetString());
		CDateTime dEndTime = CDateTime(pMsgBody->arrEndTime[i].GetString());
		uint16_t nKickDays = (dEndTime.Seconds() - dStartTime.Seconds()) / SECOND_PER_DAY;
//		uint16_t nKickDays = (nEndTime - nStartTime) / SECOND_PER_DAY;
//		CDateTime nUnlockTime(nEndTime * US_PER_SECOND);
		char strEndtime[enmMaxDateTimeStringLength];
		memset(strEndtime, '\0', enmMaxDateTimeStringLength);
		dEndTime.ToDateString(strEndtime);
		CString<enmMaxDateTimeStringLength> strLockMacEndDate(strEndtime);

		RoleID arrRoleID[MAXROLEIDPERMAC];
		memset(arrRoleID, 0, MAXROLEIDPERMAC * sizeof(RoleID));
		int32_t nCount = 0;
		g_IpMacMapMgt.GetMacRoleID(nMacAddr, arrRoleID, nCount);
		for(int j = 0; j < nCount; j++)
		{
			KickOffPlayer(arrRoleID[j], nKickDays, enmKickReason_MacLock, strLockMacEndDate, pMsgBody->arrLockReason[i]);
		}
	}
	return S_OK;
}

int32_t CLockIPOrMacEvent::OnLockPlayerEvent(MessageHeadSS * pMsgHead, CLockNotice* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	WRITE_DEBUG_LOG("got lock player req{nLockCount=%d}\n", pMsgBody->nLockCount);
	for(int i = 0; i < pMsgBody->nLockCount; i++)
	{
//		int64_t nStartTime = atol(pMsgBody->arrStartTime[i].GetString());
//		int64_t nEndTime = atol(pMsgBody->arrEndTime[i].GetString());
		CDateTime dStartTime = CDateTime(pMsgBody->arrStartTime[i].GetString());
		CDateTime dEndTime = CDateTime(pMsgBody->arrEndTime[i].GetString());
		uint16_t nKickDays = (dEndTime.Seconds() - dStartTime.Seconds()) / SECOND_PER_DAY;
//		CDateTime nUnlockTime(nEndTime * US_PER_SECOND);
		char strEndtime[enmMaxDateTimeStringLength];
		memset(strEndtime, '\0', enmMaxDateTimeStringLength);
		dEndTime.ToDateString(strEndtime);
		CString<enmMaxDateTimeStringLength> strLockPlayerEndDate(strEndtime);
		KickOffPlayer(pMsgBody->arrRoleID[i], nKickDays, enmKickReason_PlayerLock, strLockPlayerEndDate, pMsgBody->arrLockReason[i]);
	}
	return S_OK;
}

int32_t CLockIPOrMacEvent::KickOffPlayer(RoleID nRoleID, uint16_t nKickDays, KickReason nKickReason,
		CString<enmMaxDateTimeStringLength>& strLockEndDate, CString<MaxLockIPReasonLength>& strLockReason)
{
	CPlayer *pPlayer = NULL;
	int32_t nRet;
	nRet = g_PlayerMgt.GetPlayer(nRoleID, pPlayer);
	//测试暂时注释代码
	if(0 > nRet || pPlayer == NULL)
	{
		WRITE_DEBUG_LOG("lock ip or mac request, but failed to get player object!{nRoleID=%d}\n", nRoleID);
		return nRet;
	}
	CKickUserNoti stKickNoti;
	stKickNoti.nResult = nKickReason;
	stKickNoti.strReason = strLockReason;
	stKickNoti.nKickDays = nKickDays;
	stKickNoti.strLockEndDate = strLockEndDate;

	//向客户端发送踢出通知
	SendMessageNotifyToClient(MSGID_HSCL_KICKUSER_NOTI, &stKickNoti, enmInvalidRoomID, enmBroadcastType_ExpectPlayr, nRoleID);

	//向服务器发送下线通知

	COfflineNoti stOfflineNoti;
	stOfflineNoti.nRoleID = nRoleID;
	stOfflineNoti.nLogoutTime = CDateTime::CurrentDateTime().Seconds();
	stOfflineNoti.nConnUin = pPlayer->GetConnInfo();
	stOfflineNoti.nLogoutReason = enmLogoutReason_Unknown;

	CSetLogoutInfoNoti stSetLogoutInfoNoti;
	stSetLogoutInfoNoti.nRoleID = nRoleID;
	stSetLogoutInfoNoti.nLogoutTime = CDateTime::CurrentDateTime().Seconds();

	SendMessageNotifyToServer(MSGID_HSDB_SETLOGOUTINFO_NOTI, &stSetLogoutInfoNoti, enmInvalidRoomID,
			enmTransType_ByKey, nRoleID, enmEntityType_DBProxy, nRoleID);

//	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stOfflineNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, nRoleID, enmEntityType_Item);
//	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stOfflineNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, nRoleID, enmEntityType_Media);
//	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stOfflineNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, nRoleID, enmEntityType_Room);
//	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stOfflineNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, nRoleID, enmEntityType_RoomDispatcher);

	/*发送离线通知给sysinfo server*/
//	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stOfflineNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, nRoleID, enmEntityType_SysInfo);
	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stOfflineNoti, enmInvalidRoomID,
			enmTransType_Broadcast, nRoleID, enmEntityType_None);

	//delete roleid from ipmap
	g_IpMacMapMgt.DelIpRoleIDItem(pPlayer->GetConnInfo().nAddress, nRoleID);
	//delete roleid from macmap
	g_IpMacMapMgt.DelMacRoleIDItem(pPlayer->GetPlayerMacAddr(), nRoleID);
	WRITE_DEBUG_LOG("lock request, locked succeed!{nRoleID=%d}\n", nRoleID);

	int32_t nEnterRoomCount = pPlayer->GetCurEnterRoomCount();
	WRITE_DEBUG_LOG("kick player:player current entered room info:{nRoleID=%d, nEnterRoomCount=%d}\n",
			nRoleID, nEnterRoomCount);

	if(nEnterRoomCount != 0)
	{
		RoomID arrRoomID[MaxEnterRoomCount];
		memset(arrRoomID, 0, sizeof(arrRoomID));
		int32_t nGetCurRoomCount = 0;
		pPlayer->GetAllEnterRoom(arrRoomID, MaxEnterRoomCount, nGetCurRoomCount);
		for(int32_t i = 0; i < nGetCurRoomCount; ++i)
		{
			g_RoomMgt.DeletePlayer(arrRoomID[i], nRoleID);
			if(pPlayer->GetAdminRoleRank(arrRoomID[i]) == enmRoleRank_TempAdmin)
			{
				pPlayer->SetAdminRoleRank(arrRoomID[i], enmRoleRank_None);
				CRoom* pRoom = NULL;
				nRet = g_RoomMgt.GetRoom(arrRoomID[i], pRoom);
				if(nRet < 0 || pRoom == NULL)
				{
					WRITE_ERROR_LOG("logout:get room error!{nRoomID=%d}\n",arrRoomID[i]);
					return nRet;
				}
				int32_t nTempAdminCount = pRoom->GetTempAdminCount();
				if(nTempAdminCount >= 1)
				{
					pRoom->SetTempAdminCount(--nTempAdminCount);
				}
			}
			WRITE_DEBUG_LOG("player exit room!{nRoleID=%d, nRoomID=%u}\n",
					nRoleID, arrRoomID[i]);
		}
	}

	g_PlayerMgt.DestoryPlayer(nRoleID);
	uint32_t nCount = g_PlayerMgt.GetRealPlayerCount();
	if(nCount >= 1)
	{
		g_PlayerMgt.SetRealPlayerCount(--nCount);
	}
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
