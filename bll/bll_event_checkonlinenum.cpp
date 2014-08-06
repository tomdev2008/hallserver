/*
 * bll_event_checkonlinenum.cpp
 *
 *  Created on: 2012-5-24
 *      Author: fuz
 */

#include "bll_event_checkonlinenum.h"
#include "ctl/server_datacenter.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CCheckOnlineNumEvent::OnTimerEvent(CFrameTimer *pTimer)
{
	WRITE_DEBUG_LOG("-----------on check online number timer----------\n");

	//get current online number, print to log
	uint32_t nCurrentOnlineNum = g_PlayerMgt.GetPlayerCount();
	uint32_t nCurOnlineRealPlayerNum = g_PlayerMgt.GetRealPlayerCount();
	STATISTIC_WRITE_INFO_LOG("log_notice, {nCurrentOnlineNum=%u,nCurOnlineRealPlayerNum=%u}\n", nCurrentOnlineNum, nCurOnlineRealPlayerNum);

	//get max online number, print to log
	CDateTime dCurrentDate = CDateTime::CurrentDateTime();
//	if(dCurrentDate.Date() > g_MaxOnlineInfo.dMaxOnlineDate.Date() ||
//			nCurrentOnlineNum > g_MaxOnlineInfo.nMaxOnlineTotalNum)
//	{
//		g_MaxOnlineInfo.nMaxOnlineTotalNum = nCurrentOnlineNum;
//		g_MaxOnlineInfo.dMaxOnlineDate = dCurrentDate;
//	}
//	if(dCurrentDate.Date() > g_MaxOnlineInfo.dMaxOnlineDate.Date() ||
//			nCurOnlineRealPlayerNum > g_MaxOnlineInfo.nMaxOnlineRealPlayerNum)
//	{
//		g_
//	}
	if(dCurrentDate.Date() > g_MaxOnlineInfo.dMaxOnlineDate.Date())
	{
		g_MaxOnlineInfo.UpdateMaxOnlineInfo(nCurrentOnlineNum, nCurOnlineRealPlayerNum, dCurrentDate);
	}
	else
	{
		if(nCurrentOnlineNum > g_MaxOnlineInfo.nMaxOnlineTotalNum)
		{
			g_MaxOnlineInfo.UpdateTotalOnlineNum(nCurrentOnlineNum, dCurrentDate);
		}
		if(nCurOnlineRealPlayerNum > g_MaxOnlineInfo.nMaxOnlineRealPlayerNum)
		{
			g_MaxOnlineInfo.UpdateRealOnlinePlayerNum(nCurOnlineRealPlayerNum, dCurrentDate);
		}
	}

	STATISTIC_WRITE_INFO_LOG("log_notice, {nMaxOnlineTotalNum=%u, nMaxOnlineDate=%ld, nMaxOnlineRealPlayerNum=%u}\n",
			g_MaxOnlineInfo.nMaxOnlineTotalNum, g_MaxOnlineInfo.dMaxOnlineDate.Seconds(), g_MaxOnlineInfo.nMaxOnlineRealPlayerNum);

	STATISTIC_WRITE_INFO_LOG("Current player number: {nCurrentRealPlayerNum = %u}\n", nCurOnlineRealPlayerNum );
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
