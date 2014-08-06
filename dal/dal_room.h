/*
 * dal_room.h
 *
 *  Created on: 2011-11-30
 *      Author: jimm
 */

#ifndef DAL_ROOM_H_
#define DAL_ROOM_H_

#include "common/common_complextype.h"
#include "common/common_hashlist.h"
#include "common/common_string.h"
#include "frame_typedef.h"
#include "dal/dal_player.h"
#include "def/server_namespace.h"
#include "dal_updatedatainfotable.h"
#include "public_typedef.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

typedef CHashList<RoleID, MaxUserCountPerRoom, RoomPlayerBucketSize> RoomPlayerList;

class CRoom : public CObject
{
public:

	CRoom()
	{
		Reset();
	}

	virtual ~CRoom()
	{

	}

	int32_t Initialize()
	{
		Reset();
		return m_sRoomPlayerList.Initialize();
	}

	int32_t Resume()
	{
		Reset();
		return m_sRoomPlayerList.Resume();
	}

	int32_t Uninitialize()
	{
		Reset();
		return m_sRoomPlayerList.Uninitialize();
	}

	int32_t AddPlayer(const RoleID nRoleID);

	int32_t DeletePlayer(const RoleID nRoleID);

	bool IsPlayerInRoom(const RoleID nRoleID);

	int32_t GetAllPlayers(RoleID arrRoleID[], const int32_t arrSize, int32_t &nPlayerCount);

	int32_t DeleteAllPlayers();

	void SetRegionID(const RegionID nRegionID)
	{
		m_nRegionID = nRegionID;
		UpdateDataInfo(GetMemberOffset(m_nRegionID), sizeof(m_nRegionID));
	}

	RegionID GetRegionID() const
	{
		return m_nRegionID;
	}

	void SetChannelID(const ChannelID nChannelID)
	{
		m_nChannelID = nChannelID;
		UpdateDataInfo(GetMemberOffset(m_nChannelID), sizeof(m_nChannelID));
	}

	ChannelID GetChannelID() const
	{
		return m_nChannelID;
	}

	RoleID GetHostID() const
	{
		return m_nHostID;
	}

	void SetHostID(const RoleID nRoleID)
	{
		m_nHostID = nRoleID;
		UpdateDataInfo(GetMemberOffset(m_nHostID), sizeof(m_nHostID));
	}

	int32_t GetCurUserCount() const
	{
		return m_sRoomPlayerList.ObjectCount();;
	}

	int32_t GetCurVisitorCount() const
	{
		return m_nCurVisitorCount;
	}

	int32_t GetTotalPeopleCount() const
	{
		return m_nCurUserCount + m_nCurVisitorCount;
	}

	int32_t GetMaxShowUserLimit() const
	{
		return m_nMaxShowUserLimit;
	}

	int32_t GetMaxRealUserLimit() const
	{
		return m_nMaxRealUserLimit;
	}

	void SetRoomType(RoomType nRoomType)
	{
		m_nRoomType = nRoomType;
		UpdateDataInfo(GetMemberOffset(m_nRoomType), sizeof(m_nRoomType));
	}
	RoomType GetRoomType() const
	{
		return m_nRoomType;
	}

	void SetRoomID(const RoomID nRoomID)
	{
		m_nRoomID = nRoomID;
		UpdateDataInfo(GetMemberOffset(m_nRoomID), sizeof(m_nRoomID));
	}

	RoomID GetRoomID() const
	{
		return m_nRoomID;
	}

	const char *GetRoomName()
	{
		return m_strRoomName.GetString();
	}

	void SetRoomName(const char *strRoomName)
	{
		m_strRoomName =  strRoomName;
		UpdateDataInfo(GetMemberOffset(m_strRoomName), sizeof(m_strRoomName));
	}

	void SetRoomServerID(const RoomID nRoomServerID)
	{
		m_nRoomServerID = nRoomServerID;
		UpdateDataInfo(GetMemberOffset(m_nRoomServerID), sizeof(m_nRoomServerID));
	}

	ServerID GetRoomServerID() const
	{
		return m_nRoomServerID;
	}

	int32_t	 GetRobotCount()
	{
		return m_nRobotCount;
	}

	void 	SetRobotCount(int32_t nRobotCount)
	{
		m_nRobotCount = nRobotCount;
		UpdateDataInfo(GetMemberOffset(m_nRobotCount), sizeof(m_nRobotCount));
	}

	int32_t	 GetTempAdminCount()
	{
		return m_nTempAdminCount;
	}

	void	 SetTempAdminCount(int32_t nTempAdminCount)
	{
		m_nTempAdminCount = nTempAdminCount;
		UpdateDataInfo(GetMemberOffset(m_nTempAdminCount), sizeof(m_nTempAdminCount));
	}

	uint32_t GetStartPos()
	{
		return (uint32_t)offsetof(CRoom, m_nStartPos);
	}
	uint32_t GetEndPos()
	{
		return (uint32_t)offsetof(CRoom, m_nEndPos);
	}
	bool IsHaveUpdateData()
	{
		return m_nUpdateDataInfoCount > 0;
	}
	void SetTunnelServerID(int32_t nTunnelServerID)
	{
		m_nTunnelServerID = nTunnelServerID;
	}
	int32_t GetTunnelServerID()
	{
		return m_nTunnelServerID;
	}
protected:
	void Reset()
	{
		m_nRoomServerID = enmInvalidServerID;
		m_nRegionID = enmInvalidRegionID;
		m_nChannelID = enmInvalidChannelID;
		m_nRoomID = enmInvalidRoomID;
		m_nRoomType = enmInvalidRoomType;
		m_nHostID = enmInvalidRoleID;
		m_nCurUserCount = 0;
		m_nCurVisitorCount = 0;
		m_nMaxShowUserLimit = MaxDefaultShowUserLimit;
		m_nMaxRealUserLimit = MaxDefaultRealUserLimit;
		m_sRoomPlayerList.Initialize();
		m_nRobotCount = 0;
		m_nTempAdminCount = 0;
		m_nUpdateDataInfoCount = 0;
//		memset(m_arrUpdateDataInfoTable,0,sizeof(m_arrUpdateDataInfoTable));
		m_nTunnelServerID = enmInvalidServerID;
	}

private:
	RoomPlayerList::Key MakePlayerKey(const RoleID nRoleID) const;
	void UpdateDataInfo(uint32_t nOffset, uint32_t nDataSize);

public:
	int32_t GetUpdateDataInfoTable(uint8_t* arrUpdateData[],
			UpdateDataInfoTable arrUpdateTable[]);
public:
	//注意，不要在m_nStartPos前添加成员变量
	uint8_t								m_nStartPos;

private:
	ServerID							m_nRoomServerID;					//所在的roomserver
	RegionID							m_nRegionID;						//分区ID
	ChannelID							m_nChannelID;						//频道ID
	RoomID								m_nRoomID;							//房间ID
	CString<MaxRoomNameLength> 			m_strRoomName;						//房间的名称
	RoomType							m_nRoomType;						//房间的类型
	RoleID								m_nHostID;							//室主roleid
	int32_t								m_nCurVisitorCount;					//当前房间游客数量
	int32_t								m_nMaxShowUserLimit;				//客户端显示的最大用户上限
	int32_t								m_nMaxRealUserLimit;				//实际真实可容纳的用户上限

	int32_t								m_nRobotCount;						//房间机器人数量
	int32_t								m_nTempAdminCount;					//房间临管数量

public:
	//注意，这个值只是标记作用，所有基本数据类型都要在此成员变量之前定义
	uint8_t								m_nEndPos;

private:
	int32_t								m_nCurUserCount;					//当前房间用户数量（所有人，包括游客）
	RoomPlayerList						m_sRoomPlayerList;					//玩家对象池
	ServerID							m_nTunnelServerID;

private:
	int32_t								m_nUpdateDataInfoCount;
	UpdateDataInfoTable					m_arrUpdateDataInfoTable[UpdateDataInfoTableSize];
};


FRAME_HALLSERVER_NAMESPACE_END

CCOMPLEXTYPE_SPECIFICALLY(FRAME_HALLSERVER_NAMESPACE::CRoom)

#endif /* DAL_ROOM_H_ */
