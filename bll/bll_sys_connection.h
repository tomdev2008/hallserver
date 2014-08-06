/*
 * bll_sys_connection.h
 *
 *  Created on: 2012-12-14
 *      Author: jimm
 */

#ifndef BLL_SYS_CONNECTION_H_
#define BLL_SYS_CONNECTION_H_

#include "common/common_singleton.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "lightframe_impl.h"
#include "main_frame.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

class CSystemEventHandler : public ISystemEvent
{
public:
	int32_t OnSystemEvent(uint16_t nEventID, void *pParam);
};

#define	CREATE_SYSTEMEVENT_INSTANCE		CSingleton<CSystemEventHandler>::CreateInstance
#define	GET_SYSTEMEVENT_INSTANCE		CSingleton<CSystemEventHandler>::GetInstance
#define	DESTROY_SYSTEMEVENT_INSTANCE	CSingleton<CSystemEventHandler>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_SYS_CONNECTION_H_ */
