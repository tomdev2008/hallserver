/*
 * bll_event_checkonlinenum.h
 *
 *  Created on: 2012-5-24
 *      Author: fuz
 */

#ifndef BLL_EVENT_CHECKONLINENUM_H_
#define BLL_EVENT_CHECKONLINENUM_H_

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

class CCheckOnlineNumEvent : public ITimerEvent
{
public:
	CCheckOnlineNumEvent(){}
	virtual ~CCheckOnlineNumEvent(){}
	virtual int32_t OnTimerEvent(CFrameTimer *pTimer);
private:
};

#define	CREATE_CHECKONLINENUMEVENT_INSTANCE		CSingleton<CCheckOnlineNumEvent>::CreateInstance
#define	GET_CHECKONLINENUMEVENT_INSTANCE		CSingleton<CCheckOnlineNumEvent>::GetInstance
#define	DESTROY_CHECKONLINENUMEVENT_INSTANCE	CSingleton<CCheckOnlineNumEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END
#endif /* BLL_EVENT_CHECKONLINENUM_H_ */
