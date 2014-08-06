/*
 * dal_region.h
 *
 *  Created on: 2011-12-1
 *      Author: jimm
 */

#ifndef DAL_REGION_H_
#define DAL_REGION_H_

#include "common/common_complextype.h"
#include "common/common_object.h"
#include "def/server_namespace.h"
#include "def/server_errordef.h"
#include "dal_channel.h"

#include "public_typedef.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN


class CRegion : public CObject
{
public:
	CRegion()
	{
		m_nRegionID = enmInvalidRegionID;
		m_nRegionType = enmInvalidRegionType;
	}

	~CRegion()
	{

	}

	virtual int32_t Initialize()
	{
		m_nRegionID = enmInvalidRegionID;
		m_nRegionType = enmInvalidRegionType;

		m_nCurChannelCount = 0;
		for(int32_t i = 0; i < MaxChannelCountPerRegion; ++i)
		{
			m_arrChannelObject[i].Initialize();
			m_arrUsedChannelIndex[i] = enmInvalid16BitsIndex;
		}

		m_nFreeChannelCount = MaxChannelCountPerRegion;
		for(int32_t i = 0; i < MaxChannelCountPerRegion; ++i)
		{
			m_arrFreeChannelIndex[i] = MaxChannelCountPerRegion - i - 1;
		}

		return S_OK;
	}

	virtual int32_t Resume()
	{
		Initialize();
		return S_OK;
	}

	virtual int32_t Uninitialize()
	{
		Initialize();
		return S_OK;
	}

	void SetRegionType(const RegionType nRegionType)
	{
		m_nRegionType = nRegionType;
	}

	RegionType GetRegionType() const
	{
		return m_nRegionType;
	}

	void SetRegionID(const RegionID nRegionID)
	{
		m_nRegionID = nRegionID;
	}

	RegionID GetRegionID() const
	{
		return m_nRegionID;
	}

	bool IsRoomInRegion(const RoomID nRoomID);

	int32_t AddChannel(const ChannelID nChannelID, const ChannelType nChannelType);

	int32_t DeleteChannel(const ChannelID nChannelID);

	int32_t IsChannelInRegion(const ChannelID nChannelID);

	CChannel *GetChannelObject(const ChannelID nChannelID);

	int32_t GetAllChannelObject(CChannel *arrChannelObject[], const int32_t arrSize, int32_t &nChannelObjectCount);

private:
	RegionType							m_nRegionType;
	RegionID							m_nRegionID;

	int32_t								m_nCurChannelCount;
	int32_t								m_arrUsedChannelIndex[MaxChannelCountPerRegion];

	int32_t								m_nFreeChannelCount;
	int32_t								m_arrFreeChannelIndex[MaxChannelCountPerRegion];

	CChannel							m_arrChannelObject[MaxChannelCountPerRegion];
};


FRAME_HALLSERVER_NAMESPACE_END

CCOMPLEXTYPE_SPECIFICALLY(FRAME_HALLSERVER_NAMESPACE::CRegion)

#endif /* DAL_REGION_H_ */
