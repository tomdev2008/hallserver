/*
 * room_mgt.h
 *
 *  Created on: 2011-12-5
 *      Author: jimm
 */

#ifndef ROOM_MGT_H_
#define ROOM_MGT_H_

#include "common/common_pool.h"
#include "common/common_hashlist.h"
#include "dal/dal_room.h"
#include "def/server_namespace.h"
#include "public_typedef.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

//定义房间对象池和索引类型
typedef CPool<CRoom, MaxRoomCount>	RoomPool;
typedef CHashList<RoomIndex, MaxRoomCount, RoomBucketSize>	RoomList;

class CRoomMgt : public CObject
{
protected:
	enum
	{
		enmAdditionalIndex_HashListIndex			= 0,
	};
public:
	CRoomMgt();
	virtual ~CRoomMgt();

	//初始化房间管理器
	virtual int32_t Initialize();
	//恢复房间管理器
	virtual int32_t Resume();
	//注销房间管理器
	virtual int32_t Uninitialize();

	int32_t CreateRoom(const RoomID nRoomID, CRoom *&pRoom);

	int32_t GetRoom(const RoomID nRoomID, CRoom *&pRoom);

	int32_t DestoryRoom(const RoomID nRoomID);

	int32_t Clear();

	//获取对象池容量
	uint32_t GetCapacity() const;
	//获取对象池中对象数量
	uint32_t GetRoomCount() const;
	//对象池是否为空
	bool IsEmpty() const;
	//对象池是否已满
	bool IsFull() const;

	int32_t GetAllRoom(CRoom *arrRoomObject[], const int32_t arrSize, int32_t &nRoomObjectCount);

	int32_t DeleteAllPlayersInRoom(RoomID nRoomID);

	int32_t AddPlayer(RoomID nRoomID, RoleID nRoleID);

	int32_t DeletePlayer(RoomID nRoomID, RoleID nRoleID);

protected:
	int32_t CreateRoom(const RoomID nRoomID, RoomPool::CIndex *&pIndex);

	int32_t GetRoom(const RoomID nRoomID, RoomPool::CIndex*& pIndex);

protected:
	RoomList::Key MakeRoomKey(const RoomID nRoomID) const;
private:
	RoomPool					m_sRoomPool;
	RoomList					m_sRoomList;
};

FRAME_HALLSERVER_NAMESPACE_END


#endif /* ROOM_MGT_H_ */
