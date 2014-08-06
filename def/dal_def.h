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
//�Ự���Ͷ���
enum
{
	enmSessionType_Invalid = 0x00,
	enmSessionType_GetRoleInfo = 0x01, //��ȡ��ɫ��Ϣ
	enmSessionType_GetRoomInfo = 0x02, //��ȡ������Ϣ
};

//��ʱ�����Ͷ���
enum
{

};

//��ʱ����ʱʱ�䶨��
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
	enmKickReason_Relogin = 0x00, //�ظ���½
	enmKickReason_SendAd = 0x01, //���͹��
	enmKickReason_PlayerLock = 0x10,	//��ұ���
	enmKickReason_IPLock = 0x11,	//IP����
	enmKickReason_MacLock = 0x12,	//Mac����
	enmKickReason_Other = 0xff,
};


typedef uint32_t ChangeResult;
enum
{
	enmChangeResult_OK = 0x00,
	enmChangeResult_NO_PERMISSION = 0x01, //û��Ȩ��
	enmChangeResult_Is_Full = 0x02, //�����õĹ�������
	enmChangeResult_Is_AdminInOther = 0x03, //�������ķ����й���Ȩ��
	enmChangeResult_UNKNOWN = 0xff,

};

//����Ự����
enum
{
	enmSessionType_GetAdminCount = 0x04, //��ȡ��ҹ���Ȩ����Ϣ
	enmSessionType_DelTitle = 0x05, //ɾ������Ȩ��
	enmSessionType_UpdateTitle = 0x06, //���¹���Ȩ��
	enmSessionType_SetSelfInfo = 0x07,//����������Ϣ�������ݿ�
};

enum
{
	enmSetInfo_OK = 0x00, //�ɹ�
	enmSetInfo_NOT_ALLOWED = 0x01, //���ֶβ������޸�
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
//	enmInitializing				= 0, // �ָ������л��߳�ʼ����
//	enmNormal					= 1, // ������ ����ͨ��
//};

FRAME_HALLSERVER_NAMESPACE_END

#endif /* DAL_DEF_H_ */
