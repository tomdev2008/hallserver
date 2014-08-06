/*
 * bll_event_tunnelstartup.cpp
 *
 *  Created on: 2012-5-25
 *      Author: fuz
 */

#include "bll_event_tunnelstartup.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "hallserver_message_define.h"
#include "client_message_define.h"
#include "dal/to_server_message.h"
#include "dal/from_server_message.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CTunnelStartMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody == NULL || pMsgHead == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}
	CDaemonStartNotify *pNoti = dynamic_cast<CDaemonStartNotify *>(pMsgBody);
	if(pNoti == NULL)
	{
		WRITE_ERROR_LOG("null pointer: pMsgBody transform to CDaemonStartNotify class failed!"
				"{nMessageID=0x%08x, nSourceType=%d, nSourceID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nSourceType, pMsgHead->nSourceID);
		return E_NULLPOINTER;
	}

	WRITE_INFO_LOG("get daemon start notify!{serverType=%d, serverID=%d}\n", pNoti->serverType,
			pNoti->serverID);

	if(pNoti->serverType == enmEntityType_Tunnel)
	{
		// add tunnel id
		if(g_DataCenter.AddTunnel(pNoti->serverID) < 0)
		{
			WRITE_ERROR_LOG("add tunnel id error, over the max tunnel number!{MaxTunnelNum=%d}\n", MaxTunnelNum);
			return E_NULLPOINTER;
		}
	}

	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
