/*
 * to_server_message.cpp
 *
 *  Created on: 2011-12-4
 *      Author: jimm
 */

#include <memory.h>

#include "to_server_message.h"
#include "frame_errordef.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

//int32_t	CGetRoleInfoReq::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
//{
//	int32_t nRet = S_OK;
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nRoleID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nRoleID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nRoleID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	return nRet;
//}

//int32_t	CGetRoleLoginInfo::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
//{
//	return S_OK;
//}
//
//void CGetRoleLoginInfo::Dump(char* buf, const uint32_t size, uint32_t& offset)
//{
//	CGetRoleLoginInfo_Public::Dump(buf, size, offset);
//}



//int32_t	CGetRoomServerIDResp::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
//{
//	int32_t nRet = S_OK;
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nResult);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nRoleID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nRoomID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nServerID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nConnIndex);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//
//	return nRet;
//}
//
//int32_t	CGetMediaInfoResp::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
//{
//	int32_t nRet = S_OK;
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nResult);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//
//	if(nResult < 0)
//	{
//		return S_OK;
//	}
//
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nTelMediaIP);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nTelMediaPort);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nTelServerID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nCncMediaIP);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nCncMediaPort);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nCncServerID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//
//	return nRet;
//}


FRAME_HALLSERVER_NAMESPACE_END

