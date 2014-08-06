/*
 * bll_event_rolelogout.cpp
 *
 *  Created on: 2011-12-10
 *      Author: jimm
 */

#include "bll_event_logout.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "tunnelserver_message_define.h"
#include "client_message_define.h"
#include "bll_event_checkcrossdaysplayer.h"
#include "ctl/ipmacmap_mgt.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN
//消息事件
int32_t CLogoutMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		return E_NULLPOINTER;
	}

	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n",
				pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	CLogoutReq *pLogoutMsg = dynamic_cast<CLogoutReq *>(pMsgBody);
	if(NULL == pLogoutMsg)
	{
		WRITE_ERROR_LOG("null pointer: pMsgBody transform to CLogoutReq class failed!{nMessageID=0x%08x, nRoleID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

	WRITE_INFO_LOG("receive logout request!{nRoleID=%d, nLogoutReason=%d}\n",
			pMsgHead->nRoleID, pLogoutMsg->nLogoutReason);


	//退出玩家进入过的房间
	CPlayer* pPlayer = NULL;
	int32_t ret = g_PlayerMgt.GetPlayer(pMsgHead->nRoleID, pPlayer);
	if(0 > ret || pPlayer == NULL)
	{
		WRITE_ERROR_LOG("logout error, cannot find player object!{ret=0x%08x, nRoleID=%d}\n",ret, pMsgHead->nRoleID);
		return ret;
	}
	else
	{
//		ConnUin stCurUserInfo = pPlayer->GetConnInfo();
//		uint32_t nCurUserOffset = 0;
//		uint8_t arrCurUserOptionData[MaxOptionDataSize];
//		ret = stCurUserInfo.MessageEncode(arrCurUserOptionData, MaxOptionDataSize, nCurUserOffset);
//		if(ret < 0)
//		{
//			WRITE_ERROR_LOG("kick user error, encode connuin failed!{ret=0x%08x, nRoleID=%d}\n", ret, pMsgHead->nRoleID);
//			return ret;
//		}
//		else
//		{
//			if(memcmp(arrCurUserOptionData, pOptionData, nOptionLen) != 0)
//			{
//				WRITE_NOTICE_LOG("player req logout, bug conn_uin is not equal!{nRoleID=%d, "
//						"nCurAddress=%s, nCurPort=%d}\n",
//						pMsgHead->nRoleID, inet_ntoa_f(pPlayer->GetConnInfo().nAddress), pPlayer->GetConnInfo().nPort);
//				return ret;
//			}
//		}

		ConnUin stConnUin ;
		uint32_t offset = 0;
		stConnUin.MessageDecode((uint8_t *)pOptionData,nOptionLen,offset);

		ConnUin stCurUserInfo = pPlayer->GetConnInfo();

		if(!stCurUserInfo.Compare(&stConnUin))
		{
			WRITE_NOTICE_LOG("player req logout, bug conn_uin is not equal!{nRoleID=%d, "
					"nCurAddress=%s, nCurPort=%d}\n",
					pMsgHead->nRoleID, inet_ntoa_f(pPlayer->GetConnInfo().nAddress), pPlayer->GetConnInfo().nPort);
			return ret;
		}

//		if(pLogoutMsg->nLogoutReason == enmClientCloseReason_Tunnel_RecvStartRequestWhenUse)
//		{
//			//把前面登陆的给踢掉
//			CKickUserNoti stKickNoti;
//			stKickNoti.nResult = enmKickReason_Relogin;
//			stKickNoti.strReason = "already login!";
//			//向客户端发送踢出通知
//			SendMessageNotifyToClient(MSGID_HSCL_KICKUSER_NOTI, &stKickNoti, 0, enmBroadcastType_ExpectPlayr,
//					pMsgHead->nRoleID, nCurUserOffset, (char *)arrCurUserOptionData);
//		}

		CDateTime currentTime = CDateTime::CurrentDateTime();
		if(GET_CHECKCROSSDAYSEVENT_INSTANCE().GetChecked().date.Date() < currentTime.Date() ||
				!GET_CHECKCROSSDAYSEVENT_INSTANCE().GetChecked().bChecked)
		{
			int64_t nLoginTime = pPlayer->GetLoginTime();
			CDateTime loginTime(nLoginTime * US_PER_SECOND);
			nLoginTime = loginTime.Seconds();
			if(loginTime.Date() < currentTime.Date()) // cross days
			{
				CDateTime todayBeginTime(currentTime.Year(), currentTime.Month(),
					currentTime.Day(), 0, 0, 0, 0);
				nLoginTime = todayBeginTime.Seconds();
			}

			STATISTIC_WRITE_INFO_LOG("log_notice, {nRoleID=%d, nOnlinetime=%d}\n",
				pMsgHead->nRoleID, (currentTime.Seconds() - nLoginTime)/SECOND_PER_MINUTE);
		}


		int32_t nEnterRoomCount = pPlayer->GetCurEnterRoomCount();
		WRITE_DEBUG_LOG("player current entered room info:{nRoleID=%d, nEnterRoomCount=%d}\n",
				pMsgHead->nRoleID, nEnterRoomCount);

		if(nEnterRoomCount != 0)
		{
			RoomID arrRoomID[MaxEnterRoomCount];
			memset(arrRoomID, 0, sizeof(arrRoomID));
			int32_t nGetCurRoomCount = 0;
			pPlayer->GetAllEnterRoom(arrRoomID, MaxEnterRoomCount, nGetCurRoomCount);
			for(int32_t i = 0; i < nGetCurRoomCount; ++i)
			{
				g_RoomMgt.DeletePlayer(arrRoomID[i], pMsgHead->nRoleID);
				if(pPlayer->GetAdminRoleRank(arrRoomID[i]) == enmRoleRank_TempAdmin)
				{
					pPlayer->SetAdminRoleRank(arrRoomID[i], enmRoleRank_None);
					CRoom* pRoom = NULL;
					ret = g_RoomMgt.GetRoom(arrRoomID[i], pRoom);
					if(ret < 0 || pRoom == NULL)
					{
						WRITE_ERROR_LOG("logout:get room error!{nRoomID=%d}\n",arrRoomID[i]);
						return ret;
					}
					int32_t nTempAdminCount = pRoom->GetTempAdminCount();
					if(nTempAdminCount >= 1)
					{
						pRoom->SetTempAdminCount(--nTempAdminCount);
					}
				}
				WRITE_DEBUG_LOG("player exit room!{nRoleID=%d, nRoomID=%u}\n",
						pMsgHead->nRoleID, arrRoomID[i]);
			}
		}

		//删除IP和RoleID的映射
		g_IpMacMapMgt.DelIpRoleIDItem(pPlayer->GetConnInfo().nAddress, pMsgHead->nRoleID);
		//删除MAC和RoleID的映射
		g_IpMacMapMgt.DelMacRoleIDItem(pPlayer->GetPlayerMacAddr(), pMsgHead->nRoleID);

	}

	WRITE_DEBUG_LOG("send player logout notify to other servers!{nRoleID=%d}\n", pMsgHead->nRoleID);

	int64_t nCurTime = CDateTime::CurrentDateTime().Seconds();
	COfflineNoti stNoti;
	stNoti.nRoleID = pMsgHead->nRoleID;
	stNoti.nLogoutTime = nCurTime;
	stNoti.nLogoutReason = pLogoutMsg->nLogoutReason;
	stNoti.nConnUin = pPlayer->GetConnInfo();

//	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_Item);
//	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_Media);
//	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_Room);
//	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_RoomDispatcher);
//	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stNoti, enmInvalidRoomID,
//			enmTransType_P2P, pMsgHead->nRoleID, enmEntityType_Tunnel, pPlayer->GetConnInfo().nServerID);
	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stNoti, enmInvalidRoomID,
					enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_None);

	if(pPlayer->GetVipLevel() != enmVipLevel_NONE)
	{
		CSetLogoutInfoNoti stSetLogoutInfoNoti;
		stSetLogoutInfoNoti.nRoleID = pMsgHead->nRoleID;
		stSetLogoutInfoNoti.nLogoutTime = nCurTime;
		SendMessageNotifyToServer(MSGID_HSDB_SETLOGOUTINFO_NOTI, &stSetLogoutInfoNoti, enmInvalidRoomID,
				enmTransType_ByKey, pMsgHead->nRoleID, enmEntityType_DBProxy, pMsgHead->nRoleID);
	}

	/*发送离线消息给sysinfo server*/
//	SendMessageNotifyToServer(MSGID_HSTA_OFFLINE_NOTI, &stNoti, enmInvalidRoomID,
//			enmTransType_Broadcast, pMsgHead->nRoleID, enmEntityType_SysInfo);

	ret = g_PlayerMgt.DestoryPlayer(pMsgHead->nRoleID);
	uint32_t nCount = g_PlayerMgt.GetRealPlayerCount();
	if(nCount >= 1)
	{
		g_PlayerMgt.SetRealPlayerCount(--nCount);
	}

	if(ret < 0)
	{
		WRITE_ERROR_LOG("logout error, failed to destroy player object!{ret=0x%08x, nRoleID=%d}\n",
				ret, pMsgHead->nRoleID);
		return ret;
	}

	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
