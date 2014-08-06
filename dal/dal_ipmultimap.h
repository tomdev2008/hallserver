/*
 * dal_ipmacmultimap.h
 *
 *  Created on: 2012-7-4
 *      Author: zhurw
 */

#ifndef DAL_IPMACMULTIMAP_H_
#define DAL_IPMACMULTIMAP_H_

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

typedef multimap<uint32_t, RoleID> IpMapType;

class CIpRoleIDMap
{
public:
	CIpRoleIDMap(){}
	~CIpRoleIDMap(){}
//���IP��RoleID��¼
	int32_t AddRoleIDItem(uint32_t nIpAddr, RoleID nRoleID)
	{
		if(nIpAddr != 0)
		{
			m_mIpMap.insert(make_pair(nIpAddr, nRoleID));
			return S_OK;
		}
		return E_UNKNOWN;
	}
//ɾ��ĳIP��Ӧ�����м�¼
	int32_t DelRoleIDItem(uint32_t nIpAddr)
	{
		m_mIpMap.erase(nIpAddr);
		return S_OK;
	}
//ɾ��ĳIP��Ӧ��RoleID�ļ�¼
	int32_t DelRoleIDItem(uint32_t nIpAddr, RoleID nRoleID)
	{
		pair<IpMapType::iterator, IpMapType::iterator> pai;
		pai = m_mIpMap.equal_range(nIpAddr);
		IpMapType::iterator it;
		for(it = pai.first; it != pai.second; ++it)
		{
			if(it->second == nRoleID)
			{
				m_mIpMap.erase(it);
				break;
			}
		}
		return S_OK;
	}
//��ȡĳIP��Ӧ��RoleID����Ҫһ���㹻����������RoleID
	int32_t GetIPRoleID(uint32_t nIpAddr, RoleID arrRoleID[], int32_t &nCount)
	{
		RoleID* pRoleID = arrRoleID;
		nCount = 0;
		pair<IpMapType::iterator, IpMapType::iterator> pai;
		pai = m_mIpMap.equal_range(nIpAddr);
		IpMapType::iterator it;
		for(it = pai.first; it != pai.second; ++it)
		{
			pRoleID[nCount] = it->second;
			nCount++;
			if(nCount >= MAXROLEIDPERIP)
			{
				break;
			}
		}
		return S_OK;
	}
private:
	IpMapType m_mIpMap;
};

FRAME_HALLSERVER_NAMESPACE_END

#endif /* DAL_IPMACMULTIMAP_H_ */
