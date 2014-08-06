/*
 * bll_event_buildplayerdata.cpp
 *
 *  Created on: 2012-5-16
 *      Author: fuz
 */

#include "bll_event_buildplayerdata.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "hallserver_message_define.h"
#include "client_message_define.h"
#include "dal/to_server_message.h"
#include "dal/from_server_message.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

int32_t CBuildPlayerDataMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody == NULL || pMsgHead == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}
	CBuildPlayerDataNoti *pNoti = dynamic_cast<CBuildPlayerDataNoti *>(pMsgBody);
	if(pNoti == NULL)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CBuildPlayerDataNoti class failed!{nMessageID=0x%08x, nSourceID=%d}\n",
				pMsgHead->nMessageID, pMsgHead->nSourceID);
		return E_NULLPOINTER;
	}

	WRITE_DEBUG_LOG("got build player data message notify!{nPlayerCount=%d}\n", pNoti->nPlayerCount);


	// build the player data
	for(int32_t i = 0; i < pNoti->nPlayerCount; ++i)
	{
		CPlayer* pLocalPlayer = NULL;
		int32_t ret = g_PlayerMgt.GetPlayer(pNoti->arrRoleID[i], pLocalPlayer);
		if(ret < 0 || pLocalPlayer == NULL)
		{
			// build a local player by player cache
			ret = g_PlayerMgt.CreatePlayer(pNoti->arrRoleID[i], pLocalPlayer);
			uint32_t nCount = g_PlayerMgt.GetRealPlayerCount();
			g_PlayerMgt.SetRealPlayerCount(++nCount);
			if(ret < 0 || pLocalPlayer == NULL)
			{
				WRITE_ERROR_LOG("build player:create player error!{ret=0x%08x}", ret);
				continue;
			}

			uint32_t nStartPos = pLocalPlayer->GetStartPos();
			uint32_t nEndPos = pLocalPlayer->GetEndPos();

			uint32_t nPlayerDataSize = nEndPos - nStartPos;
			if(nPlayerDataSize >= pNoti->arrPlayerDataSize[i])
			{
				memcpy(((uint8_t *)pLocalPlayer) + nStartPos, pNoti->arrPlayerData[i],
						pNoti->arrPlayerDataSize[i]);

				// verify if correctly built, print out!
				char szPlayer[MaxUpdateDataSize] = {0};
				uint32_t offset = 0;
				sprintf(szPlayer + offset, "built player!{nRoleID=%d, strRoleName=%s, nAccountID=%d, "
						"nVipLevel=%d, nUserLevel=%d, nPlayerGender=%d, nCurRoomCount=%d, ",
						pLocalPlayer->GetRoleID(), pLocalPlayer->GetRoleName(), pLocalPlayer->GetAccountID(),
						pLocalPlayer->GetVipLevel(), pLocalPlayer->GetUserLevel(), pLocalPlayer->GetPlayerGender(),
						pLocalPlayer->GetCurEnterRoomCount());
				offset = (uint32_t) strlen(szPlayer);
				if(pLocalPlayer->GetCurEnterRoomCount() > 0)
				{
					sprintf(szPlayer + offset, "{");
					offset = (uint32_t) strlen(szPlayer);

					PlayerRoomInfo arrPlayerRoomInfo[MaxEnterRoomCount];
					int32_t nActualCount = 0;
					pLocalPlayer->GetPlayerRoomInfo(arrPlayerRoomInfo, MaxEnterRoomCount, nActualCount);
					for(int32_t i = 0; i < nActualCount; ++i)
					{
						char* fmt = NULL;
						if(i < nActualCount - 1)
							fmt = "{nServerID=%d, nRoomID=%d, nPlayerState=%d}, ";
						else
							fmt = "{nServerID=%d, nRoomID=%d, nPlayerState=%d}}, ";

						sprintf(szPlayer + offset, fmt, arrPlayerRoomInfo[i].nServerID,
								arrPlayerRoomInfo[i].nRoomID, arrPlayerRoomInfo[i].nPlayerState);
						offset = (uint32_t) strlen(szPlayer);
					}
				}
				sprintf(szPlayer + offset, "nLoginTime=%ld, nTunnelIndex=%d, nServerID=%d, nOnlineTime=%lu, "
						"nLeftMoney=%d, nExperience=%d, nLastVersion=%d, nIsRobot=%d, nAdminCount=%d, ",
						pLocalPlayer->GetLoginTime(), pLocalPlayer->GetConnInfo().nTunnelIndex, pLocalPlayer->GetConnInfo().nServerID,
						pLocalPlayer->GetOnlineTime(),pLocalPlayer->GetLeftMoney(), pLocalPlayer->GetExperience(),
						pLocalPlayer->GetLastVersion(), (int32_t)pLocalPlayer->IsRobot(), pLocalPlayer->GetPlayerAdminCount());
				offset = (uint32_t) strlen(szPlayer);
				if(pLocalPlayer->GetPlayerAdminCount() > 0)
				{
					sprintf(szPlayer + offset, "{");
					offset = (uint32_t) strlen(szPlayer);

					RoomID arrRoomIDS[MaxBeAdminPerPlayer];
					int32_t nActualCount = 0;
					pLocalPlayer->GetAllAdminRoom(arrRoomIDS, MaxBeAdminPerPlayer, nActualCount);
					for(int32_t i = 0; i < nActualCount; ++i)
					{
						char* fmt = NULL;
						if(i < nActualCount - 1)
							fmt = "{nRoomID=%d, nRoleRank=%d}, ";
						else
							fmt = "{nRoomID=%d, nRoleRank=%d}}, ";

						sprintf(szPlayer + offset, fmt, arrRoomIDS[i], pLocalPlayer->GetAdminRoleRank(arrRoomIDS[i]));
						offset = (uint32_t) strlen(szPlayer);
					}
				}

				sprintf(szPlayer + offset, "nIdentityType=%u, nPlayerState=%d, nCurNewPlayerRoomCount=%u}\n",
						pLocalPlayer->GetIdentityType(), pLocalPlayer->GetPlayerState(), pLocalPlayer->GetNewPlayerRoomCount());
				offset = (uint32_t) strlen(szPlayer);

				WRITE_DEBUG_LOG("%s", szPlayer);
			}
			else
			{
				continue;
			}

			// build room player list
			RoomID arrRoomID[MaxEnterRoomCount];
			int32_t nEnterRoomCount = 0;
			pLocalPlayer->GetAllEnterRoom(arrRoomID, MaxEnterRoomCount, nEnterRoomCount);
			for(int32_t j = 0; j < nEnterRoomCount; ++j)
			{
				CRoom *pRoom = NULL;
				ret = g_RoomMgt.GetRoom(arrRoomID[j], pRoom);
				if(ret < 0 || pRoom == NULL)
				{
					WRITE_ERROR_LOG("building player data, failed to find room object!{nRoomID=%d}\n",
							arrRoomID[j]);
					continue;
				}

				if(pRoom->IsPlayerInRoom(pNoti->arrRoleID[i]))
				{
					WRITE_ERROR_LOG("building player data, the player should not in this room!{nRoleID=%d, nRoomID=%d}\n",
							pNoti->arrRoleID[i], arrRoomID[j]);
					continue;
				}

				//将玩家添加到房间对象中去
				pRoom->AddPlayer(pNoti->arrRoleID[i]);

			}
		}
	}


	return S_OK;
}

FRAME_HALLSERVER_NAMESPACE_END
