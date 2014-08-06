/*
 * bll_event_buildroomdatatimeout.h
 *
 *  Created on: 2012-8-6
 *      Author: zhurw
 */

#ifndef BLL_EVENT_BUILDROOMDATATIMEOUT_H_
#define BLL_EVENT_BUILDROOMDATATIMEOUT_H_

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

class CBuildRoomDataTimeoutEvent : public ITimerEvent
{
public:
	CBuildRoomDataTimeoutEvent(){}
	virtual ~CBuildRoomDataTimeoutEvent(){}
	virtual int32_t OnTimerEvent(CFrameTimer *pTimer);
private:
};

#define	CREATE_BUILDROOMDATATIMEOUTEVENT_INSTANCE		CSingleton<CBuildRoomDataTimeoutEvent>::CreateInstance
#define	GET_BUILDROOMDATATIMEOUTEVENT_INSTANCE			CSingleton<CBuildRoomDataTimeoutEvent>::GetInstance
#define	DESTROY_BUILDROOMDATATIMEOUTEVENT_INSTANCE		CSingleton<CBuildRoomDataTimeoutEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_BUILDROOMDATATIMEOUT_H_ */
