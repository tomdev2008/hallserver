/*
 *  update_data_ctl.h
 *
 *  To do：
 *  Created on: 	2012-7-10
 *  Author: 		luocj
 */

#ifndef UPDATE_DATA_CTL_H_
#define UPDATE_DATA_CTL_H_

#include "common/common_object.h"
#include "common/common_memory.h"
#include "common/common_singleton.h"
#include "def/server_namespace.h"
//#include "frame/impl/frame_bll_impl.h"
#include "public_typedef.h"
#include <vector>
#include <algorithm>

using namespace std;

FRAME_HALLSERVER_NAMESPACE_BEGIN

typedef vector<RoleID>  NeedUptateRoleIDVector;
typedef vector<RoomID>  NeedUptateRoomIDVector;

class CUpdateDataCtl
{
public:
	CUpdateDataCtl()
	{
//		m_arrNeedUptateRoleID.clear();
//		m_arrNeedUptateRoomID.clear();
	}
	virtual ~CUpdateDataCtl()
	{

	}
	bool IsHasPlayerNeedUpdate() const
	{
		return m_arrNeedUptateRoleID.size()>0;
	}
	bool IsHasRoomNeedUpdate() const
	{
		return m_arrNeedUptateRoomID.size()>0;
	}

	void AddPlayerNeedUpdate(const RoleID nRoleID)
	{
		if(find(m_arrNeedUptateRoleID.begin(),m_arrNeedUptateRoleID.end(),nRoleID)!= m_arrNeedUptateRoleID.end() )
		{
			return;
		}
		m_arrNeedUptateRoleID.push_back(nRoleID);
	}

	void AddRoomNeedUpdate(const RoomID nRoomID)
	{
		if(find(m_arrNeedUptateRoomID.begin(),m_arrNeedUptateRoomID.end(),nRoomID)!=m_arrNeedUptateRoomID.end())
		{
			return;
		}
		m_arrNeedUptateRoomID.push_back(nRoomID);
	}

	//取得更新的玩家，并删除
	void GetUpdateRoleID(RoleID arrRoleID[],int32_t nSize,int32_t& nUpdateCount )
	{
		nUpdateCount = 0;
		NeedUptateRoleIDVector::iterator itBegin = m_arrNeedUptateRoleID.begin();
		while(itBegin!= m_arrNeedUptateRoleID.end()&&nUpdateCount<nSize)
		{
			arrRoleID[nUpdateCount++] = *itBegin;
			itBegin=m_arrNeedUptateRoleID.erase(itBegin);
		}
	}

	//取得更新的房间，并删除
	void GetUpdateRoomID(RoomID arrRoomID[],int32_t nSize,int32_t& nUpdateCount )
	{
		nUpdateCount = 0;
		NeedUptateRoomIDVector::iterator itBegin = m_arrNeedUptateRoomID.begin();
		while(itBegin!= m_arrNeedUptateRoomID.end()&&nUpdateCount<nSize)
		{
			arrRoomID[nUpdateCount++] = *itBegin;
			itBegin=m_arrNeedUptateRoomID.erase(itBegin);
		}
	}

private:
	NeedUptateRoleIDVector  m_arrNeedUptateRoleID;
	NeedUptateRoomIDVector  m_arrNeedUptateRoomID;
};

#define	CREATE_UPDATEDATACTL_INSTANCE		CSingleton<CUpdateDataCtl>::CreateInstance
#define	GET_UPDATEDATACTL_INSTANCE			CSingleton<CUpdateDataCtl>::GetInstance
#define	DESTROY_UPDATEDATACTL_INSTANCE		CSingleton<CUpdateDataCtl>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END
#endif /* UPDATE_DATA_CTL_H_ */
