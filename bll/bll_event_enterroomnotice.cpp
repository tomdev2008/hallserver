/*
 * bll_event_enterroomnotice.cpp
 *
 *  Created on: 2012-2-15
 *      Author: jimm
 */

#include "bll_event_enterroomnotice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "dal/from_server_message.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CEnterRoomNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	// check the server status
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		return E_NULLPOINTER;
	}

	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	CEnterRoomNotice *pEnterRoomNotice = dynamic_cast<CEnterRoomNotice *>(pMsgBody);
	if(NULL == pEnterRoomNotice)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CEnterRoomNotice class failed![nMessageID=0x%08x, nRoleID=%d]\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

//	WRITE_DEBUG_LOG("receive enter room notify!{nRoomID=%d, nRoleID=%d}\n",
//			pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID);

	CRoom *pRoom = NULL;
	int32_t ret = g_RoomMgt.GetRoom(pEnterRoomNotice->nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		WRITE_ERROR_LOG("enter room notify, but failed to get room object!{nRoomID=%d, nRoleID=%d}\n",
				pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID);
		return E_NULLPOINTER;

	}
	if(pRoom->IsPlayerInRoom(pEnterRoomNotice->nRoleID))
	{
		WRITE_DEBUG_LOG("player has already been in the room!{nRoleID=%d, nRoomID=%d}\n",
				pEnterRoomNotice->nRoleID, pRoom->GetRoomID());
		return S_OK;
	}

	//判断是否为机器人
	CPlayer *pPlayer = NULL;
	//未取到值或pPlayer为NULL 返回 ret < 0
	ret = g_PlayerMgt.GetPlayer(pEnterRoomNotice->nRoleID, pPlayer);
	if(ret < 0 || pPlayer == NULL)
	{
		if(pEnterRoomNotice->nPlayerType == enmPlayerType_Rebot)
		{
			WRITE_DEBUG_LOG("enter room notify, role is robot, create robot player!{nRoomID=%d, nRoleID=%d}\n",
					pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID);
			ret = g_PlayerMgt.CreatePlayer(pEnterRoomNotice->nRoleID, pPlayer);
			if(ret < 0 || pPlayer == NULL)
			{
				WRITE_ERROR_LOG("enter room notify, create robot player error!{nRoomID=%d, nRoleID=%d}\n",
						pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID);
				return E_NULLPOINTER;
			}
		}
		else
		{
			WRITE_ERROR_LOG("enter room notify, but failed to get role object!{nRoomID=%d, nRoleID=%d}\n",
				pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID);
			return E_NULLPOINTER;
		}
	}

	WRITE_DEBUG_LOG("enter room notify, before player enter room:{nRoomID=%d, nRoleID=%d, nCurUserCount=%d, nRobotCount=%d}\n",
//			pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID, pRoom->GetCurUserCount(), pRoom->GetRobotCount());
			pRoom->GetRoomID(), pEnterRoomNotice->nRoleID, pRoom->GetCurUserCount(), pRoom->GetRobotCount());

	ret = g_RoomMgt.AddPlayer(pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID);
	if(0 > ret)
	{
		WRITE_ERROR_LOG("enter room notify, add player to room failed!{nRoomID=%d, nRoleID=%d}\n",
				pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID);
		return E_NULLPOINTER;
	}

	if(pEnterRoomNotice->nPlayerType == enmPlayerType_Rebot)
	{
		//统计机器人数量
		pPlayer->SetPlayerRobot();
		int32_t nRobotCount = pRoom->GetRobotCount();
		pRoom->SetRobotCount(++nRobotCount);
	}

	if(pEnterRoomNotice->nRoomType==enmRoomType_NewPlayer)
	{
		pPlayer->SetNewPlayerRoomCount(pPlayer->GetNewPlayerRoomCount()+1);
	}

//	WRITE_DEBUG_LOG("enter room notify, succeed to add player to room!{nRoomID=%d, nRoleID=%d}\n",
//			pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID);
	WRITE_DEBUG_LOG("enter room notify, after player enter room:{nRoomID=%d, nRoleID=%d, nCurUserCount=%d, nRobotCount=%d}\n",
			pEnterRoomNotice->nRoomID, pEnterRoomNotice->nRoleID, pRoom->GetCurUserCount(), pRoom->GetRobotCount());

	//更新数据到tunnel
	UpdatePlayerDataInfo();
	UpdateRoomDataInfo();
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
