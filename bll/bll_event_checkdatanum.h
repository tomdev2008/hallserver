/*
 * bll_event_checkdatanum.h
 *
 *  Created on: 2012-5-24
 *      Author: fuz
 */

#ifndef BLL_EVENT_CHECKDATANUM_H_
#define BLL_EVENT_CHECKDATANUM_H_

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

class CCheckDataNumEvent : public ITimerEvent
{
public:
	CCheckDataNumEvent(){}
	virtual ~CCheckDataNumEvent(){}
	virtual int32_t OnTimerEvent(CFrameTimer *pTimer);
private:
};

#define	CREATE_CHECKDATANUMEVENT_INSTANCE		CSingleton<CCheckDataNumEvent>::CreateInstance
#define	GET_CHECKDATANUMEVENT_INSTANCE			CSingleton<CCheckDataNumEvent>::GetInstance
#define	DESTROY_CHECKDATANUMEVENT_INSTANCE		CSingleton<CCheckDataNumEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END
#endif /* BLL_EVENT_CHECKDATANUM_H_ */
