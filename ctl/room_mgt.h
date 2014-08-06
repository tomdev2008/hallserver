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

//���巿�����غ���������
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

	//��ʼ�����������
	virtual int32_t Initialize();
	//�ָ����������
	virtual int32_t Resume();
	//ע�����������
	virtual int32_t Uninitialize();

	int32_t CreateRoom(const RoomID nRoomID, CRoom *&pRoom);

	int32_t GetRoom(const RoomID nRoomID, CRoom *&pRoom);

	int32_t DestoryRoom(const RoomID nRoomID);

	int32_t Clear();

	//��ȡ���������
	uint32_t GetCapacity() const;
	//��ȡ������ж�������
	uint32_t GetRoomCount() const;
	//������Ƿ�Ϊ��
	bool IsEmpty() const;
	//������Ƿ�����
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
