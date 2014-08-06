/*
 * bll_sys_conneciton.cpp
 *
 *  Created on: 2012-12-14
 *      Author: jimm
 */

#include "bll_sys_connection.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CSystemEventHandler::OnSystemEvent(uint16_t nEventID, void *pParam)
{
	WRITE_DEBUG_LOG("recv sys connect event!\n");

	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END

