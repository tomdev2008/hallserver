/*
 * dal_channel.h
 *
 *  Created on: 2011-12-1
 *      Author: jimm
 */

#ifndef DAL_CHANNEL_H_
#define DAL_CHANNEL_H_

#include "common/common_complextype.h"
#include "common/common_object.h"
#include "def/server_namespace.h"
#include "public_typedef.h"
#include "dal_room.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

typedef CHashList<RoomID, MaxRoomCountPerChannel, ChannelBucketSize>	RoomIDList;


class CChannel : public CObject
{
public:
	CChannel()
	{
		Reset();
	}

	~CChannel()
	{

	}

	virtual int32_t Initialize()
	{
		Reset();
		return m_sRoomIDList.Initialize();
	}

	virtual int32_t Resume()
	{
		Reset();
		return m_sRoomIDList.Resume();
	}

	virtual int32_t Uninitialize()
	{
		Reset();
		return m_sRoomIDList.Uninitialize();
	}

	int32_t AddRoom(const RoomID nRoomID, const RoomType nRoomType);

	int32_t DeleteRoom(const RoomID nRoomID);

	bool IsRoomInChannel(const RoomID nRoomID);

	int32_t GetAllRoom(RoomID arrRoomID[], const int32_t arrSize, int32_t &nRoomCount);

	RegionType GetRegionType() const
	{
		return m_nRegionType;
	}

	void SetRegionType(const RegionType nRegionType)
	{
		m_nRegionType = nRegionType;
	}

	RegionID GetRegionID() const
	{
		return m_nRegionID;
	}

	void SetRegionID(const RegionID nRegionID)
	{
		m_nRegionID = nRegionID;
	}

	ChannelType GetChannelType() const
	{
		return m_nChannelType;
	}

	void SetChannelType(const ChannelType nChannelType)
	{
		m_nChannelType = nChannelType;
	}

	ChannelID GetChannelID() const
	{
		return m_nChannelID;
	}

	void SetChannelID(const ChannelID nChannelID)
	{
		m_nChannelID = nChannelID;
	}

	int32_t GetRoomCount() const
	{
		return m_nCurRoomCount;
	}

protected:
	RoomIDList::Key MakeKey(const RoomID nRoomID);

	void Reset()
	{
		m_nRegionType = enmInvalidRegionType;
		m_nRegionID = enmInvalidRegionID;
		m_nChannelType = enmInvalidChannelType;
		m_nChannelID = enmInvalidChannelID;
		m_nCurRoomCount = 0;
	}

private:
	RegionType						m_nRegionType;			//��������
	RegionID						m_nRegionID;			//������
	ChannelType						m_nChannelType;			//Ƶ������
	ChannelID						m_nChannelID;			//Ƶ����
	CString<MaxChannelNameLength>	m_strChannelName;		//Ƶ������
	int32_t						m_nCurRoomCount;		//��ǰ������
	RoomIDList						m_sRoomIDList;			//��������
};


FRAME_HALLSERVER_NAMESPACE_END

CCOMPLEXTYPE_SPECIFICALLY(FRAME_HALLSERVER_NAMESPACE::CChannel)

#endif /* DAL_CHANNEL_H_ */
