/*
 * bll_event_checkcrossdaysplayer.cpp
 *
 *  Created on: 2012-5-30
 *      Author: fuz
 */

#include "bll_event_checkcrossdaysplayer.h"
#include "ctl/server_datacenter.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CCheckCrossDaysPlayerTimerEvent::OnTimerEvent(CFrameTimer *pTimer)
{
	WRITE_DEBUG_LOG("-----------on check cross days player timer----------\n");

	CDateTime currentTime = CDateTime::CurrentDateTime();
	int64_t nLeftSeconds = g_BllBase.GetLeftSecondsBeforeTomorrow(currentTime);
	if((nLeftSeconds <= enmCheckCrossDaysBeforeTomo * SECOND_PER_MINUTE) &&
			(checked.date.Date() < CDateTime::CurrentDateTime().Date() || !checked.bChecked))
	{
		WRITE_DEBUG_LOG("start to log all the online players to statistic log!{nPlayerCount=%d}\n",
				g_PlayerMgt.GetPlayerCount());
		PlayerPool::CIndex* pIndex = g_PlayerMgt.First();
		while(pIndex != NULL)
		{
			CPlayer* pPlayer = pIndex->ObjectPtr();
			if(pPlayer->IsRobot())
			{
				pIndex = pIndex->Next();
				continue;
			}
			int64_t nLoginTime = pPlayer->GetLoginTime();
			CDateTime loginTime(nLoginTime * US_PER_SECOND);
			nLoginTime = loginTime.Seconds();
			if(loginTime.Date() < currentTime.Date()) // cross days
			{
				CDateTime todayBeginTime(currentTime.Year(), currentTime.Month(),
						currentTime.Day(), 0, 0, 0, 0);
				nLoginTime = todayBeginTime.Seconds();
			}

			STATISTIC_WRITE_INFO_LOG("log_notice, {nRoleID=%d, nOnlinetime=%ld}\n",
					pPlayer->GetRoleID(), (currentTime.Seconds() - nLoginTime + nLeftSeconds)/SECOND_PER_MINUTE);

			pIndex = pIndex->Next();
		}
		checked.date = currentTime;
		checked.bChecked = true;
	}
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
