/*
 * ipmacmap_mgt.cpp
 *
 *  Created on: 2012-7-4
 *      Author: zhurw
 */

#include "ipmacmap_mgt.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

CIpMacMapMgt::CIpMacMapMgt()
{
}

CIpMacMapMgt::~CIpMacMapMgt()
{
}

int32_t CIpMacMapMgt::Initialize()
{
	return S_OK;
}

int32_t CIpMacMapMgt::Uninitialize()
{
	return S_OK;
}

int32_t CIpMacMapMgt::Resume()
{
	return S_OK;
}
int32_t CIpMacMapMgt::AddIpRoleIDItem(uint32_t nIpAddr, RoleID nRoleID)
{
	return m_sIpRoleIDMap.AddRoleIDItem(nIpAddr, nRoleID);
}
int32_t CIpMacMapMgt::DelIpRoleIDItem(uint32_t nIpAddr)
{
	return m_sIpRoleIDMap.DelRoleIDItem(nIpAddr);
}
int32_t CIpMacMapMgt::DelIpRoleIDItem(uint32_t nIpAddr, RoleID nRoleID)
{
	return m_sIpRoleIDMap.DelRoleIDItem(nIpAddr, nRoleID);
}
int32_t CIpMacMapMgt::GetIPRoleID(uint32_t nIpAddr, RoleID arrRoleID[], int32_t &nCount)
{
	RoleID* pRoleID = arrRoleID;
	return m_sIpRoleIDMap.GetIPRoleID(nIpAddr, pRoleID, nCount);
}
int32_t CIpMacMapMgt::AddMacRoleIDItem(uint64_t nMacAddr, RoleID nRoleID)
{
	return m_sMacRoleIDMap.AddRoleIDItem(nMacAddr, nRoleID);
}
int32_t CIpMacMapMgt::DelMacRoleIDItem(uint64_t nMacAddr)
{
	return m_sMacRoleIDMap.DelRoleIDItem(nMacAddr);
}
int32_t CIpMacMapMgt::DelMacRoleIDItem(uint64_t nMacAddr, RoleID nRoleID)
{
	return m_sMacRoleIDMap.DelRoleIDItem(nMacAddr, nRoleID);
}
int32_t CIpMacMapMgt::GetMacRoleID(uint64_t nMacAddr, RoleID arrRoleID[], int32_t &nCount)
{
	RoleID* pRoleID = arrRoleID;
	return m_sMacRoleIDMap.GetMacRoleID(nMacAddr, pRoleID, nCount);
}

uint64_t CIpMacMapMgt::MacCStringToLong(const char* strMacAddr)
{
	uint64_t n = 0;
	uint64_t result = 0;
	int32_t strLen = (int32_t)strlen(strMacAddr);
	for(int32_t i; i < strLen; i++)
	{
		if(strMacAddr[i] >= 'A' && strMacAddr[i] <= 'F')
		{
			n = strMacAddr[i] - 'A' + 10;
		}
		else if(strMacAddr[i] >= 'a' && strMacAddr[i] <= 'f')
		{
			n = strMacAddr[i] - 'a' + 10;
		}
		else
		{
			n = strMacAddr[i] - '0';
		}
		result = result * 16 + n;

	}

	return result;
}

FRAME_HALLSERVER_NAMESPACE_END
