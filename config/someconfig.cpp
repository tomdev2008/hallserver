/*
 * someconfig.cpp
 *
 *  Created on: 2011-12-3
 *      Author: jimm
 */
#include "common/common_singleton.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "someconfig.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN


#define LEVEL  "level"

#define ROOMTYPES   "roomtypes"
#define ROOMTYPE   "roomtype"
#define PLAYERCOUNT  "playercount"
#define HOSTCOUNT "hostcount"
#define SECHOSTCOUNT "sechostcount"
#define ADMCOUNT "admcount"
#define TEMPADMCOUNT "tempadmcount"
#define VIPTYPE      "viptype"
#define VIPVALUE     "vipvalue"
#define IDENTITYTYPE "identitytype"
#define IDENTITYVALUE "identityvalue"

#define CLIENT			"client"
#define ALLOWEDVERSION	"allowedversion"
#define MINCLIENTVER		"minclientver"
#define MAXCLIENTVER		"maxclientver"

int32_t CAboutRoomConfig::Initialize(const char* szFileName, const int32_t nType)
{
	//加载配置文件

	int32_t ret = S_OK;
	TiXmlDocument doc(szFileName);
	if (!doc.LoadFile(TIXML_ENCODING_UTF8))
	{
		g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Load about room config file failed!\n");
		return E_UNKNOWN;
	}
	g_FrameLogEngine.WriteBaseLog(enmLogLevel_Notice,"Load about room config file success!\n");

	//获取根节点
	TiXmlElement *pRoot = doc.RootElement();
	if (NULL == pRoot)
	{
		g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Root node cannot be found in config file!\n");
		return E_UNKNOWN;
	}
	g_FrameLogEngine.WriteBaseLog(enmLogLevel_Notice,"Get about room config root node success!\n");

	ret = LoadRoomTypeInfo(pRoot);
	if (0 > ret)
	{
		return ret;
	}

	return ret;
}

int32_t CAboutRoomConfig::Reload(const char* szFileName, const int32_t type)
{
	return Initialize(szFileName);
}

int32_t CAboutRoomConfig::LoadRoomTypeInfo(TiXmlElement* pRoot)
{
	m_nRoomTypeCount = 0;
//	memset(m_arrRoomTypeInfo,0,sizeof(CRoomTypeInfo)*MAX_ROOM_TYPE_COUNT);
	const char* pszValue = NULL;

	TiXmlElement *pRoomTypes = pRoot->FirstChildElement(ROOMTYPES);
	TiXmlElement *pRoomType = pRoomTypes->FirstChildElement(ROOMTYPE);

	if(pRoomType == NULL)
	{
		g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get pRoomType failed!\n");
		return E_NULLPOINTER;
	}

	while(NULL != pRoomType)
	{
		int32_t nMaxPlayercount;
		pszValue = pRoomType->Attribute(PLAYERCOUNT, &nMaxPlayercount);
		if (NULL == pszValue)
		{
			g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get MaxPlayercount failed!\n");
			return E_UNKNOWN;
		}
		m_arrRoomTypeInfo[m_nRoomTypeCount].nMaxPlayercount = nMaxPlayercount;
		int32_t nMaxHoatCount;
		pszValue = pRoomType->Attribute(HOSTCOUNT, &nMaxHoatCount);
		if (NULL == pszValue)
		{
			g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get MaxHoatCount failed!\n");
			return E_UNKNOWN;
		}
		m_arrRoomTypeInfo[m_nRoomTypeCount].nMaxHoatCount = nMaxHoatCount;
		int32_t nMaxSecHostCount;
		pszValue = pRoomType->Attribute(SECHOSTCOUNT, &nMaxSecHostCount);
		if (NULL == pszValue)
		{
			g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get MaxSecHostCount failed!\n");
			return E_UNKNOWN;
		}
		m_arrRoomTypeInfo[m_nRoomTypeCount].nMaxSecHostCount = nMaxSecHostCount;
		int32_t nMaxAdmCount;
		pszValue = pRoomType->Attribute(ADMCOUNT, &nMaxAdmCount);
		if (NULL == pszValue)
		{
			g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get MaxAdmCount failed!\n");
			return E_UNKNOWN;
		}
		m_arrRoomTypeInfo[m_nRoomTypeCount].nMaxAdmCount = nMaxAdmCount;
		int32_t nMaxTempAdmCount;
		pszValue = pRoomType->Attribute(TEMPADMCOUNT, &nMaxTempAdmCount);
		if (NULL == pszValue)
		{
			g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get MaxTempAdmCount failed!\n");
			return E_UNKNOWN;
		}
		m_arrRoomTypeInfo[m_nRoomTypeCount].nMaxTempAdmCount = nMaxTempAdmCount;

		m_nRoomTypeCount++;
		pRoomType = pRoomType->NextSiblingElement();
	}
	m_nVipTypeCount = 0;
	TiXmlElement *pVipTypes = pRoomTypes->NextSiblingElement();
	TiXmlElement *pVipType = pVipTypes->FirstChildElement(VIPTYPE);
	if(pVipType == NULL)
	{
		g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get pVipType failed!\n");
		return E_NULLPOINTER;
	}
	while(pVipType != NULL)
	{
		int32_t nVipValue = 0;
		pszValue=pVipType->Attribute(VIPVALUE);
		if (NULL == pszValue)
		{
				g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get viptype failed!\n");
				return E_UNKNOWN;
		}
		sscanf(pszValue,"%x",(int32_t *)&nVipValue);
		m_arrVipTypeInfo[m_nVipTypeCount].nVipType = nVipValue;
		int32_t nAdmCount = 0;
		pszValue=pVipType->Attribute(ADMCOUNT);
		if (NULL == pszValue)
		{
				g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get viptype admcount failed!\n");
				return E_UNKNOWN;
		}
		sscanf(pszValue,"%d",(int32_t *)&nAdmCount);
		m_arrVipTypeInfo[m_nVipTypeCount].nMaxAdmCount = nAdmCount;
		m_nVipTypeCount++;
		pVipType = pVipType->NextSiblingElement();
	}
	m_nIdentityTypeCount = 0;
	TiXmlElement *pIdentityTypes = pVipTypes->NextSiblingElement();
	TiXmlElement *pIdentityType = pIdentityTypes->FirstChildElement(IDENTITYTYPE);
	if(pIdentityType == NULL)
	{
		g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get pIdentityType failed!\n");
		return E_NULLPOINTER;
	}
	while(pIdentityType != NULL)
	{
		int32_t nIdentityValue = 0;
		pszValue=pIdentityType->Attribute(IDENTITYVALUE);
		if (NULL == pszValue)
		{
				g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get viptype failed!\n");
				return E_UNKNOWN;
		}
		sscanf(pszValue,"%x",(int32_t *)&nIdentityValue);
		m_arrIdentityTypeCount[m_nIdentityTypeCount].nIdentityType = nIdentityValue;
		int32_t nAdmCount = 0;
		pszValue=pIdentityType->Attribute(ADMCOUNT);
		if (NULL == pszValue)
		{
				g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get viptype admcount failed!\n");
				return E_UNKNOWN;
		}
		sscanf(pszValue,"%d",(int32_t *)&nAdmCount);
		m_arrIdentityTypeCount[m_nIdentityTypeCount].nMaxAdmCount = nAdmCount;
		m_nIdentityTypeCount++;
		pIdentityType = pIdentityType->NextSiblingElement();
	}
	return S_OK;
}

int32_t CAboutClientConfig::Initialize(const char* szFileName, int nType)
{
	//加载配置文件

	int32_t ret = S_OK;
	TiXmlDocument doc(szFileName);
	if (!doc.LoadFile(TIXML_ENCODING_UTF8))
	{
		g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Load about client config file failed!\n");
		return E_UNKNOWN;
	}
	g_FrameLogEngine.WriteBaseLog(enmLogLevel_Notice,"Load about client config file success!\n");

	//获取根节点
	TiXmlElement *pRoot = doc.RootElement();
	if (NULL == pRoot)
	{
		g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Root node cannot be found in config file!\n");
		return E_UNKNOWN;
	}
	g_FrameLogEngine.WriteBaseLog(enmLogLevel_Notice,"Get about client config root node success!\n");

	ret = LoadClientConfigInfo(pRoot);
	if (0 > ret)
	{
		return ret;
	}

	return ret;
}

int32_t CAboutClientConfig::Reload(const char* szFileName, int nType)
{
	return Initialize(szFileName, nType);
}

int32_t CAboutClientConfig::LoadClientConfigInfo(TiXmlElement* pRoot)
{
	TiXmlElement* pClient = pRoot->FirstChildElement(CLIENT);
	TiXmlElement* pAllowedVer = pClient->FirstChildElement(ALLOWEDVERSION);
	if(pAllowedVer == NULL)
	{
		g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get pAllowedver failed!\n");
		return E_NULLPOINTER;
	}
	const char* pszValue = NULL;

	while(pAllowedVer != NULL)
	{
		pszValue = pAllowedVer->Attribute(MINCLIENTVER, &m_nMinAllowedVer);
		if(pszValue == NULL)
		{
			g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get m_nMinAllowedVer failed!\n");
			return E_UNKNOWN;
		}
		pszValue = pAllowedVer->Attribute(MAXCLIENTVER, &m_nMaxAllowedVer);
		if(pszValue == NULL)
		{
			g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error,"Get m_nMaxAllowedVer failed!\n");
			return E_UNKNOWN;
		}
		pAllowedVer = pAllowedVer->NextSiblingElement();
	}
	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
