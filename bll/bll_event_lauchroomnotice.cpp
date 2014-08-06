/*
 * bll_event_lauchroomnotice.cpp
 *
 *  Created on: 2012-8-6
 *      Author: zhurw
 */
#include "bll_event_lauchroomnotice.h"
#include "public_message_define.h"
#include "ctl/server_datacenter.h"
#include "public_typedef.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CLauchRoomNoticeEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		return E_NULLPOINTER;
	}

	if(pMsgBody == NULL || pMsgHead == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	CLauchRoomNotice* pLauchRoomNotice = dynamic_cast<CLauchRoomNotice*>(pMsgBody);
	if(pLauchRoomNotice == NULL)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CLauchRoomNotice class failed![nMessageID=0x%08x, nRoleID=%d]\n",
				pMsgHead->nMessageID, pLauchRoomNotice->nRoleID);
		return E_NULLPOINTER;
	}
	CPlayer* pPlayer = NULL;
	int32_t nRet = g_PlayerMgt.GetPlayer(pLauchRoomNotice->nRoleID, pPlayer);
	if(nRet < 0 || pPlayer == NULL)
	{
		WRITE_NOTICE_LOG("lauchroomnotice:player is not online!{nRoomID=%d,nRoleID=%d}",
				pLauchRoomNotice->nRoomID, pLauchRoomNotice->nRoleID);
		return S_OK;
	}
	RoleRank nPreRoleRank = enmRoleRank_None;
	pPlayer->SetAdminRoleRank(pLauchRoomNotice->nRoomID, enmRoleRank_Host,nPreRoleRank);

	CRoom *pRoom = NULL;
	nRet = g_RoomMgt.GetRoom(pLauchRoomNotice->nRoomID, pRoom);
	if(0 > nRet || pRoom == NULL)
	{
		WRITE_ERROR_LOG("get room object error!{nRoomID=%d, nRet=0x%08x}\n", pLauchRoomNotice->nRoomID, nRet);
		return nRet;
	}
	if(nPreRoleRank == enmRoleRank_TempAdmin)
	{
		int32_t TemAdminCount = pRoom->GetTempAdminCount();
		if( TemAdminCount >= 1)
		{
			pRoom->SetTempAdminCount(--TemAdminCount);
		}
	}
	return S_OK;
}
FRAME_HALLSERVER_NAMESPACE_END
