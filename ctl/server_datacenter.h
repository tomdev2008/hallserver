/*
 * server_datacenter.h
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#ifndef SERVER_DATACENTER_H_
#define SERVER_DATACENTER_H_

#include "common/common_object.h"
#include "common/common_memory.h"
#include "common/common_singleton.h"
#include "common/common_datetime.h"
#include "def/server_namespace.h"
#include "lightframe_impl.h"
#include "dal/dal_region.h"
#include "public_typedef.h"

#include "player_mgt.h"
#include "region_mgt.h"
#include "room_mgt.h"
#include "media_mgt.h"
#include "def/dal_def.h"
#include "main_frame.h"
#include <sys/time.h>

//#include "ipmacmap_mgt.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

//向hall提交定时更新信息的server个数
#define MaxServerInfoCount		4

class ServerInfo
{
public:
	EntityType			nEntityType;
	ServerID			nServerID;
	int32_t				nCurPlayerCount;
	RoleID				arrRoleID[MaxOnlinePlayerCount];

	void Reset()
	{
		nEntityType = enmInvalidEntityType;
		nServerID = enmInvalidServerID;
		nCurPlayerCount = 0;
		for(int32_t i = 0; i < MaxOnlinePlayerCount; ++i)
		{
			arrRoleID[i] = enmInvalidRoleID;
		}
	}

	ServerInfo()
	{
		Reset();
	}
};

struct MaxOnlineInfo
{
	uint32_t nMaxOnlineTotalNum;
	uint32_t nMaxOnlineRealPlayerNum;
	CDateTime dMaxOnlineDate;
	MaxOnlineInfo()
	{
		nMaxOnlineTotalNum = 0;
		nMaxOnlineRealPlayerNum = 0;
		dMaxOnlineDate = CDateTime::CurrentDateTime();
	}
	void UpdateMaxOnlineInfo(uint32_t nTotalNum, uint32_t nRealNum, CDateTime& dDate)
	{
		nMaxOnlineTotalNum = nTotalNum;
		nMaxOnlineRealPlayerNum = nRealNum;
		dMaxOnlineDate = dDate;
	}
	void UpdateTotalOnlineNum(uint32_t nTotalNum, CDateTime& dDate)
	{
		nMaxOnlineTotalNum = nTotalNum;
		dMaxOnlineDate = dDate;
	}
	void UpdateRealOnlinePlayerNum(uint32_t nRealNum, CDateTime& dDate)
	{
		nMaxOnlineRealPlayerNum = nRealNum;
		dMaxOnlineDate = dDate;
	}
};

struct TunnelServerInfo
{
	int32_t nServerID;		  //tunnel server id
	uint32_t nRoomTotalNum;	  //room对象总数
	uint32_t nPlayerTotalNum; //player对象总数
	uint32_t nRoomBuildNum;	  //已经恢复的room对象总数
	uint32_t nPlayerBuildNum; //已经恢复的player对象总数
	bool	 bInitialized;
	void Initialize()
	{
		nServerID = enmInvalidServerID;
		nRoomTotalNum = 0;
		nPlayerTotalNum = 0;
		nRoomBuildNum = 0;
		nPlayerBuildNum = 0;
		bInitialized = false;
	}
};

class CServerDataCenter : public IDataCenter
{
public:
	CServerDataCenter();
	virtual ~CServerDataCenter();

public:
	virtual const char* GetName();

	//向Frame提供自己所需内存大小的查询接口
	virtual size_t GetSize();

	//获取共享内存Key(返回0表示由Frame生成)
	virtual int32_t GetKey(){return 0;}

	//初始化接口调用的时候 共享内存已分配
	virtual int32_t Initialize();
	virtual int32_t Resume();
	virtual int32_t Uninitialize();

public:
	//获取玩家管理器对象
	CPlayerMgt& GetPlayerMgt()
	{
		return *m_pPlayerMgt;
	}

	CRegionMgt& GetRegionMgt()
	{
		return *m_pRegionMgt;
	}

	CRoomMgt& GetRoomMgt()
	{
		return *m_pRoomMgt;
	}

	CMediaMgt& GetMediaMgt()
	{
		return *m_pMediaMgt;
	}

	MaxOnlineInfo& GetMaxOnlineInfo()
	{
		return m_MaxOnlineInfo;
	}

	ServerStatus& GetServerStatus()
	{
		return m_nServerStatus;
	}

	uint32_t& GetTunnelServerNum()
	{
		return m_nTunnelServerNum;
	}
	TunnelServerInfo* GetTunnServerArray()
	{
		return m_arrTunnelServerInfo;
	}

//	CIpMacMapMgt& GetIpMacMapMgt()
//	{
//		return *m_pIpMacMapMgt;
//	}
//	uint32_t& GetRoomTotalNum()
//	{
//		return m_nRoomTotalNum;
//	}
//
//	uint32_t& GetPlayerTotalNum()
//	{
//		return m_nPlayerTotalNum;
//	}

	int32_t GetAllPlayers(const RoomID nRoomID, CPlayer *arrPlayer[],
			const int32_t arrSize, int32_t &nPlayerCount);

	int32_t GetAllPlayers(CRoom *pRoom, CPlayer *arrPlayer[],
			const int32_t arrSize, int32_t &nPlayerCount);

	int32_t PopPlayers(EntityType nEntityType, ServerID nServerID, CPlayer *arrPlayer[],
			int32_t arrSize, int32_t &nPlayerCount);

	int32_t PushPlayers(EntityType nEntityType, ServerID nServerID, RoleID arrRoleID[],
			int32_t arrSize, int32_t nPlayerCount);

	int32_t AddTunnel(int32_t nTunnelServerID, uint32_t nRoomNum = 0, uint32_t nPlayerNum = 0);
	int32_t AddTunnelBuildNum(int32_t nTunnelServerID, uint32_t nRoomBuildNum, uint32_t nPlayerBuildNum);
	int32_t DelTunnel(int32_t nTunnelServerID);
	int32_t GetTunnelID(RoomID nRoomID, int32_t& nTunnelServerID);
	//int32_t GetTunnelTotalNum(int32_t nTunnelServerID, uint32_t& nRoomNum, uint32_t& nPlayerNum);
	//int32_t GetTunnelBuildNum(int32_t nTunnelServerID, uint32_t& nRoomBuildNum, uint32_t& nPlayerBuildNum);
	int32_t GetTunnelServerIndex(int32_t nTunnelServerID, uint32_t& nIndex);

protected:

	CPlayerMgt				*m_pPlayerMgt;								//玩家管理器对象
	CRegionMgt				*m_pRegionMgt;								//分区管理器对象
	CRoomMgt				*m_pRoomMgt;								//房间管理器对象
	CMediaMgt				*m_pMediaMgt;								//媒体管理器对象

	ServerInfo				m_arrServerInfo[MaxServerInfoCount];

	uint32_t				m_nTunnelServerNum;							//tunnel server 数
	TunnelServerInfo		m_arrTunnelServerInfo[MaxTunnelNum];		//tunnel server id列表

	MaxOnlineInfo			m_MaxOnlineInfo;							//最大在线人数与时间点

	ServerStatus			m_nServerStatus;							//server状态

	//uint32_t 				m_nRoomTotalNum;							//需要恢复的room对象总数
	//uint32_t				m_nPlayerTotalNum;							//需要恢复的player对象总数
//	CIpMacMapMgt			*m_pIpMacMapMgt;							//IP、RoleID映射管理器
};


#define	CREATE_DALDATACENTER_INSTANCE		CSingleton<CServerDataCenter>::CreateInstance
#define	GET_DALDATACENTER_INSTANCE			CSingleton<CServerDataCenter>::GetInstance
#define	DESTROY_DALDATACENTER_INSTANCE		CSingleton<CServerDataCenter>::DestroyInstance

#define g_DataCenter		GET_DALDATACENTER_INSTANCE()
#define g_PlayerMgt			GET_DALDATACENTER_INSTANCE().GetPlayerMgt()
#define g_RegionMgt			GET_DALDATACENTER_INSTANCE().GetRegionMgt()
#define g_RoomMgt			GET_DALDATACENTER_INSTANCE().GetRoomMgt()
#define g_MediaMgt			GET_DALDATACENTER_INSTANCE().GetMediaMgt()
#define g_MaxOnlineInfo		GET_DALDATACENTER_INSTANCE().GetMaxOnlineInfo()
#define g_TunnelServerNum	GET_DALDATACENTER_INSTANCE().GetTunnelServerNum()
#define g_TunnelServerArray	GET_DALDATACENTER_INSTANCE().GetTunnServerArray()
#define g_ServerStatus		GET_DALDATACENTER_INSTANCE().GetServerStatus()

//#define g_IpMacMapMgt		GET_DALDATACENTER_INSTANCE().GetIpMacMapMgt()

#define VerifyTunnel() \
do{ \
	bool bAllInitialized = true; \
	for(uint32_t i = 0; i < g_TunnelServerNum; i++) \
	{ \
		bAllInitialized = bAllInitialized & g_TunnelServerArray[i].bInitialized; \
	} \
	if(bAllInitialized) \
	{ \
		g_ServerStatus = enmServerStatus_Running; \
	} \
}while(0)

FRAME_HALLSERVER_NAMESPACE_END

#endif /* SERVER_DATACENTER_H_ */
