/*
 * ipmacmap_mgt.h
 *
 *  Created on: 2012-7-4
 *      Author: zhurw
 */

#ifndef IPMACMAP_MGT_H_
#define IPMACMAP_MGT_H_

//#include "common/common_pool.h"
//#include "common/common_hashlist.h"
#include "dal/dal_ipmultimap.h"
#include "def/server_namespace.h"
#include "dal/dal_macmultimap.h"
#include "common/common_object.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

class CIpMacMapMgt : public CObject
{
public:
	CIpMacMapMgt();
	virtual ~CIpMacMapMgt();

	//��ʼ����ҹ�����
	virtual int32_t Initialize();
	//�ָ���ҹ�����
	virtual int32_t Resume();
	//ע����ҹ�����
	virtual int32_t Uninitialize();
	int32_t AddIpRoleIDItem(uint32_t nIpAddr, RoleID nRoleID);
	int32_t DelIpRoleIDItem(uint32_t nIpAddr);
	int32_t DelIpRoleIDItem(uint32_t nIpAddr, RoleID nRoleID);
	int32_t GetIPRoleID(uint32_t nIpAddr, RoleID arrRoleID[], int32_t &arrSize);

	int32_t AddMacRoleIDItem(uint64_t nIpAddr, RoleID nRoleID);
	int32_t DelMacRoleIDItem(uint64_t nIpAddr);
	int32_t DelMacRoleIDItem(uint64_t nIpAddr, RoleID nRoleID);
	int32_t GetMacRoleID(uint64_t nIpAddr, RoleID arrRoleID[], int32_t &arrSize);

	uint64_t MacCStringToLong(const char* strMacAddr);//ת����BC3012F2D8����ʽΪ������

private:
//	void    GetDecimal(char hex, char* dec);

private:
	CIpRoleIDMap m_sIpRoleIDMap;
	CMacRoleIDMap m_sMacRoleIDMap;
};

#define CREATE_IPMACMAPMGT_INSTANCE		CSingleton<CIpMacMapMgt>::CreateInstance
#define GET_IPMACMAPMGT_INSTANCE		CSingleton<CIpMacMapMgt>::GetInstance
#define DESTROY_IPMACMAPMGT_INSTANCE	CSingleton<CIpMacMapMgt>::DestroyInstance

#define g_IpMacMapMgt	GET_IPMACMAPMGT_INSTANCE()

FRAME_HALLSERVER_NAMESPACE_END

#endif /* IPMACMAP_MGT_H_ */
