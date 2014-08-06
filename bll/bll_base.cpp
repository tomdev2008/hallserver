/*
 * bll_base.cpp
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#include "bll_base.h"
#include "public_typedef.h"
#include "ctl/server_datacenter.h"
#include "dal/to_server_message.h"
#include "ctl/update_data_ctl.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

#define MaxPlayerCountUpdate   1000
#define MaxRoomCountUpdate     100

static ConnUin g_arrConnUin[MaxUserCountPerRoom];

/*
 发送请求消息
 */
void CBllBase::SendMessageRequest(uint32_t nMsgID, IMsgBody* pMsgBody, const RoomID nRoomID, TransType nTransType,
		RoleID nRoleID, EntityType nDestType, const int32_t nDestID, const SessionIndex nSessionIndex,
		const uint16_t nOptionLen, const char *pOptionData, const char *szDumpContent)
{
	MessageHeadSS stSendMessage;
//	memset(&stSendMessage, 0, sizeof(stSendMessage));
	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nMessageType = enmMessageType_Request;
	stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
	stSendMessage.nDestType = nDestType;
	stSendMessage.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
	stSendMessage.nDestID = nDestID;
	stSendMessage.nRoleID = nRoleID;
	stSendMessage.nSequence = 0;
	stSendMessage.nTransType = nTransType;
	stSendMessage.nSessionIndex = nSessionIndex;
	stSendMessage.nRoomID = nRoomID;
	stSendMessage.nZoneID = 0;
	stSendMessage.nRouterIndex = 0;

	g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
	DumpMessage(szDumpContent, &stSendMessage, pMsgBody, nOptionLen, pOptionData);
	UpdatePlayerDataInfo();
	UpdateRoomDataInfo();
}
/*
 发送相应消息
 */
void CBllBase::SendMessageResponse(uint32_t nMsgID,MessageHeadSS * pMsgHead,IMsgBody* pMsgBody,
		TransType nTransType, const uint16_t nOptionLen, const char *pOptionData,const char *szDumpContent)
{
	MessageHeadSS stSendMessage;
//	memset(&stSendMessage, 0, sizeof(stSendMessage));

	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nMessageType = enmMessageType_Response;
	stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
	stSendMessage.nDestType = pMsgHead->nSourceType;
	stSendMessage.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
	stSendMessage.nDestID = pMsgHead->nSourceID;
	stSendMessage.nRoleID = pMsgHead->nRoleID;
	stSendMessage.nSequence = pMsgHead->nSequence;
	stSendMessage.nTransType = nTransType;
	stSendMessage.nSessionIndex = pMsgHead->nSessionIndex;
	stSendMessage.nRoomID = pMsgHead->nRoomID;
	stSendMessage.nZoneID = pMsgHead->nZoneID;
	if(stSendMessage.nDestType == enmEntityType_Tunnel)
	{
		ConnUin stInfo;
		uint32_t offset = 0;
		int32_t ret = stInfo.MessageDecode((uint8_t *)pOptionData, (uint32_t)nOptionLen, offset);
		if(ret < 0)
		{
			WRITE_ERROR_LOG("send message response:decode connuin failed!{ret=0x%08x, nRoleID=%d}\n", ret, pMsgHead->nRoleID);
			return;
		}
		stSendMessage.nRouterIndex = 0;//g_FrameConfigMgt.GetRouterIndex(stInfo.nRouterAddress, stInfo.nRouterPort);
	}
	else
	{
		stSendMessage.nRouterIndex = pMsgHead->nRouterIndex;
	}

	g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
	DumpMessage(szDumpContent,&stSendMessage, pMsgBody, nOptionLen, pOptionData);
	UpdatePlayerDataInfo();
	UpdateRoomDataInfo();
}

void CBllBase::SendMessageNotifyToServer(uint32_t nMsgID,IMsgBody* pMsgBody,const RoomID nRoomID,const TransType nTransType,
		const RoleID nRoleID,const EntityType nDestType,const int32_t nDestID,
		const uint16_t nOptionLen, const char *pOptionData,const char *szDumpContent)
{
	MessageHeadSS stSendMessage;
//	memset(&stSendMessage, 0, sizeof(stSendMessage));
	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nMessageType = enmMessageType_Notify;
	stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
	stSendMessage.nDestType = nDestType;
	stSendMessage.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
	stSendMessage.nDestID = nDestID;
	stSendMessage.nRoleID = nRoleID;
	stSendMessage.nSequence = 0;
	stSendMessage.nTransType = nTransType;
	stSendMessage.nSessionIndex = enmInvalidSessionIndex;
	stSendMessage.nRoomID = nRoomID;
	if(nMsgID == MSGID_HSTA_ONLINE_NOTI)//上线通知tunnel创建player缓存
	{
		COnlineNoti *pNoti = dynamic_cast<COnlineNoti *>(pMsgBody);
		if(pNoti == NULL)
		{
			WRITE_ERROR_LOG("send message to server:pointer transfer to COnlineNoti failed");
			return;
		}
		stSendMessage.nZoneID = 0;//g_FrameConfigMgt.GetZoneID(enmRouterType_Default);
		stSendMessage.nRouterIndex = 0;//g_FrameConfigMgt.GetRouterIndex(pNoti->nConnUin.nRouterAddress, pNoti->nConnUin.nRouterPort);
	}
	else if(nMsgID == MSGID_HSTA_OFFLINE_NOTI)//下线通知tunnel销毁player缓存
	{
		COfflineNoti *pNoti = dynamic_cast<COfflineNoti *>(pMsgBody);
		if(pNoti == NULL)
		{
			WRITE_ERROR_LOG("send message to server:pointer transfer to COfflineNoti failed");
			return;
		}
		stSendMessage.nZoneID = 0;//g_FrameConfigMgt.GetZoneID(enmRouterType_Default);
		stSendMessage.nRouterIndex = 0;//g_FrameConfigMgt.GetRouterIndex(pNoti->nConnUin.nRouterAddress, pNoti->nConnUin.nRouterPort);
	}
	else
	{
		stSendMessage.nZoneID = 0;//g_FrameConfigMgt.GetZoneID(enmRouterType_Sync);
		stSendMessage.nRouterIndex = 0;
	}

	g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
	DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nOptionLen, pOptionData);
	UpdatePlayerDataInfo();
	UpdateRoomDataInfo();
}

/*
 发送通知消息
 */
void CBllBase::SendMessageNotify(const RoleID nRoleID, IMsgBody* pMsgBody,uint32_t nMsgID,
		const char *szDumpContent, const RoomID nRoomID, const uint16_t nOptionLen, const char *pOptionData)
{
	MessageHeadSS stSendMessage;
//	memset(&stSendMessage, 0, sizeof(stSendMessage));
	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nMessageType = enmMessageType_Notify;
	stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
	stSendMessage.nDestType = enmEntityType_Hall;
	stSendMessage.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
	stSendMessage.nDestID = 0;
	stSendMessage.nRoleID = nRoleID;
	stSendMessage.nSequence = 0;
	stSendMessage.nTransType = enmTransType_ByKey;
	stSendMessage.nSessionIndex = enmInvalidSessionIndex;
	stSendMessage.nRoomID = nRoomID;
	stSendMessage.nZoneID = 0;
	stSendMessage.nRouterIndex = 0;

	g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
	DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nOptionLen, pOptionData);
	UpdatePlayerDataInfo();
	UpdateRoomDataInfo();
}

//发送广播消息给房间内所有玩家
void CBllBase::SendMessageNotifyToClient(uint32_t nMsgID, IMsgBody* pMsgBody, RoomID nRoomID, BroadcastType nType,
		RoleID nRoleID, const uint16_t nOptionLen,const char *pOptionData, const char *szDumpContent)
{
	MessageHeadSS stSendMessage;
//	memset(&stSendMessage, 0, sizeof(stSendMessage));
	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nMessageType = enmMessageType_Notify;
	stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
	stSendMessage.nDestType = enmEntityType_Tunnel;
	stSendMessage.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
	//	stSendMessage.nDestID = 0;
	stSendMessage.nSequence = 0;
	stSendMessage.nTransType = enmTransType_P2P;
	stSendMessage.nSessionIndex = enmInvalidSessionIndex;
	stSendMessage.nRoomID = nRoomID;
	stSendMessage.nRoleID = enmInvalidRoleID;
	stSendMessage.nZoneID = 0;//g_FrameConfigMgt.GetZoneID(enmRouterType_Default);

	int32_t ret = S_OK;
	if(enmBroadcastType_ExpectPlayr == nType)
	{
		CPlayer *pPlayer = NULL;
		PlayerIndex nPlayerIndex = enmInvalidPlayerIndex;
		ret = g_PlayerMgt.GetPlayer(nRoleID, pPlayer);
		if(ret < 0)
		{
			WRITE_ERROR_LOG("get expect player object error!{RoomID=%d, ret=0x%08x}\n", nRoomID, ret);
			return;
		}

		stSendMessage.nRoleID = nRoleID;
		stSendMessage.nDestID = pPlayer->GetConnInfo().nServerID;
		stSendMessage.nRouterIndex = 0;//g_FrameConfigMgt.GetRouterIndex(pPlayer->GetConnInfo().nRouterAddress, pPlayer->GetConnInfo().nRouterPort);

		g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
		DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nOptionLen, pOptionData);
	}
	else if(enmBroadcastType_ExceptPlayr == nType)
	{
		stSendMessage.nTransType = enmTransType_Broadcast;
		stSendMessage.nDestID = enmInvalidServerID;

		stSendMessage.nRoleID = nRoleID;
		stSendMessage.nRouterIndex = 0;

		g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
		DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nOptionLen, pOptionData);
	}
	else if(enmBroadcastType_All == nType)
	{
		stSendMessage.nTransType = enmTransType_Broadcast;
		stSendMessage.nDestID = enmInvalidServerID;
		stSendMessage.nRouterIndex = 0;

		g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
		DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nOptionLen, pOptionData);
	}

	UpdatePlayerDataInfo();
	UpdateRoomDataInfo();
}

void CBllBase::SendMessageNotify(const RoleID nRoleID, const EntityType nDestType ,
		const TransType nTransType, IMsgBody* pMsgBody,uint32_t nMsgID,const char *szDumpContent,
		const RoomID nRoomID, const uint16_t nOptionLen, const char *pOptionData)
{
	MessageHeadSS stSendMessage;
//	memset(&stSendMessage, 0, sizeof(stSendMessage));
	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nMessageType = enmMessageType_Notify;
	stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
	stSendMessage.nDestType = nDestType;
	stSendMessage.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
	stSendMessage.nDestID = 0;
	stSendMessage.nRoleID = nRoleID;
	stSendMessage.nSequence = 0;
	stSendMessage.nTransType = nTransType;
	stSendMessage.nSessionIndex = enmInvalidSessionIndex;
	stSendMessage.nRoomID = nRoomID;
	stSendMessage.nZoneID = 0;
	stSendMessage.nRouterIndex = 0;

	g_Frame.PostMessage(&stSendMessage, pMsgBody, nOptionLen, pOptionData);
	DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nOptionLen, pOptionData);
	UpdatePlayerDataInfo();
	UpdateRoomDataInfo();
}

/*
 打印接受，发送的消息
 */
void CBllBase::DumpMessage(const char* szContent, MessageHeadSS *pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const char *pOptionData)
{
	uint32_t offset = 0;
	char szLog[enmMaxLogInfoLength];
	szLog[0] = 0;
	sprintf(szLog + offset, szContent);
	offset = (uint32_t)strlen(szLog);

	sprintf(szLog + offset, " MessageHead=");
	offset = (uint32_t)strlen(szLog);

	sprintf(szLog + offset, "{nMessageID=0x%08x, nMessageType=%d, nSourceType=%d, nDestType=%d, "
			"nSourceID=%d, nDestID=%d, nRoleID=%d, nSequence=%d, nTransType=%d, nSessionIndex=%d, "
			"nRoomID=%d, nZoneID=%d, nRouterIndex=%d, nOptionLen=%d}",
			pMsgHead->nMessageID, pMsgHead->nMessageType, pMsgHead->nSourceType,
			pMsgHead->nDestType, pMsgHead->nSourceID, pMsgHead->nDestID, pMsgHead->nRoleID,
			pMsgHead->nSequence, pMsgHead->nTransType, pMsgHead->nSessionIndex, pMsgHead->nRoomID,
			pMsgHead->nZoneID, pMsgHead->nRouterIndex, nOptionLen);
	offset = (uint32_t)strlen(szLog);

	sprintf(szLog + offset, " MessageBody=");
	offset = (uint32_t)strlen(szLog);
	pMsgBody->Dump(szLog, enmMaxLogInfoLength, offset);

	WRITE_NOTICE_LOG("%s\n", szLog);
}

//同步玩家数据
int32_t CBllBase::UpdatePlayerDataInfo()
{
	while(GET_UPDATEDATACTL_INSTANCE().IsHasPlayerNeedUpdate())
	{
		int32_t nNeedUpdatePlayerCount = 0;
		RoleID arrRoleID[MaxPlayerCountUpdate];
		GET_UPDATEDATACTL_INSTANCE().GetUpdateRoleID(arrRoleID,MaxPlayerCountUpdate,nNeedUpdatePlayerCount);

		for(int32_t i = 0;i<nNeedUpdatePlayerCount;i++)
		{
			CPlayer *pPlayer = NULL;
			int32_t ret = g_PlayerMgt.GetPlayer(arrRoleID[i], pPlayer);
			if(ret < 0 || pPlayer == NULL)
			{
				continue;
			}

			if(!pPlayer->IsHaveUpdateData() || pPlayer->IsRobot())
			{
				continue;
			}

			uint8_t *arrUpdateData[UpdateDataInfoTableSize];
			UpdateDataInfoTable arrUpdateTable[UpdateDataInfoTableSize];

			int32_t nUpdateCount = pPlayer->GetUpdateDataInfoTable(arrUpdateData, arrUpdateTable);
			MessageHeadSS stUpdateMsgHead;
			stUpdateMsgHead.nMessageID = MSGID_HSTS_PLAYERUPDATE_NOTI;
			stUpdateMsgHead.nMessageType = enmMessageType_Notify;
			stUpdateMsgHead.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
			stUpdateMsgHead.nDestType = enmEntityType_HallDataCenter;
			stUpdateMsgHead.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
			stUpdateMsgHead.nRoleID = arrRoleID[i];
			stUpdateMsgHead.nSequence = 0;
			stUpdateMsgHead.nTransType = enmTransType_Broadcast;
			stUpdateMsgHead.nSessionIndex = enmInvalidSessionIndex;
			stUpdateMsgHead.nRoomID = enmInvalidRoomID;
			stUpdateMsgHead.nZoneID = 0;//g_FrameConfigMgt.GetZoneID(enmRouterType_Default);
			stUpdateMsgHead.nRouterIndex = 0;//g_FrameConfigMgt.GetRouterIndex(pPlayer->GetConnInfo().nRouterAddress, pPlayer->GetConnInfo().nRouterPort);;

			CUpdateDataNoti stNoti;
			stNoti.nType = enmUpdateDataType_Player;
			stNoti.nValue = arrRoleID[i];
			stNoti.nStartPos = pPlayer->GetStartPos();
			stNoti.nEndPos = pPlayer->GetEndPos();
			for(int32_t i = 0; i < nUpdateCount; ++i)
			{
				stNoti.nDataOffset = arrUpdateTable[i].nOffset;
				stNoti.nDataSize = arrUpdateTable[i].nDataSize;
				memcpy(stNoti.arrData, arrUpdateData[i], stNoti.nDataSize);

				DumpMessage("update player data noti : ",&stUpdateMsgHead, &stNoti);
				g_Frame.PostMessage(&stUpdateMsgHead, &stNoti);
			}
		}
	}
	return S_OK;
}

//同步房间数据
int32_t CBllBase::UpdateRoomDataInfo(UpdateDataType nUpdateType)
{
	while(GET_UPDATEDATACTL_INSTANCE().IsHasRoomNeedUpdate())
	{
		int32_t nNeedUpdateRoomCount = 0;
		RoomID arrRoomID[MaxRoomCountUpdate];
		GET_UPDATEDATACTL_INSTANCE().GetUpdateRoomID(arrRoomID,MaxRoomCountUpdate,nNeedUpdateRoomCount);

		for(int32_t i = 0;i<nNeedUpdateRoomCount;i++)
		{
			//获取房间对象
			CRoom *pRoom = NULL;
			int32_t ret = g_RoomMgt.GetRoom(arrRoomID[i], pRoom);
			if(ret < 0 || pRoom == NULL)
			{
				continue;
			}
			if(!pRoom->IsHaveUpdateData())
			{
				continue;
			}

			// got a tunnel id, if haven't
			if(pRoom->GetTunnelServerID() == enmInvalidServerID)
			{
				int32_t nTunnelServerID = enmInvalidServerID;
				int32_t nRet = g_DataCenter.GetTunnelID(arrRoomID[i], nTunnelServerID);
				if(nRet < 0 || nTunnelServerID == enmInvalidServerID)
				{
					continue;
				}
				pRoom->SetTunnelServerID(nTunnelServerID);
			}

			uint8_t *arrUpdateData[UpdateDataInfoTableSize];
			UpdateDataInfoTable arrUpdateTable[UpdateDataInfoTableSize];

			int32_t nUpdateCount = pRoom->GetUpdateDataInfoTable(arrUpdateData, arrUpdateTable);
			MessageHeadSS stUpdateMsgHead;
			stUpdateMsgHead.nMessageID = MSGID_HSTS_PLAYERUPDATE_NOTI;
			stUpdateMsgHead.nMessageType = enmMessageType_Notify;
			stUpdateMsgHead.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
			stUpdateMsgHead.nDestType = enmEntityType_HallDataCenter;
			stUpdateMsgHead.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
			//stUpdateMsgHead.nDestID = pMsgHead->nRoleID;
			stUpdateMsgHead.nRoleID = enmInvalidRoleID;
			stUpdateMsgHead.nSequence = 0;
			stUpdateMsgHead.nTransType = enmTransType_Broadcast;
			stUpdateMsgHead.nSessionIndex = enmInvalidSessionIndex;
			stUpdateMsgHead.nRoomID = arrRoomID[i];
			stUpdateMsgHead.nZoneID = 0;//g_FrameConfigMgt.GetZoneID(enmRouterType_Sync);
			stUpdateMsgHead.nRouterIndex = 0;

			CUpdateDataNoti stNoti;
			stNoti.nType = nUpdateType;
			stNoti.nValue = (int32_t)(arrRoomID[i]);
			stNoti.nStartPos = pRoom->GetStartPos();
			stNoti.nEndPos = pRoom->GetEndPos();
			for(int32_t j = 0; j < nUpdateCount; ++j)
			{
				stNoti.nDataOffset = arrUpdateTable[j].nOffset;
				stNoti.nDataSize = arrUpdateTable[j].nDataSize;
				memcpy(stNoti.arrData, arrUpdateData[j], stNoti.nDataSize);

				DumpMessage("update room data noti : ",&stUpdateMsgHead, &stNoti);
				g_Frame.PostMessage(&stUpdateMsgHead, &stNoti);
			}
		}
	}
	return S_OK;
}

int64_t CBllBase::GetLeftSecondsBeforeTomorrow(CDateTime currentTime)
{
	CDateTime todayEndTime(currentTime.Year(), currentTime.Month(),
			currentTime.Day(), 23, 59, 59, 0);
	return todayEndTime.Seconds() + 1 - currentTime.Seconds();
}

FRAME_HALLSERVER_NAMESPACE_END

