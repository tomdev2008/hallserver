/*
 * region_mgt.cpp
 *
 *  Created on: 2011-12-5
 *      Author: jimm
 */

#include "frame_errordef.h"
#include "region_mgt.h"
#include "server_datacenter.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

//取得当前分区的所有用户数量
int32_t CRegionMgt::GetRegionUserCount(const RegionID nRegionID)
{
	CRegion *pRegion = GetRegion(nRegionID);
	if(pRegion == NULL)
	{
		return 0;
	}

	CChannel *arrChannelObject[MaxChannelCountPerRegion] = { NULL };
	int32_t nChannelObjectCount = 0;

	pRegion->GetAllChannelObject(arrChannelObject, MaxChannelCountPerRegion, nChannelObjectCount);

	int32_t nRegionUserCount = 0;
	for(int32_t i = 0; i < nChannelObjectCount; ++i)
	{
		RoomID arrRoomID[MaxRoomCountPerChannel] = { enmInvalidRoomID };
		int32_t nRoomCount = 0;

		arrChannelObject[i]->GetAllRoom(arrRoomID, MaxRoomCountPerChannel, nRoomCount);
		for(int32_t j = 0; j < nRoomCount; ++j)
		{
			CRoom *pRoom = NULL;
			g_RoomMgt.GetRoom(arrRoomID[j], pRoom);
			if(pRoom == NULL)
			{
				continue;
			}

			nRegionUserCount += pRoom->GetTotalPeopleCount();
		}
	}

	return nRegionUserCount;
}


FRAME_HALLSERVER_NAMESPACE_END

