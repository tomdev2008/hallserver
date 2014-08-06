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
	uint16_t 		nMaxPlayercount;  //��������������
	uint16_t 		nMaxHoatCount; //�������������
	uint16_t		nMaxSecHostCount;//���������������
	uint16_t		nMaxAdmCount;//����Ա���������
	uint16_t		nMaxTempAdmCount;//��ʱ����Ա���������

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
	uint8_t           nVipType;                  //vip����
	uint16_t           nMaxAdmCount;              //����Ա����
	CVipTypeInfo()
	{
		nVipType = 0;
		nMaxAdmCount = 0;
	}
};
class CIdentityTypeInfo
{
public:
	IdentityType          nIdentityType;             //�������
	uint16_t          nMaxAdmCount;              //����Ա����
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
	uint16_t		m_nRoomTypeCount;//���ڷ������������
	CRoomTypeInfo   m_arrRoomTypeInfo[MAX_ROOM_TYPE_COUNT]; ////����������ж������йط������͵���Ϣ
	uint16_t          m_nVipTypeCount;                        //��ǰVIP��������
	CVipTypeInfo      m_arrVipTypeInfo[MAX_VIP_TYPE_COUNT];   //�������ö�ȡ��vip��Ӧ�Ĺ���������Ϣ
	uint16_t          m_nIdentityTypeCount;                     //���������Ŀ
	CIdentityTypeInfo m_arrIdentityTypeCount[MAX_IDENTITY_TYPE_COUNT]; //����������Ͷ�Ӧ�Ĺ���Ա����
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
