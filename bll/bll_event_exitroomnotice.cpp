/*
 * bll_event_exitroomnotice.cpp
 *
 *  Created on: 2012-2-15
 *      Author: jimm
 */

#include "bll_event_exitroomnotice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "dal/from_server_message.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CExitRoomNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
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

	CExitRoomNotice *pExitRoomNotice = dynamic_cast<CExitRoomNotice *>(pMsgBody);
	if(NULL == pExitRoomNotice)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CExitRoomNotice class failed![nMessageID=0x%08x, nRoleID=%d]\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

	WRITE_INFO_LOG("receive exit room notify!{nRoomID=%d, nRoleID=%d}\n", pExitRoomNotice->nRoomID, pExitRoomNotice->nRoleID);

	CRoom *pRoom = NULL;
	int32_t ret = g_RoomMgt.GetRoom(pExitRoomNotice->nRoomID, pRoom);
	if(ret < 0 || pRoom == NULL)
	{
		// degrade this log level from error to warning, because room server won't change the logout logic!!
		WRITE_WARNING_LOG("exit room notify, but failed to get room object!{nRoomID=%d, nRoleID=%d}\n",
				pExitRoomNotice->nRoomID, pExitRoomNotice->nRoleID);
		return E_NULLPOINTER;
	}

	CPlayer *pPlayer = NULL;
	ret = g_PlayerMgt.GetPlayer(pExitRoomNotice->nRoleID, pPlayer);
	if(ret < 0 || pPlayer == NULL)
	{
		WRITE_NOTICE_LOG("exit room notify, but failed to get role object!{nRoomID=%d, nRoleID=%d}\n",
				pExitRoomNotice->nRoomID, pExitRoomNotice->nRoleID);
		pRoom->DeletePlayer(pExitRoomNotice->nRoleID);
		return E_NULLPOINTER;
	}

//	WRITE_DEBUG_LOG("exit room notify, before exit:{nCurUserCount=%d, nRobotCount=%d}\n",
//			pRoom->GetCurUserCount(), pRoom->GetRobotCount());
	g_RoomMgt.DeletePlayer(pExitRoomNotice->nRoomID, pExitRoomNotice->nRoleID);

	RoleRank nRoleRank = pPlayer->GetAdminRoleRank(pRoom->GetRoomID());
	WRITE_DEBUG_LOG("exitroom:player admin role rank{nRoleRank=%d}\n",nRoleRank);
	//临管退出房间取消他的临管权限
	if(nRoleRank == enmRoleRank_TempAdmin)
	{
		pPlayer->SetAdminRoleRank(pRoom->GetRoomID(), enmRoleRank_None);
		int32_t nCurTempAdminCount = pRoom->GetTempAdminCount();
		if(nCurTempAdminCount >= 1)
		{
			pRoom->SetTempAdminCount(--nCurTempAdminCount);
		}
	}

	if(pExitRoomNotice->nRoomType==enmRoomType_NewPlayer)
	{
		uint32_t nNewPlayerRoomCount = pPlayer->GetNewPlayerRoomCount();
		if(nNewPlayerRoomCount>0)
		{
			pPlayer->SetNewPlayerRoomCount(--nNewPlayerRoomCount);
		}
	}
	//机器人退房删除机器人玩家信息
	if(pPlayer->IsRobot() && pPlayer->GetCurEnterRoomCount() == 0)
	{
		//统计机器人数量
		int32_t nRobotCount = pRoom->GetRobotCount();
		if(nRobotCount >= 1)
		{
			pRoom->SetRobotCount(--nRobotCount);
		}
		ret = g_PlayerMgt.DestoryPlayer(pPlayer->GetRoleID());
		if(0 > ret)
		{
			WRITE_ERROR_LOG("destroy robot player failed!{nRoleID=%d}\n", pExitRoomNotice->nRoleID);
			return ret;
		}
		WRITE_DEBUG_LOG("robot enter room count=0, delete this robot player!{nRoleID=%d}\n", pExitRoomNotice->nRoleID);
	}

	WRITE_DEBUG_LOG("exit room notify, after player exit:{nRoomID=%d, nCurUserCount=%d, nRobotCount=%d}\n",
			pRoom->GetRoomID(), pRoom->GetCurUserCount(), pRoom->GetRobotCount());

//  收到销毁房间通知后再销毁房间
//	if(pRoom->GetCurUserCount()==0)
//	{
//		//房间没有玩家的时候删除房间
//		g_RoomMgt.DestoryRoom(pRoom->GetRoomID());
//	}

	//更新数据到tunnel
	UpdatePlayerDataInfo();
	UpdateRoomDataInfo();
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END

