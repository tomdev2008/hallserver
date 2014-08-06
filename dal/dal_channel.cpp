/*
 * dal_channel.cpp
 *
 *  Created on: 2011-12-3
 *      Author: jimm
 */

#include "dal_channel.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN


int32_t CChannel::AddRoom(const RoomID nRoomID, const RoomType nRoomType)
{
	RoomIDList::Key key = MakeKey(nRoomID);
	RoomIDList::CIndex* pIndex = m_sRoomIDList.Insert(key, nRoomID);
	if (NULL != pIndex)
	{
		++m_nCurRoomCount;
	}

	return S_OK;
}

int32_t CChannel::DeleteRoom(const RoomID nRoomID)
{
	RoomIDList::Key key = MakeKey(nRoomID);
	int32_t ret = m_sRoomIDList.Erase(key);
	if (0 <= ret)
	{
		--m_nCurRoomCount;
	}

	return S_OK;
}

bool CChannel::IsRoomInChannel(const RoomID nRoomID)
{
	RoomIDList::Key key = MakeKey(nRoomID);
	RoomIDList::CIndex *pIndex = m_sRoomIDList.Find(key);

	return NULL != pIndex;
}

int32_t CChannel::GetAllRoom(RoomID arrRoomID[], const int32_t arrSize, int32_t &nRoomCount)
{
	nRoomCount = 0;
	RoomIDList::CIndex *pIndex = m_sRoomIDList.First();

	while(NULL != pIndex && nRoomCount < arrSize)
	{
		arrRoomID[nRoomCount++] = pIndex->Object();
		pIndex = pIndex->Next();
	}

	return S_OK;
}

RoomIDList::Key CChannel::MakeKey(const RoomID nRoomID)
{
	RoomIDList::Key key = { 0 };
	key.nKeyLength = (uint32_t)sizeof(nRoomID);
	*(RoleID*)key.arrKey = nRoomID;

	return key;
}

FRAME_HALLSERVER_NAMESPACE_END

