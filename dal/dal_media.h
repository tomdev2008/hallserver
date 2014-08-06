/*
 * dal_media.h
 *
 *  Created on: 2012-1-11
 *      Author: jimm
 */

#ifndef DAL_MEDIA_H_
#define DAL_MEDIA_H_

#include "common/common_complextype.h"
#include "common/common_hashlist.h"
#include "common/common_string.h"
#include "frame_typedef.h"
#include "dal/dal_room.h"
#include "def/server_namespace.h"

#include "public_typedef.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

typedef CHashList<RoomID, MaxRoomCountPerServer, MaxRoomCountPerServer * 64> MediaRoomList;

class CMedia : public CObject
{
public:

	CMedia()
	{
		Reset();
	}

	virtual ~CMedia()
	{

	}

	int32_t Initialize()
	{
		Reset();
		return m_stMediaRoomList.Initialize();
	}

	int32_t Resume()
	{
		Reset();
		return m_stMediaRoomList.Resume();
	}

	int32_t Uninitialize()
	{
		Reset();
		return m_stMediaRoomList.Uninitialize();
	}

	void SetServerID(ServerID nServerID)
	{
		m_nServerID = nServerID;
	}

	ServerID GetServerID()
	{
		return m_nServerID;
	}

	void SetNetType(NetType nNetType)
	{
		m_nNetType = nNetType;
	}

	NetType GetNetType()
	{
		return m_nNetType;
	}

	void SetServerAddress(uint32_t nServerAddress)
	{
		m_nServerAddress = nServerAddress;
	}

	uint32_t GetServerAddress()
	{
		return m_nServerAddress;
	}

	void SetServerPort(uint16_t nServerPort)
	{
		m_nServerPort = nServerPort;
	}

	uint16_t GetServerPort()
	{
		return m_nServerPort;
	}

	int32_t AddRoom(const RoomID nRoomID);

	int32_t DeleteRoom(const RoomID nRoomID);

	void DeleteAllRooms();

	bool IsRoomInMedia(const RoomID nRoomID);

	int32_t GetAllRooms(RoomID arrRoomID[], const int32_t arrSize, int32_t &nRoomCount);

	int32_t GetRoomCount();

protected:
	void Reset()
	{
		m_nServerID 	= enmInvalidServerID;
		m_nNetType			= enmNetType_Tel;
		m_nServerAddress	= 0;
		m_nServerPort		= 0;
		m_stMediaRoomList.Initialize();
	}

private:
	MediaRoomList::Key MakeKey(const RoomID nRoomID) const;

private:
	ServerID							m_nServerID;					//所在的roomserver
	NetType								m_nNetType;							//网络类型
	uint32_t							m_nServerAddress;
	uint16_t							m_nServerPort;
	MediaRoomList						m_stMediaRoomList;						//玩家对象池
};


FRAME_HALLSERVER_NAMESPACE_END

CCOMPLEXTYPE_SPECIFICALLY(FRAME_HALLSERVER_NAMESPACE::CMedia)

#endif /* DAL_MEDIA_H_ */
