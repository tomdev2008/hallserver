/*
 * bll_event_checkcrossdaysplayer.h
 *
 *  Created on: 2012-5-30
 *      Author: fuz
 */

#ifndef BLL_EVENT_CHECKCROSSDAYSPLAYER_H_
#define BLL_EVENT_CHECKCROSSDAYSPLAYER_H_

#include "frame_namespace.h"
#include "frame_def.h"
#include "lightframe_impl.h"
#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "dal/from_server_message.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"
#include "common/common_datetime.h"
#include "dal/dal_player.h"
#include "def/dal_def.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

class Checked
{
public:
	Checked()
	{
		bChecked = false;
		date = CDateTime::CurrentDateTime();
	}

	bool bChecked;
	CDateTime date;
};

class CCheckCrossDaysPlayerTimerEvent : public ITimerEvent
{
public:
	CCheckCrossDaysPlayerTimerEvent(){}
	virtual ~CCheckCrossDaysPlayerTimerEvent(){}
	virtual int32_t OnTimerEvent(CFrameTimer *pTimer);
	Checked GetChecked() { return checked; }
private:
	Checked checked;
};

#define	CREATE_CHECKCROSSDAYSEVENT_INSTANCE		CSingleton<CCheckCrossDaysPlayerTimerEvent>::CreateInstance
#define	GET_CHECKCROSSDAYSEVENT_INSTANCE			CSingleton<CCheckCrossDaysPlayerTimerEvent>::GetInstance
#define	DESTROY_CHECKCROSSDAYSEVENT_INSTANCE		CSingleton<CCheckCrossDaysPlayerTimerEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END
#endif /* BLL_EVENT_CHECKCROSSDAYSPLAYER_H_ */
