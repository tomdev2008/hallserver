/*
 * dal_def.h
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#ifndef DAL_DEF_H_
#define DAL_DEF_H_

#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "common/common_list.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

#define SERVER_NAME_STRING		"hallserver"
#define SERVER_VERSION			"Version: 1.0.build1, Release Date: 2014-06-28"

#define MaxTokenBuffSize		2048
#define MaxTunnelNum			64

#define MAXROLEIDPERIP			1024
#define MAXROLEIDPERMAC			32
//会话类型定义
enum
{
	enmSessionType_Invalid = 0x00,
	enmSessionType_GetRoleInfo = 0x01, //获取角色信息
	enmSessionType_GetRoomInfo = 0x02, //获取房间信息
};

//定时器类型定义
enum
{

};

//定时器超时时间定义
enum
{
	enmGetRoleInfoTimeoutPeriod = 3000 * US_PER_MS,
	enmGetRoomAdminCountTimeoutPerios = 3000 * US_PER_MS,
	enmSetSelfInfoTimeoutPerios = 3000 * US_PER_MS,
};

typedef uint32_t LoginResult;
enum
{
	enmLoginResult_OK = 0x00,
	enmLoginResult_AccountLock = 0x01,
	enmLoginResult_IpLock = 0x02,
	enmLoginResult_MacLock = 0x03,
	enmLoginResult_Timeout = 0x04,
	enmLoginResult_VerError = 0x05,
	enmLoginResult_Unknown = 0xff,
};
enum
{
	enmGetOnlinePlayerInfoResult_Unknown   = 0x80410101,
	enmGetGlobalPlayerInfoResult_Unknown   = 0x80410102,
	enmFetchRoleOnlineStatusResult_Unknown = 0x80410103,
};

typedef uint32_t KickReason;
enum
{
	enmKickReason_Relogin = 0x00, //重复登陆
	enmKickReason_SendAd = 0x01, //发送广告
	enmKickReason_PlayerLock = 0x10,	//玩家被封
	enmKickReason_IPLock = 0x11,	//IP被封
	enmKickReason_MacLock = 0x12,	//Mac被封
	enmKickReason_Other = 0xff,
};


typedef uint32_t ChangeResult;
enum
{
	enmChangeResult_OK = 0x00,
	enmChangeResult_NO_PERMISSION = 0x01, //没有权限
	enmChangeResult_Is_Full = 0x02, //被设置的管理已满
	enmChangeResult_Is_AdminInOther = 0x03, //在其他的房间有管理权限
	enmChangeResult_UNKNOWN = 0xff,

};

//定义会话类型
enum
{
	enmSessionType_GetAdminCount = 0x04, //获取玩家管理权限信息
	enmSessionType_DelTitle = 0x05, //删除管理权限
	enmSessionType_UpdateTitle = 0x06, //更新管理权限
	enmSessionType_SetSelfInfo = 0x07,//设置自身信息更新数据库
};

enum
{
	enmSetInfo_OK = 0x00, //成功
	enmSetInfo_NOT_ALLOWED = 0x01, //有字段不允许修改
	enmSetInfo_UNKNOWN = 0xff,
};

enum
{
	enmCheckOnlineNumTimePeriod = 5, // minutes
	enmCheckCrossDaysTimePeriod = 1, // minutes
	enmCheckCrossDaysBeforeTomo = 1, // minutes
	enmBuildRoomDataTimeOut		= 10,//seconds
	enmCheckDataNumTimePeriod = 10, // seconds
};

enum
{
	enmMaxQueueIndex = 8,
};

//typedef uint8_t ServerStatus;
//enum
//{
//	enmInitializing				= 0, // 恢复数据中或者初始化中
//	enmNormal					= 1, // 正常， 可以通信
//};

FRAME_HALLSERVER_NAMESPACE_END

#endif /* DAL_DEF_H_ */
