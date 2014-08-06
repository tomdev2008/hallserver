/*
 * dal_room.cpp
 *
 *  Created on: 2011-12-1
 *      Author: jimm
 */

#include "dal_room.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CRoom::AddPlayer(const RoleID nRoleID)
{
	RoomPlayerList::Key key = MakePlayerKey(nRoleID);
	RoomPlayerList::CIndex* pIndex = m_sRoomPlayerList.Insert(key, nRoleID);
	if (NULL != pIndex)
	{
		++m_nCurUserCount;
	}

	return S_OK;
}

int32_t CRoom::DeletePlayer(const RoleID nRoleID)
{
	RoomPlayerList::Key key = MakePlayerKey(nRoleID);
	int32_t ret = m_sRoomPlayerList.Erase(key);
	if (0 <= ret)
	{
		--m_nCurUserCount;
	}

	return S_OK;
}

bool CRoom::IsPlayerInRoom(const RoleID nRoleID)
{
	RoomPlayerList::Key key = MakePlayerKey(nRoleID);
	RoomPlayerList::CIndex *pIndex = m_sRoomPlayerList.Find(key);

	return NULL != pIndex;
}


int32_t CRoom::GetAllPlayers(RoleID arrRoleID[], const int32_t arrSize, int32_t &nPlayerCount)
{
	nPlayerCount = 0;
	RoomPlayerList::CIndex *pIndex = m_sRoomPlayerList.First();

	while(NULL != pIndex && nPlayerCount < arrSize)
	{
		arrRoleID[nPlayerCount++] = pIndex->Object();
		pIndex = pIndex->Next();
	}

	return S_OK;
}

int32_t CRoom::DeleteAllPlayers()
{
	RoleID arrRoleID[MaxUserCountPerRoom];
	int32_t nPlayerCount = 0;

	GetAllPlayers(arrRoleID, MaxUserCountPerRoom, nPlayerCount);

	for(int32_t i = 0; i < nPlayerCount; ++i)
	{
		DeletePlayer(arrRoleID[i]);
	}

	return S_OK;
}

RoomPlayerList::Key CRoom::MakePlayerKey(const RoleID nRoleID) const
{
	RoomPlayerList::Key key = { 0 };
	key.nKeyLength = (uint32_t)sizeof(RoleID);
	*(RoleID*)key.arrKey = nRoleID;

	return key;
}

void CRoom::UpdateDataInfo(uint32_t nOffset, uint32_t nDataSize)
{
	for(int32_t i = 0; i < m_nUpdateDataInfoCount; ++i)
	{
		if(m_arrUpdateDataInfoTable[i].nOffset == nOffset)
		{
			m_arrUpdateDataInfoTable[i].nDataSize = nDataSize;
			return;
		}
	}

	if(m_nUpdateDataInfoCount < UpdateDataInfoTableSize)
	{
		m_arrUpdateDataInfoTable[m_nUpdateDataInfoCount].nOffset = nOffset;
		m_arrUpdateDataInfoTable[m_nUpdateDataInfoCount].nDataSize = nDataSize;

		++m_nUpdateDataInfoCount;
	}
	GET_UPDATEDATACTL_INSTANCE().AddRoomNeedUpdate(m_nRoomID);
}

int32_t CRoom::GetUpdateDataInfoTable(uint8_t* arrUpdateData[],
		UpdateDataInfoTable arrUpdateTable[])
{
	//排序
		for(int32_t i = 1;i<m_nUpdateDataInfoCount;i++)
		{
			UpdateDataInfoTable dataTemp = m_arrUpdateDataInfoTable[i];
			int32_t j =i;
			while(j>0&&m_arrUpdateDataInfoTable[j-1].nOffset>dataTemp.nOffset)
			{
				m_arrUpdateDataInfoTable[j] = m_arrUpdateDataInfoTable[j-1];
				--j;
			}
			m_arrUpdateDataInfoTable[j] = dataTemp;
		}

		//合并可以合并的
		uint32_t nOffsetTemp = m_arrUpdateDataInfoTable[0].nOffset;
		uint32_t nDatasizeTemp = m_arrUpdateDataInfoTable[0].nDataSize;
		int32_t nIndex = 0;

		uint8_t* pStartPos = &m_nStartPos;

		for(int32_t i = 1;i< m_nUpdateDataInfoCount;i++)
		{
			if(m_arrUpdateDataInfoTable[i].nOffset>=nOffsetTemp&&m_arrUpdateDataInfoTable[i].nOffset<=nOffsetTemp+nDatasizeTemp)
			{
				if(nIndex>=1)
				{
					nIndex--;
				}
				arrUpdateTable[nIndex].nOffset = nOffsetTemp;
			}
			else if(m_arrUpdateDataInfoTable[i].nOffset<=nOffsetTemp)
			{
				arrUpdateTable[nIndex].nOffset = m_arrUpdateDataInfoTable[i].nOffset;
			}
			else if(m_arrUpdateDataInfoTable[i].nOffset==nOffsetTemp+nDatasizeTemp)
			{
				if(nIndex>=1)
				{
					nIndex--;
				}
				arrUpdateTable[nIndex].nOffset = nOffsetTemp;
				arrUpdateTable[nIndex].nDataSize = m_arrUpdateDataInfoTable[i].nDataSize+nDatasizeTemp;
				arrUpdateData[nIndex] = &pStartPos[arrUpdateTable[nIndex].nOffset];
				nIndex++;
				nOffsetTemp = arrUpdateTable[nIndex-1].nOffset;
				nDatasizeTemp = arrUpdateTable[nIndex-1].nDataSize;
				continue;
			}
			else if(m_arrUpdateDataInfoTable[i].nOffset>nOffsetTemp+nDatasizeTemp)
			{
				if(nIndex==0||arrUpdateTable[nIndex-1].nOffset != nOffsetTemp)
				{
					arrUpdateTable[nIndex].nOffset = nOffsetTemp;
					arrUpdateTable[nIndex].nDataSize = nDatasizeTemp;
					arrUpdateData[nIndex] = &pStartPos[arrUpdateTable[nIndex].nOffset];
					nIndex++;
				}
				arrUpdateTable[nIndex]= m_arrUpdateDataInfoTable[i];
				arrUpdateData[nIndex] = &pStartPos[arrUpdateTable[nIndex].nOffset];
				nIndex++;
				nOffsetTemp = arrUpdateTable[nIndex-1].nOffset;
				nDatasizeTemp = arrUpdateTable[nIndex-1].nDataSize;
				continue;
			}
			if(m_arrUpdateDataInfoTable[i].nOffset+m_arrUpdateDataInfoTable[i].nDataSize>nOffsetTemp+nDatasizeTemp)
			{
				arrUpdateTable[nIndex].nDataSize= m_arrUpdateDataInfoTable[i].nOffset+m_arrUpdateDataInfoTable[i].nDataSize-nOffsetTemp;
			}
			else
			{
				arrUpdateTable[nIndex].nDataSize = nDatasizeTemp;
			}
			nOffsetTemp = arrUpdateTable[nIndex].nOffset;
			nDatasizeTemp = arrUpdateTable[nIndex].nDataSize;
			arrUpdateData[nIndex] = &pStartPos[arrUpdateTable[nIndex].nOffset];
			nIndex++;
		}

		if(m_nUpdateDataInfoCount==1)
		{
			arrUpdateData[nIndex] = &pStartPos[m_arrUpdateDataInfoTable[0].nOffset];
			arrUpdateTable[nIndex] = m_arrUpdateDataInfoTable[0];
			nIndex++;
		}
		//清空
		m_nUpdateDataInfoCount = 0;
		memset(m_arrUpdateDataInfoTable,0,sizeof(m_arrUpdateDataInfoTable));

		return nIndex;
}

FRAME_HALLSERVER_NAMESPACE_END
