/*
 * bll_event_updateviplevelnotice.cpp
 *
 *  Created on: 2012-3-15
 *      Author: zhurw
 */

#include "bll_event_updateviplevelnotice.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN
int32_t CUpdateVipNotiEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	// check the server status
	if(g_ServerStatus != enmServerStatus_Running)
	{
		WRITE_ERROR_LOG("server is not ready!\n");
		return E_NULLPOINTER;
	}

	int32_t nRet = S_OK;
	if(pMsgHead == NULL || pMsgBody == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	CUpdateMemVipLevelNotify* pUpdateVipLevelNoti = dynamic_cast<CUpdateMemVipLevelNotify*>(pMsgBody);
	if(pUpdateVipLevelNoti == NULL)
	{
		WRITE_ERROR_LOG("null pointer: pMsgBody transform to CUpdateMemVipLevelNotify class failed!"
				"{nMessageID=0x%08x, nRoleID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

	switch(pMsgHead->nMessageID)
	{
		case MSGID_SSRS_UPDATELEVEL_NOTI:
		{
			WRITE_INFO_LOG("receive update vip notify!{nRoleID=%d, nVipLevel=%d}\n",
					pUpdateVipLevelNoti->nRoleID, pUpdateVipLevelNoti->nVipLevel);

			nRet = UpdateMemVipLevel(pUpdateVipLevelNoti->nRoleID, pUpdateVipLevelNoti->nVipLevel);
			if(0 > nRet)
			{
				WRITE_ERROR_LOG("update vip notify, update vip error!{nRoleID=%d, nVipLevel=%d, nRet=0x%08x}\n",
						pUpdateVipLevelNoti->nRoleID, pUpdateVipLevelNoti->nVipLevel, nRet);
				return E_UNKNOWN;
			}
		}
		break;
		default:
		WRITE_ERROR_LOG("update vip notify, get wrong message!{nMessageID=0x%08x}\n", pMsgHead->nMessageID);
		break;
	}
	return S_OK;
}

int32_t CUpdateVipNotiEvent::UpdateMemVipLevel(RoleID nRoleID, VipLevel nVipLevel)
{
	int32_t nRet = S_OK;
	CPlayer* pPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(nRoleID, pPlayer);
	if(0 > nRet || pPlayer == NULL)
	{
		WRITE_NOTICE_LOG("update vip level, cannot find role object!{nRoleID=%d, nRet=0x%08x}\n", nRoleID, nRet);
		return E_NULLPOINTER;
	}
	if(nVipLevel > enmVipLevel_King || nVipLevel <= enmVipLevel_NONE)
	{
		WRITE_ERROR_LOG("update vip level, wrong vip level!{nRoleID=%d, nVipLevel=%d}\n", nRoleID, nVipLevel);
		return E_NULLPOINTER;
	}

	pPlayer->SetVipLevel(nVipLevel);
	WRITE_DEBUG_LOG("update vip level, succeed to set vip level!{nRoleID=%d, nVipLevel=%d}\n", nRoleID, nVipLevel);
	return S_OK;
}
FRAME_HALLSERVER_NAMESPACE_END
