/*
 * bll_event_builddatanum.cpp
 *
 *  Created on: 2012-5-24
 *      Author: fuz
 */

#include "bll_event_builddatanum.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "hallserver_message_define.h"
#include "client_message_define.h"
#include "dal/to_server_message.h"
#include "dal/from_server_message.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CBuildDataNumMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody == NULL || pMsgHead == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}
	CTunnelUpdateDataNumNoti *pNoti = dynamic_cast<CTunnelUpdateDataNumNoti *>(pMsgBody);
	if(pNoti == NULL)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CTunnelUpdateDataNumNoti class failed!{nMessageID=0x%08x, nSourceID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nSourceID);
		return E_NULLPOINTER;
	}

	WRITE_INFO_LOG("got build data number message notify!{nSourceID=%d, nRoomNum=%u, nPlayerNum=%u}\n",
			pMsgHead->nSourceID, pNoti->nRoomNum, pNoti->nPlayerNum);

	// add tunnel id and total cache number
	if(g_DataCenter.AddTunnel(pMsgHead->nSourceID, pNoti->nRoomNum, pNoti->nPlayerNum) < 0)
	{
		WRITE_ERROR_LOG("build cache number:over the max tunnel server id!{MaxTunnelNum=%d}\n", MaxTunnelNum);
		return E_NULLPOINTER;
	}

	if(pNoti->nRoomNum == 0 && pNoti->nPlayerNum == 0)
	{
		uint32_t nIndex;
		int32_t ret = g_DataCenter.GetTunnelServerIndex(pMsgHead->nSourceID, nIndex);
		if(ret != S_OK || nIndex >= MaxTunnelNum)
		{
			WRITE_ERROR_LOG("cannot find the tunnel server!{nSourceID=%d}\n", pMsgHead->nSourceID);
		}
		TunnelServerInfo& tunnelServerInfo = g_TunnelServerArray[nIndex];
		tunnelServerInfo.bInitialized = true;
	}

	// verify all tunnel,所有tunnel发送的数据都恢复后服务器状态置为enmServerStatus_Running
//	VerifyTunnel();

	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
