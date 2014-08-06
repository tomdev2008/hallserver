/*
 * from_server_message.cpp
 *
 *  Created on: 2011-12-6
 *      Author: jimm
 */

#include <memory.h>

#include "from_server_message.h"
#include "frame_errordef.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

//int32_t CGetRoomServerIDReq::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
//{
//	int32_t nRet = S_OK;
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
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nConnIndex);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//
//	return nRet;
//}

//int32_t CGetRoomServerIDReq::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
//{
//	int32_t nRet = S_OK;
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoleID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoomID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nConnIndex);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//
//	return nRet;
//}

//int32_t	CGetRoleInfoResp::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
//{
//	int32_t nRet = S_OK;
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nResult);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//
//	if(nResult < 0)
//	{
//
//	}
//	else
//	{
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoleID);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, nLastVersion);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, nLastLoginTime);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, nSystemTime);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, nGender);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::MessageDecode(buf, size, offset, strRoleName);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, n179ID);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, nVipLevel);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, nUserLevel);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, nOnlineTime);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, nLeftTime);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, nLeftMoney);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, nLastLogoutTime);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, nExperience);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//	}
//
//	return nRet;
//}

//int32_t CRoleLogoutNoti::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
//{
//	return S_OK;
//}

//int32_t CUpdateInfoToHallNotice::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
//{
//	int32_t nRet = S_OK;
//
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoomCount);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//
//	for(int32_t i = 0; i < nRoomCount; ++i)
//	{
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, arrRoomID[i]);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//	}
//
//	for(int32_t i = 0; i < nRoomCount; ++i)
//	{
//		nRet = CFrameCodeEngine::Decode(buf, size, offset, arrPlayerCount[i]);
//		if (0 > nRet)
//		{
//			return nRet;
//		}
//	}
//
//	for(int32_t i = 0; i < nRoomCount; ++i)
//	{
//		for(int32_t j = 0; j < arrPlayerCount[i]; ++j)
//		{
//			nRet = CFrameCodeEngine::Decode(buf, size, offset, arrRoleID[i][j]);
//			if (0 > nRet)
//			{
//				return nRet;
//			}
//		}
//	}
//
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nServerAddress);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nServerPort);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nServerNetType);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	return S_OK;
//}

//int32_t CGetMediaInfoReq::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
//{
//	return S_OK;
//}

//int32_t CEnterRoomNotice::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
//{
//	int32_t nRet = S_OK;
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoleID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoomID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nServerID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nPlayerType);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoleRank);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::MessageDecode(buf, size, offset, strRoleName);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nAccountID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoomType);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	return nRet;
//}

//int32_t CExitRoomNotice::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
//{
//	int32_t nRet = S_OK;
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoleID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoomID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoomType);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	return nRet;
//}

//int32_t	CCreateRoomNotice::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
//{
//	int32_t nRet = S_OK;
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nRegionType);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nRegionID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::MessageEncode(buf, size, offset, strRegionName);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nChannelType);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nChannelID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::MessageEncode(buf, size, offset, strChannelName);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nRoomType);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nRoomID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::MessageEncode(buf, size, offset, strRoomName);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Encode(buf, size, offset, nServerID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//
//	return nRet;
//}
//
//
//int32_t	CCreateRoomNotice::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
//{
//	int32_t nRet = S_OK;
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRegionType);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRegionID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::MessageDecode(buf, size, offset, strRegionName);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nChannelType);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nChannelID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::MessageDecode(buf, size, offset, strChannelName);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoomType);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nRoomID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::MessageDecode(buf, size, offset, strRoomName);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nServerID);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	nRet = CFrameCodeEngine::Decode(buf, size, offset, nMemberCount);
//	if (0 > nRet)
//	{
//		return nRet;
//	}
//	if(nMemberCount > 0)
//	{
//		for(int32_t i = 0; i < nMemberCount; ++i)
//		{
//			nRet = CFrameCodeEngine::Decode(buf, size, offset, arrRoleID[i]);
//			if (0 > nRet)
//			{
//				return nRet;
//			}
//		}
//		for(int32_t i = 0; i < nMemberCount; ++i)
//		{
//			nRet = CFrameCodeEngine::Decode(buf, size, offset, arrRoleRank[i]);
//			if (0 > nRet)
//			{
//				return nRet;
//			}
//		}
//	}
//
//
//	return nRet;
//}

FRAME_HALLSERVER_NAMESPACE_END

