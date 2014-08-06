/*
 * to_server_message.h
 *
 *  Created on: 2011-12-4
 *      Author: jimm
 */

#ifndef TO_SERVER_MESSAGE_H_
#define TO_SERVER_MESSAGE_H_

#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "hallserver_message_define.h"
#include "tunnelserver_message_define.h"
#include "roomserver_message_define.h"
#include "common/common_codeengine.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

class CGetRoleInfoReq : public CHallGetRoleLoginInfoReq_Public
{
public:
//	int32_t		MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
};

class CGetRoomServerIDResp : public CGetRoomServerIDResp_Public
{
public:
//	int32_t		MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
};

class CGetMediaInfoResp : public CGetMediaInfoResp_Public
{
public:
//	int32_t		MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
};


FRAME_HALLSERVER_NAMESPACE_END

#endif /* TO_SERVER_MESSAGE_H_ */
