/*
 * dal_player.h
 *
 *  Created on: 2011-11-30
 *      Author: jimm
 */

#ifndef DAL_PLAYER_H_
#define DAL_PLAYER_H_

#include "common/common_complextype.h"
#include "common/common_object.h"
#include "common/common_string.h"
#include "frame_typedef.h"
#include "frame_protocolhead.h"
#include "def/server_namespace.h"
#include "public_typedef.h"
#include "def/server_errordef.h"
#include "dal_updatedatainfotable.h"
#include "ctl/update_data_ctl.h"
#include "config/someconfig.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

static RoleRank nTempRoleRank = enmRoleRank_None;
class PlayerRoomInfo
{
public:
	ServerID		nServerID;			//所在的roomserverid
	RoomID			nRoomID;			//进入的房间
	PlayerState		nPlayerState;		//在房间的状态

	PlayerRoomInfo()
	{
		Reset();
	}

	void Reset()
	{
		nRoomID			= enmInvalidRoomID;
		nPlayerState	= enmInvalidPlayerState;
		nServerID		= enmInvalidServerID;
	}
};

class CPlayer : public CObject
{
public:
	CPlayer()
	{
		Reset();
	}

	virtual ~CPlayer()
	{

	}

	int32_t Initialize()
	{
		Reset();
		return S_OK;
	}

	int32_t Resume()
	{
		Reset();
		return S_OK;
	}

	int32_t Uninitialize()
	{
		Reset();
		return S_OK;
	}

	int32_t GetCurEnterRoomCount() const
	{
		return m_nCurRoomCount;
	}

	void GetAllEnterRoom(RoomID arrRoomID[], const int32_t arrSize, int32_t &nGetCurRoomCount)
	{
		nGetCurRoomCount = m_nCurRoomCount > arrSize ? arrSize : m_nCurRoomCount;
		for(int32_t i = 0; i < nGetCurRoomCount; ++i)
		{
			arrRoomID[i] = m_arrPlayerRoomInfo[i].nRoomID;
		}
	}

	void GetAllAdminRoom(RoomID arrRoomID[], const int32_t arrSize, int32_t &nGetAdminCount)
	{
		nGetAdminCount = m_nAdminCount > arrSize ? arrSize : m_nAdminCount;
		for(int32_t i = 0; i < nGetAdminCount; ++i)
		{
			arrRoomID[i] = m_arrRoomIDS[i];
		}
	}

	void GetPlayerRoomInfo(PlayerRoomInfo arrPlayerRoomInfo[], const int32_t arrSize, int32_t &nGetInfoCount)
	{
		nGetInfoCount = m_nCurRoomCount > arrSize ? arrSize : m_nCurRoomCount;
		memcpy(arrPlayerRoomInfo, m_arrPlayerRoomInfo, sizeof(PlayerRoomInfo) * nGetInfoCount);
	}

	void GetAllPlayerStateInAllRoom(PlayerState arrPlayerState[], const int32_t arrSize, int32_t &nGetPlayerStateCount)
	{
		nGetPlayerStateCount = m_nCurRoomCount > arrSize ? arrSize : m_nCurRoomCount;
		for(int32_t i = 0; i < nGetPlayerStateCount; ++i)
		{
			arrPlayerState[i] = m_arrPlayerRoomInfo[i].nPlayerState;
		}
	}

	void SetRoleID(const RoleID nRoleID)
	{
		m_nRoleID = nRoleID;
		UpdateDataInfo(GetMemberOffset(m_nRoleID), sizeof(m_nRoleID));
	}

	RoleID GetRoleID() const
	{
		return m_nRoleID;
	}

	void SetRoleName(const char *strRoleName)
	{
		m_strRoleName = strRoleName;
		UpdateDataInfo(GetMemberOffset(m_strRoleName), sizeof(m_strRoleName));
	}

	const char *GetRoleName()
	{
		return m_strRoleName.GetString();
	}

	void SetAccountID(AccountID nAccountID)
	{
		m_nAccountID = nAccountID;
		UpdateDataInfo(GetMemberOffset(m_nAccountID), sizeof(m_nAccountID));
	}

	AccountID GetAccountID()
	{
		return m_nAccountID;
	}

	void SetVipLevel(VipLevel nVipLevel)
	{
		m_nVipLevel = nVipLevel;
		UpdateDataInfo(GetMemberOffset(m_nVipLevel), sizeof(m_nVipLevel));
	}

	VipLevel GetVipLevel()
	{
		return m_nVipLevel;
	}

	void SetUserLevel(UserLevel nUserLevel)
	{
		m_nUserLevel = nUserLevel;
		UpdateDataInfo(GetMemberOffset(m_nUserLevel), sizeof(m_nUserLevel));
	}

	UserLevel GetUserLevel()
	{
		return m_nUserLevel;
	}

	void SetPlayerGender(const Gender nGender)
	{
		m_nPlayerGender = nGender;
		UpdateDataInfo(GetMemberOffset(m_nPlayerGender), sizeof(m_nPlayerGender));
	}

	Gender GetPlayerGender() const
	{
		return m_nPlayerGender;
	}

	void SetLoginTime(const int64_t nLoginTime)
	{
		m_nLoginTime = nLoginTime;
		UpdateDataInfo(GetMemberOffset(m_nLoginTime), sizeof(m_nLoginTime));
	}

	int64_t GetLoginTime() const
	{
		return m_nLoginTime;
	}

	void SetConnInfo(const ConnUin &stInfo)
	{
		m_stConnInfo = stInfo;
		UpdateDataInfo(GetMemberOffset(m_stConnInfo), sizeof(ConnUin));
	}

	const ConnUin &GetConnInfo()
	{
		return m_stConnInfo;
	}

	void SetOnlineTime(uint64_t nOnlineTime)
	{
		m_nOnlineTime = nOnlineTime;
		UpdateDataInfo(GetMemberOffset(m_nOnlineTime), sizeof(m_nOnlineTime));
	}

	uint64_t GetOnlineTime()
	{
		return m_nOnlineTime;
	}

	void SetLeftMoney(uint32_t nLeftMoney)
	{
		m_nLeftMoney = nLeftMoney;
		UpdateDataInfo(GetMemberOffset(m_nLeftMoney), sizeof(m_nLeftMoney));
	}

	uint32_t GetLeftMoney()
	{
		return m_nLeftMoney;
	}

	void SetExperience(int32_t nExperience)
	{
		m_nExperience = nExperience;
		UpdateDataInfo(GetMemberOffset(m_nExperience), sizeof(m_nExperience));
	}

	int32_t GetExperience()
	{
		return m_nExperience;
	}

	void SetLastVersion(uint32_t nLastVersion)
	{
		m_nLastVersion = nLastVersion;
		UpdateDataInfo(GetMemberOffset(m_nLastVersion), sizeof(m_nLastVersion));
	}

	uint32_t GetLastVersion()
	{
		return m_nLastVersion;
	}

	void SetMagnateLevel(MagnateLevel nMagnateLevel)
	{
		m_nMagnateLevel = nMagnateLevel;
		UpdateDataInfo(GetMemberOffset(m_nMagnateLevel), sizeof(m_nMagnateLevel));
	}

	MagnateLevel GetMagnateLevel()
	{
		return m_nMagnateLevel;
	}

	void SetConsume(uint32_t nConsume)
	{
		m_nConsume = nConsume;
		UpdateDataInfo(GetMemberOffset(m_nConsume), sizeof(m_nConsume));
	}

	uint64_t GetConsume()
	{
		return m_nConsume;
	}
	void SetNextLevelMoney(uint32_t nNextLevelMoney)
	{
		m_nNextLevelMoney = nNextLevelMoney;
	}
	uint64_t GetNextLevelMoney()
	{
		return m_nNextLevelMoney;
	}

/*获取新加字段*/
	int32_t GetPlayerAdminCount()
	{
		return m_nAdminCount;
	}

	RoleRank GetAdminRoleRank(RoomID nRoomID)
	{
		RoleRank nRoleRank = enmRoleRank_None;
		for(int32_t i = 0; i < MaxBeAdminPerPlayer; ++i)
		{
			if(m_arrRoomIDS[i] == nRoomID)
			{
				nRoleRank = m_arrRoleRank[i];
				break;
			}
		}
		return nRoleRank;
	}

/*设置新加字段*/
	int32_t SetPlayerAdminCount(int32_t nAdminCount)
	{
		if(nAdminCount > MaxBeAdminPerPlayer)
		{
			return E_HS_ADMINOUTOFBOUND;
		}
		m_nAdminCount = nAdminCount;
		UpdateDataInfo(GetMemberOffset(m_nAdminCount), sizeof(m_nAdminCount));
		return S_OK;
	}

	//添加新的房间权限记录并更新数目
	int32_t SetAdminRoleRank(RoomID nRoomID, RoleRank nRoleRank ,RoleRank &nPreRoleRank = nTempRoleRank)
	{
		for(int32_t i = 0; i < MaxBeAdminPerPlayer; ++i)
		{
			if(m_arrRoomIDS[i] == nRoomID)
			{
				nPreRoleRank = m_arrRoleRank[i];
				if(nRoleRank == enmRoleRank_None)
				{
					m_arrRoleRank[i] = m_arrRoleRank[m_nAdminCount - 1];
					m_arrRoomIDS[i] = m_arrRoomIDS[m_nAdminCount - 1];
					m_arrRoleRank[m_nAdminCount - 1] = enmRoleRank_None;
					m_arrRoomIDS[m_nAdminCount - 1] = enmInvalidRoomID;
					UpdateDataInfo(GetMemberOffset(m_arrRoleRank[i]), sizeof(m_arrRoleRank[i]));
					UpdateDataInfo(GetMemberOffset(m_arrRoomIDS[i]), sizeof(m_arrRoomIDS[i]));
					UpdateDataInfo(GetMemberOffset(m_arrRoleRank[m_nAdminCount - 1]), sizeof(m_arrRoleRank[m_nAdminCount - 1]));
					UpdateDataInfo(GetMemberOffset(m_arrRoomIDS[m_nAdminCount - 1]), sizeof(m_arrRoomIDS[m_nAdminCount - 1]));
					--m_nAdminCount;
					UpdateDataInfo(GetMemberOffset(m_nAdminCount), sizeof(m_nAdminCount));
					return S_OK;
				}
				else
				{
					m_arrRoleRank[i] = nRoleRank;
					UpdateDataInfo(GetMemberOffset(m_arrRoleRank[i]), sizeof(m_arrRoleRank[i]));
					return S_OK;
				}
			}
		}
		if((m_nAdminCount < MaxBeAdminPerPlayer) && (m_arrRoomIDS[m_nAdminCount] == enmInvalidRoomID))
		{
			nPreRoleRank = m_arrRoomIDS[m_nAdminCount];
			m_arrRoomIDS[m_nAdminCount] = nRoomID;
			m_arrRoleRank[m_nAdminCount] = nRoleRank;
			UpdateDataInfo(GetMemberOffset(m_arrRoleRank[m_nAdminCount]), sizeof(m_arrRoleRank[m_nAdminCount]));
			UpdateDataInfo(GetMemberOffset(m_arrRoomIDS[m_nAdminCount]), sizeof(m_arrRoomIDS[m_nAdminCount]));
			m_nAdminCount++;
			UpdateDataInfo(GetMemberOffset(m_nAdminCount), sizeof(m_nAdminCount));
			return S_OK;
		}
		//超过最大管理数返回E_HS_ADMINOUTOFBOUND
		return E_HS_ADMINOUTOFBOUND;
	}

	//判断玩家是否有资格添加权限
	int32_t IsCanAddTitle(RoomID nRoomID, RoleRank nAddRoleRank)
	{
		RoleRank nRoleRank = GetAdminRoleRank(nRoomID);
		switch(nAddRoleRank)
		{
		case enmRoleRank_MinorHost:
			if(nRoleRank >= enmRoleRank_MinorHost)
			{
				return E_HS_NOPERMISSIONS;
			}
			break;
		case enmRoleRank_Admin:
			if(nRoleRank >= enmRoleRank_Admin)
			{
					return E_HS_NOPERMISSIONS;
			}
			break;
		case enmRoleRank_TempAdmin:
			if(nRoleRank >= enmRoleRank_TempAdmin)
			{
					return E_HS_NOPERMISSIONS;
			}
			break;
		default:
			return E_HS_NOTEXISTROLERANK;
			break;
		}
		if(m_nVipLevel <= enmVipLevel_NONE && !IsRobot())
		{
			return E_HS_NOPERMISSIONS;
		}
		uint16_t  nVipAdminCount = g_AboutRoomConfig.GetVipAdminCount(m_nVipLevel);
		uint16_t  nIdentityAdmCount = g_AboutRoomConfig.GetIdentityAdminCount(m_nIdentityType);
		uint16_t  nMaxRealAdmCount = nVipAdminCount;
		if (nIdentityAdmCount != 0)
		{
			nMaxRealAdmCount = nIdentityAdmCount;
		}
		uint16_t  nAdmCount = 0;
		for(int32_t i = 0; i < MaxBeAdminPerPlayer; ++i)
		{
			if(m_arrRoomIDS[i] == nRoomID)
			{
				continue;
			}
			//加临管的时候不判断在其他房间是否为管理员
			else if(m_arrRoleRank[i] != enmRoleRank_None&&nAddRoleRank!=enmRoleRank_TempAdmin&&m_arrRoleRank[i]!=enmRoleRank_TempAdmin)
			{
				nAdmCount++;
			}
		}
		if (nAdmCount >= nMaxRealAdmCount)
		{
			return E_HS_ISOTHERROOMADMIN;
		}
		return S_OK;
	}

	void SetPlayerRobot()
	{
		m_nPlayerType = enmPlayerType_Rebot;
		UpdateDataInfo(GetMemberOffset(m_nPlayerType), sizeof(m_nPlayerType));
	}

	bool IsRobot()
	{
		if(m_nPlayerType == enmPlayerType_Rebot)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool CanEnterRoom() const
	{
		return !(m_nCurRoomCount >= MaxEnterRoomCount);
	}

	void EnterRoom(const RoomID nRoomID, const ServerID nServerID)
	{
		if(!CanEnterRoom())
		{
			//todo : write something
			return;
		}

		m_arrPlayerRoomInfo[m_nCurRoomCount].nRoomID = nRoomID;
		m_arrPlayerRoomInfo[m_nCurRoomCount].nServerID = nServerID;
		m_arrPlayerRoomInfo[m_nCurRoomCount].nPlayerState = enmPlayerState_InRoom;

		UpdateDataInfo(GetMemberOffset(m_arrPlayerRoomInfo[m_nCurRoomCount]), sizeof(m_arrPlayerRoomInfo[m_nCurRoomCount]));
		++m_nCurRoomCount;
		UpdateDataInfo(GetMemberOffset(m_nCurRoomCount), sizeof(m_nCurRoomCount));
	}

	void ExitRoom(const RoomID nRoomID)
	{
		for(int32_t i = 0; i < m_nCurRoomCount; ++i)
		{
			if(nRoomID == m_arrPlayerRoomInfo[i].nRoomID)
			{
				//将最后一位填充删除的位置
				memcpy(&m_arrPlayerRoomInfo[i], &m_arrPlayerRoomInfo[m_nCurRoomCount - 1], sizeof(PlayerRoomInfo));

				m_arrPlayerRoomInfo[m_nCurRoomCount - 1].Reset();

				UpdateDataInfo(GetMemberOffset(m_arrPlayerRoomInfo[i]), sizeof(m_arrPlayerRoomInfo[i]));
				UpdateDataInfo(GetMemberOffset(m_arrPlayerRoomInfo[m_nCurRoomCount - 1]), sizeof(m_arrPlayerRoomInfo[m_nCurRoomCount - 1]));

				if(m_nCurRoomCount > 0)
				{
					--m_nCurRoomCount;
					UpdateDataInfo(GetMemberOffset(m_nCurRoomCount), sizeof(m_nCurRoomCount));
				}

				break;
			}
		}
	}

	void SetIdentityType(const IdentityType nIdentityType)
	{
		m_nIdentityType = nIdentityType;
		UpdateDataInfo(GetMemberOffset(m_nIdentityType), sizeof(m_nIdentityType));
	}

	IdentityType GetIdentityType()
	{
		return m_nIdentityType;
	}
	bool IsOfficialPlayer()
	{
		return ((m_nIdentityType&enmIdentityType_OPERATOR)==enmIdentityType_OPERATOR)||((m_nIdentityType&enmIdentityType_PATROLMAN)==enmIdentityType_PATROLMAN);
	}

	void SetPlayerState(PlayerState nPlayerState)
	{
		m_nPlayerState = nPlayerState;
		UpdateDataInfo(GetMemberOffset(m_nPlayerState), sizeof(m_nPlayerState));
	}
	PlayerState GetPlayerState()
	{
		return m_nPlayerState;
	}
	void SetNewPlayerRoomCount(uint32_t nNewPlayerRoomCount)
	{
		m_nCurNewPlayerRoomCount = nNewPlayerRoomCount;
		UpdateDataInfo(GetMemberOffset(m_nCurNewPlayerRoomCount), sizeof(m_nCurNewPlayerRoomCount));
	}
	uint32_t GetNewPlayerRoomCount()
	{
		return m_nCurNewPlayerRoomCount;
	}
	int32_t GetUpdateDataInfoTable(uint8_t* arrUpdateData[], UpdateDataInfoTable arrUpdateTable[])
	{
		//排序
		for(int32_t i = 1;i < m_nUpdateDataInfoCount;i++)
		{
			UpdateDataInfoTable dataTemp = m_arrUpdateDataInfoTable[i];
			int32_t j = i;
			while(j > 0 && m_arrUpdateDataInfoTable[j-1].nOffset>dataTemp.nOffset)
			{
				m_arrUpdateDataInfoTable[j] = m_arrUpdateDataInfoTable[j-1];
				--j;
			}
			m_arrUpdateDataInfoTable[j] = dataTemp;
		}

		//合并可以合并的
		uint32_t nOffsetTemp = m_arrUpdateDataInfoTable[0].nOffset;
		uint32_t nDatasizeTemp = m_arrUpdateDataInfoTable[0].nDataSize;
		int32_t nIndex = 0;

		uint8_t* pStartPos = &m_nStartPos;

		for(int32_t i = 1;i < m_nUpdateDataInfoCount;i++)
		{
			if(m_arrUpdateDataInfoTable[i].nOffset >= nOffsetTemp && m_arrUpdateDataInfoTable[i].nOffset <= nOffsetTemp+nDatasizeTemp)
			{
				if(nIndex >= 1)
				{
					nIndex--;
				}
				arrUpdateTable[nIndex].nOffset = nOffsetTemp;
			}
			else if(m_arrUpdateDataInfoTable[i].nOffset <= nOffsetTemp)
			{
				arrUpdateTable[nIndex].nOffset = m_arrUpdateDataInfoTable[i].nOffset;
			}
			else if(m_arrUpdateDataInfoTable[i].nOffset == nOffsetTemp+nDatasizeTemp)
			{
				if(nIndex >= 1)
				{
					nIndex--;
				}
				arrUpdateTable[nIndex].nOffset = nOffsetTemp;
				arrUpdateTable[nIndex].nDataSize = m_arrUpdateDataInfoTable[i].nDataSize+nDatasizeTemp;
				arrUpdateData[nIndex] = &pStartPos[arrUpdateTable[nIndex].nOffset];
				nIndex++;
				nOffsetTemp = arrUpdateTable[nIndex-1].nOffset;
				nDatasizeTemp = arrUpdateTable[nIndex-1].nDataSize;
				continue;
			}
			else if(m_arrUpdateDataInfoTable[i].nOffset > nOffsetTemp+nDatasizeTemp)
			{
				if(nIndex == 0 || arrUpdateTable[nIndex-1].nOffset != nOffsetTemp)
				{
					arrUpdateTable[nIndex].nOffset = nOffsetTemp;
					arrUpdateTable[nIndex].nDataSize = nDatasizeTemp;
					arrUpdateData[nIndex] = &pStartPos[arrUpdateTable[nIndex].nOffset];
					nIndex++;
				}
				arrUpdateTable[nIndex]= m_arrUpdateDataInfoTable[i];
				arrUpdateData[nIndex] = &pStartPos[arrUpdateTable[nIndex].nOffset];
				nIndex++;
				nOffsetTemp = arrUpdateTable[nIndex-1].nOffset;
				nDatasizeTemp = arrUpdateTable[nIndex-1].nDataSize;
				continue;
			}
			if((m_arrUpdateDataInfoTable[i].nOffset+m_arrUpdateDataInfoTable[i].nDataSize) > nOffsetTemp+nDatasizeTemp)
			{
				arrUpdateTable[nIndex].nDataSize= m_arrUpdateDataInfoTable[i].nOffset+m_arrUpdateDataInfoTable[i].nDataSize-nOffsetTemp;
			}
			else
			{
				arrUpdateTable[nIndex].nDataSize = nDatasizeTemp;
			}
			nOffsetTemp = arrUpdateTable[nIndex].nOffset;
			nDatasizeTemp = arrUpdateTable[nIndex].nDataSize;
			arrUpdateData[nIndex] = &pStartPos[arrUpdateTable[nIndex].nOffset];
			nIndex++;
		}

		if(m_nUpdateDataInfoCount == 1)
		{
			arrUpdateData[nIndex] = &pStartPos[m_arrUpdateDataInfoTable[0].nOffset];
			arrUpdateTable[nIndex] = m_arrUpdateDataInfoTable[0];
			nIndex++;
		}
		//清空
		m_nUpdateDataInfoCount = 0;
		memset(m_arrUpdateDataInfoTable,0,sizeof(m_arrUpdateDataInfoTable));

		return nIndex;
	}

	uint64_t GetPlayerMacAddr()
	{
		return m_nMacAddr;
	}

	void 	setPlayerMacAddr(uint64_t nMacAddr)
	{
		m_nMacAddr = nMacAddr;
		UpdateDataInfo(GetMemberOffset(m_nMacAddr), sizeof(m_nMacAddr));
	}
	uint32_t GetStartPos()
	{
		return (uint32_t)offsetof(CPlayer, m_nStartPos);
	}
	uint32_t GetEndPos()
	{
		return (uint32_t)offsetof(CPlayer, m_nEndPos);
	}
	bool IsHaveUpdateData()
	{
		return m_nUpdateDataInfoCount > 0;
	}
private:
	void UpdateDataInfo(uint32_t nOffset, uint32_t nDataSize)
	{
		for(int32_t i = 0; i < m_nUpdateDataInfoCount; ++i)
		{
			if(m_arrUpdateDataInfoTable[i].nOffset == nOffset)
			{
				m_arrUpdateDataInfoTable[i].nDataSize = nDataSize;
				return;
			}
		}

		if(m_nUpdateDataInfoCount < UpdateDataInfoTableSize)
		{
			m_arrUpdateDataInfoTable[m_nUpdateDataInfoCount].nOffset = nOffset;
			m_arrUpdateDataInfoTable[m_nUpdateDataInfoCount].nDataSize = nDataSize;

			++m_nUpdateDataInfoCount;
		}

		GET_UPDATEDATACTL_INSTANCE().AddPlayerNeedUpdate(m_nRoleID);
	}
	void Reset()
	{
		m_nRoleID = enmInvalidRoleID;
		m_strRoleName.Initialize();
		m_nAccountID = enmInvalidAccountID;
		m_nVipLevel = enmInvalidVipLevel;
		m_nUserLevel = 0;
		m_nPlayerGender = enmGender_Unknown;
		m_nCurRoomCount = 0;
//		memset(m_arrPlayerRoomInfo, 0, sizeof(m_arrPlayerRoomInfo));
		m_nLoginTime = 0;
		m_stConnInfo.Reset();
		m_nOnlineTime = 0;
		m_nLeftMoney = 0;
		m_nExperience = 0;
		m_nLastVersion = 0;
		m_nMagnateLevel = enmMagnateLevel_0;
		m_nConsume = 0;
		m_nNextLevelMoney = 0;
		/*处理新加字段*/
		m_nAdminCount = 0;
		memset(m_arrRoomIDS, enmInvalidRoomID, sizeof(m_arrRoomIDS));
		memset(m_arrRoleRank, enmRoleRank_None, sizeof(m_arrRoleRank));
		m_nPlayerType = enmPlayerType_Normal;
		m_nIdentityType = enmIdentityType_None;
		m_nPlayerState = enmPlayerState_InHall;
		m_nCurNewPlayerRoomCount = 0;

		m_nMacAddr = 0;
		m_nUpdateDataInfoCount = 0;
//		memset(m_arrUpdateDataInfoTable,0,sizeof(m_arrUpdateDataInfoTable));
	}
public:
	//注意，不要在m_nStartPos前添加成员变量
	uint8_t				m_nStartPos;
private:
	RoleID				m_nRoleID;
	CString<MaxRoleNameLength>	m_strRoleName;
	AccountID			m_nAccountID;
	VipLevel			m_nVipLevel;
	UserLevel			m_nUserLevel;
	Gender				m_nPlayerGender;
	int32_t				m_nCurRoomCount;									//当前进入的房间数
	PlayerRoomInfo		m_arrPlayerRoomInfo[MaxEnterRoomCount];
	int64_t				m_nLoginTime;										//登陆时间
	ConnUin				m_stConnInfo;
	uint64_t			m_nOnlineTime;
	uint32_t			m_nLeftMoney;
	int32_t				m_nExperience;
	uint32_t			m_nLastVersion;
	PlayerType			m_nPlayerType;
	MagnateLevel        m_nMagnateLevel;               //富豪等级
	uint64_t            m_nConsume;                    //玩家消费额
	uint64_t            m_nNextLevelMoney;             //下一个等级需要的消费额
	/*新加字段,用户管理权限和相关房间*/
	int32_t 			m_nAdminCount;										//管理的房间数
	RoomID          	m_arrRoomIDS[MaxBeAdminPerPlayer];		//管理的房间ID
	RoleRank        	m_arrRoleRank[MaxBeAdminPerPlayer];		//管理权限
	IdentityType        m_nIdentityType;//玩家身份
	PlayerState         m_nPlayerState;//玩家的状态(只保存一些全局的状态)
	uint32_t			m_nCurNewPlayerRoomCount;//当前进入的新手房数

	uint64_t			m_nMacAddr;		//玩家MAC地址

public:
	//注意，这个值只是标记作用，所有基本数据类型都要在此成员变量之前定义
	uint8_t				m_nEndPos;
public:
	int32_t				m_nUpdateDataInfoCount;//已经设置的变量个数
	UpdateDataInfoTable	m_arrUpdateDataInfoTable[UpdateDataInfoTableSize];//存变量偏移量和大小
};


FRAME_HALLSERVER_NAMESPACE_END

CCOMPLEXTYPE_SPECIFICALLY(FRAME_HALLSERVER_NAMESPACE::CPlayer)

#endif /* DAL_PLAYER_H_ */


