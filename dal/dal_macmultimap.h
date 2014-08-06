/*
 * dal_macmultimap.h
 *
 *  Created on: 2012-7-4
 *      Author: zhurw
 */

#ifndef DAL_MACMULTIMAP_H_
#define DAL_MACMULTIMAP_H_

#include <map>
//#include "common/common_complextype.h"
//#include "common/common_object.h"
//#include "common/common_string.h"
#include "frame_typedef.h"
//#include "frame/def/frame_protocolhead.h"
#include "def/server_namespace.h"
#include "public_typedef.h"
#include "def/server_errordef.h"
#include "common/common_errordef.h"
#include "def/dal_def.h"

using namespace std;

FRAME_HALLSERVER_NAMESPACE_BEGIN

typedef multimap<uint64_t, RoleID> MacMapType;

class CMacRoleIDMap
{
public:
	CMacRoleIDMap(){}
	~CMacRoleIDMap(){}
//添加Mac、RoleID记录
	int32_t AddRoleIDItem(uint64_t nMacAddr, RoleID nRoleID)
	{
		if(nMacAddr != 0)
		{
			m_mMacMap.insert(make_pair(nMacAddr, nRoleID));
			return S_OK;
		}
		return E_UNKNOWN;
	}
//删除某Mac对应的所有记录
	int32_t DelRoleIDItem(uint64_t nMacAddr)
	{
		m_mMacMap.erase(nMacAddr);
		return S_OK;
	}
//删除某Mac对应的RoleID的记录
	int32_t DelRoleIDItem(uint64_t nMacAddr, RoleID nRoleID)
	{
		pair<MacMapType::iterator, MacMapType::iterator> pai;
		pai = m_mMacMap.equal_range(nMacAddr);
		MacMapType::iterator it;
		for(it = pai.first; it != pai.second; ++it)
		{
			if(it->second == nRoleID)
			{
				m_mMacMap.erase(it);
				break;
			}
		}
		return S_OK;
	}
//获取某Mac对应的RoleID，需要一个足够大的数组接收RoleID
	int32_t GetMacRoleID(uint64_t nMacAddr, RoleID arrRoleID[], int32_t &nCount)
	{
		RoleID* pRoleID = arrRoleID;
		nCount = 0;
		pair<MacMapType::iterator, MacMapType::iterator> pai;
		pai = m_mMacMap.equal_range(nMacAddr);
		MacMapType::iterator it;
		for(it = pai.first; it != pai.second; ++it)
		{
			pRoleID[nCount] = it->second;
			nCount++;
			if(nCount >= MAXROLEIDPERMAC)
			{
				break;
			}
		}
		return S_OK;
	}
private:
	MacMapType m_mMacMap;
};

FRAME_HALLSERVER_NAMESPACE_END

#endif /* DAL_MACMULTIMAP_H_ */
