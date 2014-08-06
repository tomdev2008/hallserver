/*
 * dal_region.cpp
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#include "dal_region.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

bool CRegion::IsRoomInRegion(const RoomID nRoomID)
{
	for(int32_t i = 0; i < m_nCurChannelCount; ++i)
	{
		int32_t index = m_arrUsedChannelIndex[i];
		if(m_arrChannelObject[index].IsRoomInChannel(nRoomID))
		{
			return true;
		}
	}

	return false;
}

int32_t CRegion::AddChannel(const ChannelID nChannelID, const ChannelType nChannelType)
{
	if(m_nCurChannelCount >= MaxChannelCountPerRegion || m_nFreeChannelCount == 0)
	{
		return E_HS_CHANNELISFULL;
	}

	//取出一个空闲的
	int32_t index = m_arrFreeChannelIndex[m_nFreeChannelCount];
	m_arrFreeChannelIndex[m_nFreeChannelCount] = enmInvalid16BitsIndex;
	--m_nFreeChannelCount;

	m_arrUsedChannelIndex[m_nCurChannelCount] = index;
	++m_nCurChannelCount;

	m_arrChannelObject[index].SetChannelID(nChannelID);
	m_arrChannelObject[index].SetChannelType(nChannelType);
	m_arrChannelObject[index].SetRegionID(m_nRegionID);
	m_arrChannelObject[index].SetRegionType(m_nRegionType);

	return S_OK;
}

int32_t CRegion::DeleteChannel(const ChannelID nChannelID)
{
	if(m_nCurChannelCount == 0)
	{
		//todo : write something
		return S_OK;
	}

	int32_t index = 0;
	bool bIsFind = false;
	for(int32_t i = 0; i < m_nCurChannelCount; ++i)
	{
		index = m_arrUsedChannelIndex[i];
		if(m_arrChannelObject[index].GetChannelID() == nChannelID)
		{
			m_arrChannelObject[index].Uninitialize();

			m_arrFreeChannelIndex[m_nFreeChannelCount++] = index;

			//最后一个位置
			if(i + 1 == m_nCurChannelCount)
			{
				m_arrUsedChannelIndex[i] = enmInvalid16BitsIndex;
				--m_nCurChannelCount;
			}
			else
			{
				//把最后一个调过来
				if(enmInvalid16BitsIndex != m_arrUsedChannelIndex[m_nCurChannelCount - 1])
				{
					m_arrUsedChannelIndex[i] = m_arrUsedChannelIndex[m_nCurChannelCount - 1];
					--m_nCurChannelCount;
				}
			}

			bIsFind = true;
			break;
		}
	}

	if(!bIsFind)
	{
		//todo : write something
	}

	return S_OK;
}

int32_t CRegion::IsChannelInRegion(const ChannelID nChannelID)
{
	for(int32_t i = 0; i < m_nCurChannelCount; ++i)
	{
		int32_t index = m_arrUsedChannelIndex[i];
		if(nChannelID == m_arrChannelObject[index].GetChannelID())
		{
			return true;
		}
	}

	return false;
}

CChannel *CRegion::GetChannelObject(const ChannelID nChannelID)
{
	for(int32_t i = 0; i < m_nCurChannelCount; ++i)
	{
		int32_t index = m_arrUsedChannelIndex[i];
		if(nChannelID == m_arrChannelObject[index].GetChannelID())
		{
			return &m_arrChannelObject[index];
		}
	}

	return NULL;
}

int32_t CRegion::GetAllChannelObject(CChannel *arrChannelObject[], const int32_t arrSize, int32_t &nChannelObjectCount)
{
	nChannelObjectCount = 0;
	for(int32_t i = 0; i < m_nCurChannelCount; ++i)
	{
		int32_t index = m_arrUsedChannelIndex[i];
		if(nChannelObjectCount < arrSize)
		{
			arrChannelObject[nChannelObjectCount] = &m_arrChannelObject[index];
			++nChannelObjectCount;
		}
		else
		{
			return S_OK;
		}
	}

	return S_OK;
}


FRAME_HALLSERVER_NAMESPACE_END

