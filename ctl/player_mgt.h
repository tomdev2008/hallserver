/*
 * player_mgt.h
 *
 *  Created on: 2011-11-30
 *      Author: jimm
 */

#ifndef PLAYER_MGT_H_
#define PLAYER_MGT_H_

#include "common/common_pool.h"
#include "common/common_hashlist.h"
#include "dal/dal_player.h"
#include "def/server_namespace.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

//������Ҷ���غ���������
typedef CPool<CPlayer, MaxOnlinePlayerCount>	PlayerPool;
typedef CHashList<PlayerIndex, MaxOnlinePlayerCount, enmPlayerBucketSize>	PlayerList;

class CPlayerMgt : public CObject
{
protected:
	enum
	{
		enmAdditionalIndex_HashListIndex			= 0,
	};
public:
	CPlayerMgt();
	virtual ~CPlayerMgt();

	//��ʼ����ҹ�����
	virtual int32_t Initialize();
	//�ָ���ҹ�����
	virtual int32_t Resume();
	//ע����ҹ�����
	virtual int32_t Uninitialize();

	int32_t CreatePlayer(const RoleID nRoleID, CPlayer *&pPlayer);

	int32_t GetPlayer(const RoleID nRoleID, CPlayer *&pPlayer);

	int32_t GetAllPlayer(CPlayer *arrPlayerObject[], const int32_t arrSize, int32_t &nPlayerCount, bool isNeedRobot = true);

	int32_t GetAllPlayer(RoleID arrRoleID[], const int32_t arrSize, int32_t &nPlayerCount, bool isNeedRobot = true);

	int32_t DestoryPlayer(const RoleID nRoleID);

	int32_t Clear();

	//��ȡ���������
	uint32_t GetCapacity() const;
	//��ȡ������ж�������
	uint32_t GetPlayerCount() const;
	//������Ƿ�Ϊ��
	bool IsEmpty() const;
	//������Ƿ�����
	bool IsFull() const;

	uint32_t GetRealPlayerCount()
	{
		return m_nRealPlayerCount;
	}

	void	SetRealPlayerCount(uint32_t nCount)
	{
		m_nRealPlayerCount = nCount;
	}

	PlayerPool::CIndex * First() { return m_sPlayerPool.First(); }
protected:
	int32_t CreatePlayer(const RoleID nRoleID, PlayerPool::CIndex *&pIndex);

	int32_t GetPlayer(const RoleID nRoleID, PlayerPool::CIndex*& pIndex);

protected:
	PlayerList::Key MakeUserKey(const RoleID nRoleID) const;
private:
	PlayerPool					m_sPlayerPool;
	PlayerList					m_sPlayerList;
	uint32_t					m_nRealPlayerCount;
};

FRAME_HALLSERVER_NAMESPACE_END

#endif /* PLAYER_MGT_H_ */
