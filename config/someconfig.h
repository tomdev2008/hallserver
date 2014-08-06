/*
 * someconfig.h
 *
 *  Created on: 2011-12-3
 *      Author: jimm
 */

#ifndef SOMECONFIG_H_
#define SOMECONFIG_H_

//#include <memory.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
#include "tinyxml/tinyxml.h"
#include "common/common_singleton.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_config_container.h"
#include "main_frame.h"
#include "frame_logengine.h"
#include "public_typedef.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

#define DEFAULT_ABOUTROOM_CONFIG		"./config/aboutroomconfig.xml"
#define MAX_ROOM_TYPE_COUNT			8
#define MAX_VIP_TYPE_COUNT          20
#define MAX_IDENTITY_TYPE_COUNT     20

#define DEFAULT_ABOUTCLIENT_CONFIG "./config/aboutclient.xml"

class CRoomTypeInfo
{
public:
	uint16_t 		nMaxPlayercount;  //房间最大进入人数
	uint16_t 		nMaxHoatCount; //室主的最大数量
	uint16_t		nMaxSecHostCount;//副室主的最大数量
	uint16_t		nMaxAdmCount;//管理员的最大数量
	uint16_t		nMaxTempAdmCount;//临时管理员的最大数量

	CRoomTypeInfo()
	{
		nMaxPlayercount = 0;
		nMaxHoatCount = 0;
		nMaxSecHostCount = 0;
		nMaxAdmCount = 0;
		nMaxTempAdmCount = 0;
	}
};
class CVipTypeInfo
{
public:
	uint8_t           nVipType;                  //vip类型
	uint16_t           nMaxAdmCount;              //管理员数量
	CVipTypeInfo()
	{
		nVipType = 0;
		nMaxAdmCount = 0;
	}
};
class CIdentityTypeInfo
{
public:
	IdentityType          nIdentityType;             //身份类型
	uint16_t          nMaxAdmCount;              //管理员数量
	CIdentityTypeInfo()
	{
		nIdentityType = enmIdentityType_None;
		nMaxAdmCount = 0;
	}
};
class CAboutRoomConfig :public IConfigCenter
{
public:
	CAboutRoomConfig()
	{
		m_nRoomTypeCount=0;
		m_nVipTypeCount = 0;
		m_nIdentityTypeCount = 0;
//		memset(m_arrRoomTypeInfo,0,sizeof(CRoomTypeInfo)*MAX_ROOM_TYPE_COUNT);
	}
	virtual ~CAboutRoomConfig()
	{

	}

public:
	int32_t Initialize(const char* szFileName = DEFAULT_ABOUTROOM_CONFIG, const int32_t nType = 0);
	int32_t Reload(const char* szFileName = DEFAULT_ABOUTROOM_CONFIG, const int32_t type = 0);
	int32_t Resume()
	{
		m_nRoomTypeCount = 0;
		memset(m_arrRoomTypeInfo,0,sizeof(CRoomTypeInfo)*MAX_ROOM_TYPE_COUNT);
		m_nVipTypeCount = 0;
		memset(m_arrVipTypeInfo,0,sizeof(CVipTypeInfo)*MAX_VIP_TYPE_COUNT);
		m_nIdentityTypeCount = 0;
		memset(m_arrIdentityTypeCount, 0, sizeof(CIdentityTypeInfo)*MAX_IDENTITY_TYPE_COUNT);
		return S_OK;
	}
	int32_t Uninitialize()
	{
		Resume();
		return S_OK;
	}

	CRoomTypeInfo GetRoomTypeInfo(uint16_t nMaxPlayerCount)
	{
		CRoomTypeInfo retCRoomTypeInfo;
		for(int32_t i = 0; i<m_nRoomTypeCount;i++)
		{
			if(nMaxPlayerCount==m_arrRoomTypeInfo[i].nMaxPlayercount)
			{
				return m_arrRoomTypeInfo[i];
			}
		}
		return retCRoomTypeInfo;
	}
	uint16_t  GetVipAdminCount(uint8_t nVipType)
	{
		for(int16_t i = 0; i < m_nVipTypeCount; ++i)
		{
			if (m_arrVipTypeInfo[i].nVipType == nVipType)
			{
				return m_arrVipTypeInfo[i].nMaxAdmCount;
			}
		}
		return 0;
	}
	uint16_t GetIdentityAdminCount(IdentityType nIdentityType)
	{
		for(uint16_t i = 0; i < m_nIdentityTypeCount; ++i)
		{
			if(m_arrIdentityTypeCount[i].nIdentityType & nIdentityType)
			{
				return m_arrIdentityTypeCount[i].nMaxAdmCount;
			}
		}
		return 0;
	}
private:
	int32_t LoadRoomTypeInfo(TiXmlElement* pRoot);

private:
	uint16_t		m_nRoomTypeCount;//现在房间的类型数量
	CRoomTypeInfo   m_arrRoomTypeInfo[MAX_ROOM_TYPE_COUNT]; ////保存从配置中读出的有关房间类型的信息
	uint16_t          m_nVipTypeCount;                        //当前VIP类型数量
	CVipTypeInfo      m_arrVipTypeInfo[MAX_VIP_TYPE_COUNT];   //保存配置读取的vip对应的管理数量信息
	uint16_t          m_nIdentityTypeCount;                     //身份类型数目
	CIdentityTypeInfo m_arrIdentityTypeCount[MAX_IDENTITY_TYPE_COUNT]; //保存身份类型对应的管理员数量
};

class CAboutClientConfig : public IConfigCenter
{
public:
	CAboutClientConfig()
	{
		m_nMinAllowedVer = 0;
		m_nMaxAllowedVer = 0;
	}
	virtual ~CAboutClientConfig()
	{
	}
public:
	int32_t Initialize(const char* szFileName = DEFAULT_ABOUTCLIENT_CONFIG, const int32_t nType = 0);
	int32_t Reload(const char* szFileName = DEFAULT_ABOUTCLIENT_CONFIG, const int32_t nType = 0);
	int32_t Resume()
	{
		m_nMinAllowedVer = 0;
		m_nMaxAllowedVer = 0;
		return S_OK;
	}
	int32_t Uninitialize()
	{
		m_nMinAllowedVer = 0;
		m_nMaxAllowedVer = 0;
		return S_OK;
	}
	bool IsAllowedVersion(int32_t nVersion)
	{
		if(nVersion >= m_nMinAllowedVer && nVersion <= m_nMaxAllowedVer)
		{
			return true;
		}
		return false;
	}
private:
	int32_t LoadClientConfigInfo(TiXmlElement* pRoot);
private:
	int32_t m_nMinAllowedVer;
	int32_t m_nMaxAllowedVer;
};

typedef CFrameConfigContainer<CAboutRoomConfig>	  CAboutRoomConfigContainer;
typedef CFrameConfigContainer<CAboutClientConfig> CAboutClientConfigContainer;

#define INITIALIZE_ABOUTROOMCONFIG		CSingleton<CAboutRoomConfigContainer>::GetInstance().Initialize
#define GET_ABOUTROOMCONFIG				CSingleton<CAboutRoomConfigContainer>::GetInstance().GetConfigObject

#define INITIALIZE_ABOUTCLIENTCONFIG	CSingleton<CAboutClientConfigContainer>::GetInstance().Initialize
#define GET_ABOUTCLIENTCONFIG			CSingleton<CAboutClientConfigContainer>::GetInstance().GetConfigObject

#define g_AboutRoomConfig 	GET_ABOUTROOMCONFIG()
#define g_AboutClientConfig GET_ABOUTCLIENTCONFIG()

FRAME_HALLSERVER_NAMESPACE_END

#endif /* SOMECONFIG_H_ */
