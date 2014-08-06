/*
 * dal_media.cpp
 *
 *  Created on: 2012-1-11
 *      Author: jimm
 */

#include "dal_media.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

MediaRoomList::Key CMedia::MakeKey(const RoomID nRoomID) const
{
	MediaRoomList::Key key = { 0 };
	key.nKeyLength = (uint32_t)sizeof(RoomID);
	*(RoomID*)key.arrKey = nRoomID;

	return key;
}


int32_t CMedia::AddRoom(const RoomID nRoomID)
{
	MediaRoomList::Key key = MakeKey(nRoomID);
	MediaRoomList::CIndex* pIndex = m_stMediaRoomList.Insert(key, nRoomID);
	if (NULL != pIndex)
	{
	}

	return S_OK;
}

int32_t CMedia::DeleteRoom(const RoomID nRoomID)
{
	MediaRoomList::Key key = MakeKey(nRoomID);
	int32_t ret = m_stMediaRoomList.Erase(key);

	return ret;
}

void CMedia::DeleteAllRooms()
{
	m_stMediaRoomList.Clear();
}

bool CMedia::IsRoomInMedia(const RoomID nRoomID)
{
	MediaRoomList::Key key = MakeKey(nRoomID);
	MediaRoomList::CIndex *pIndex = m_stMediaRoomList.Find(key);

	return NULL != pIndex;
}

int32_t CMedia::GetAllRooms(RoomID arrRoomID[], const int32_t arrSize, int32_t &nRoomCount)
{
	nRoomCount = 0;
	MediaRoomList::CIndex *pIndex = m_stMediaRoomList.First();

	while(NULL != pIndex && nRoomCount < arrSize)
	{
		arrRoomID[nRoomCount++] = pIndex->Object();
		pIndex = pIndex->Next();
	}

	return S_OK;
}

int32_t CMedia::GetRoomCount()
{
	return m_stMediaRoomList.ObjectCount();
}

FRAME_HALLSERVER_NAMESPACE_END
