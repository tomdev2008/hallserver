/*
 * region_mgt.h
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#ifndef REGION_MGT_H_
#define REGION_MGT_H_

#include "dal/dal_region.h"
#include "def/server_namespace.h"
#include "def/server_errordef.h"
#include "public_typedef.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

class CRegionMgt : public CObject
{
public:
	CRegionMgt()
	{
		m_nCurUsedRegionCount = 0;
		for(int32_t i = 0; i < MaxRegionCount; ++i)
		{
			m_arrIsUsed[i] = false;
		}
	}

	~CRegionMgt()
	{

	}

	virtual int32_t Initialize()
	{
		for(int32_t i = 0; i < MaxRegionCount;++i)
		{
			m_arrRegion[i].Initialize();
		}

		return S_OK;
	}

	virtual int32_t Resume()
	{
		for(int32_t i = 0; i < MaxRegionCount;++i)
		{
			m_arrRegion[i].Resume();
		}

		return S_OK;
	}

	virtual int32_t Uninitialize()
	{
		for(int32_t i = 0; i < MaxRegionCount;++i)
		{
			m_arrRegion[i].Uninitialize();
		}

		return S_OK;
	}

	CRegion *GetRegion(const RegionID nRegionID)
	{
		for(int32_t i = 0; i < MaxRegionCount; ++i)
		{
			if(m_arrIsUsed[i] && (m_arrRegion[i].GetRegionID() == nRegionID))
			{
				return &m_arrRegion[i];
			}
		}

		return NULL;
	}

	void GetAllRegion(CRegion *arrRegion[], const int32_t arrSize, int32_t &nRegionCount)
	{
		nRegionCount = 0;
		for(int32_t i = 0; i < MaxRegionCount; ++i)
		{
			if(m_arrIsUsed[i])
			{
				arrRegion[nRegionCount++] = &m_arrRegion[i];
			}
		}
	}

	int32_t AddRegion(const RegionID nRegionID, const RegionType nRegionType)
	{
		if(m_nCurUsedRegionCount >= MaxRegionCount)
		{
			//todo : write something
			return E_HS_ADDREGIONISFULL;
		}

		int32_t index = enmInvalid16BitsIndex;
		for(int32_t i = 0; i < MaxRegionCount; ++i)
		{
			if(m_arrIsUsed[i])
			{
				if(m_arrRegion[i].GetRegionID() == nRegionID)
				{
					//todo : write something
					return E_HS_ADDREGIONISEXIST;
				}
			}
			else
			{
				if(enmInvalid16BitsIndex == index)
				{
					index = i;
				}
			}
		}

		if(index == enmInvalid16BitsIndex)
		{
			return E_HS_ADDREGIONISFULL;
		}

		m_arrRegion[index].SetRegionID(nRegionID);
		m_arrRegion[index].SetRegionType(nRegionType);

		return S_OK;
	}

	int32_t DeleteRegion(const RegionID nRegionID)
	{
		for(int32_t i = 0; i < MaxRegionCount; ++i)
		{
			if(m_arrIsUsed[i])
			{
				if(m_arrRegion[i].GetRegionID() == nRegionID)
				{
					m_arrRegion[i].Uninitialize();
					m_arrIsUsed[i] = false;
					--m_nCurUsedRegionCount;
				}
			}
		}

		return S_OK;
	}

	int32_t GetRegionCount() const
	{
		return m_nCurUsedRegionCount;
	}

	int32_t GetRegionUserCount(const RegionID nRegionID);

private:
	int32_t				m_nCurUsedRegionCount;
	CRegion					m_arrRegion[MaxRegionCount];				//大区对象
	bool					m_arrIsUsed[MaxRegionCount];
};


FRAME_HALLSERVER_NAMESPACE_END

#endif /* REGION_MGT_H_ */
