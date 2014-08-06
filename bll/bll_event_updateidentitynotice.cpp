/*
 * bll_event_updateidentitynotice.cpp
 *
 *  Created on: 2012-9-19
 *      Author: liufl
 */

#include "bll_event_updateidentitynotice.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN
int32_t CUpdateIdentityNotiEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
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

	CUpdateIdentityNoti* pUpdateIdentityNoti = dynamic_cast<CUpdateIdentityNoti*>(pMsgBody);
	if(pUpdateIdentityNoti == NULL)
	{
		WRITE_ERROR_LOG("null pointer: pMsgBody transform to CUpdateMemIdentityNotify class failed!"
				"{nMessageID=0x%08x, nRoleID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

	switch(pMsgHead->nMessageID)
	{
		case MSGID_ISOS_UPDATEIDENTITY_NOTI:
		{
			CPlayer* pPlayer = NULL;
			for(int32_t i=0;i<pUpdateIdentityNoti->nCount;++i)
			{
				nRet=S_OK;
				nRet = g_PlayerMgt.GetPlayer(pUpdateIdentityNoti->nRoleID[i], pPlayer);
				if(0 > nRet || pPlayer == NULL)
				{
						WRITE_ERROR_LOG("update identity, cannot find role object!{nRoleID=%d, nRet=0x%08x}\n", pUpdateIdentityNoti->nRoleID[i], nRet);
						continue;
				}
				pPlayer->SetIdentityType(pUpdateIdentityNoti->nIdentityType[i]);
				WRITE_DEBUG_LOG("update identity, succeed to set identity!{nRoleID=%d, nIdentity=%d}\n", pUpdateIdentityNoti->nRoleID[i],pUpdateIdentityNoti->nIdentityType[i]);
			}
		}
		break;
		default:
		WRITE_ERROR_LOG("update identity notify, get wrong message!{nMessageID=0x%08x}\n", pMsgHead->nMessageID);
		break;
	}
	return S_OK;
}


FRAME_HALLSERVER_NAMESPACE_END
