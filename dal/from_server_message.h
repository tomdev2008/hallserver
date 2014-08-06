/*
 * from_server_message.h
 *
 *  Created on: 2011-12-6
 *      Author: jimm
 */

#ifndef FROM_SERVER_MESSAGE_H_
#define FROM_SERVER_MESSAGE_H_

#include "common/common_singleton.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_msg_impl.h"
#include "common/common_string.h"
#include "common/common_codeengine.h"
#include "def/dal_def.h"
#include "public_typedef.h"
#include "hallserver_message_define.h"
#include "tunnelserver_message_define.h"
#include "roomserver_message_define.h"
#include "public_message_define.h"
#include "daemon_message_define.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN


//��½����
class CGetRoomServerIDReq : public CGetRoomServerIDReq_Public
{
//public:
//	int32_t		MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
//	int32_t		MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
};

class CGetRoleInfoResp : public CHallGetRoleInfoResp_Public
{
//public:
//	int32_t		MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
};

//�ϱ�������Ϣ��hall
class CUpdateInfoToHallNotice : public CUpdateInfoToHallNotice_Public
{
public:
//	int32_t		MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
};

class CGetMediaInfoReq : public CGetMediaInfoReq_Public
{
public:
//	int32_t		MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
};

//���뷿��֪ͨ
class CEnterRoomNotice : public CEnterRoomNotice_Public
{
public:
//	int32_t		MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
};


//�˳�����֪ͨ
class CExitRoomNotice : public CExitRoomNotice_Public
{
public:
//	int32_t		MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
};

//�����ڴ�viplevel֪ͨ
class CUpdateMemVipLevelNotify : public CUpdateVipLevelNotify
{
};
//��������֪ͨ
class CCreateRoomNotice : public CCreateRoomNotice_Public
{
public:
//	int32_t		MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
//	int32_t		MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
};

//�����������ָ� player ����֪ͨ
class CBuildPlayerDataNoti : public CRecoverHallPlayerNoti_public
{
};

//�����������ָ� room ����֪ͨ
class CRecoverHallRoomNoti : public CRecoverHallRoomNoti_public
{
};

//�ָ���������room �� player ��������������tunnel��������һ����
class CTunnelUpdateDataNumNoti : public CTunnelUpdateDataNumNoti_public
{
};

//DAEMON�㲥�����з�������ĳ��������������֪ͨ
class CDaemonStartNotify : public CDaemonStartNotify_Public
{
};

//���ٷ���֪ͨ
class CDestoryRoomNotice : public CDestoryRoomNotice_Public
{
};
#define	CREATE_GETROLEINFORESPEVENT_INSTANCE	CSingleton<GetRoleInfoResp>::CreateInstance
#define	GET_GETROLEINFORESPEVENT_INSTANCE		CSingleton<GetRoleInfoResp>::GetInstance
#define	DESTROY_GETROLEINFORESPEVENT_INSTANCE	CSingleton<GetRoleInfoResp>::DestroyInstance

//add your message to map
MESSAGEMAP_DECL_BEGIN(HallServer_from_server)

MESSAGEMAP_DECL_MSG(MSGID_DBHS_GETROLELOGININFO_RESP, (new CGetRoleInfoResp()))

MESSAGEMAP_DECL_MSG(MSGID_TSHS_GETROOMSERVERID_REQ, (new CGetRoomServerIDReq()))

MESSAGEMAP_DECL_MSG(MSGID_RSHS_UPDATEINFOTOHALL_NOTICE, (new CUpdateInfoToHallNotice()))

MESSAGEMAP_DECL_MSG(MSGID_RSHS_GETMEDIAINFO_REQ, (new CGetMediaInfoReq()))

MESSAGEMAP_DECL_MSG(MSGID_RSMS_ENTER_ROOM_NOTICE, (new CEnterRoomNotice()))

MESSAGEMAP_DECL_MSG(MSGID_RSMS_EXIT_ROOM_NOTICE, (new CExitRoomNotice()))

MESSAGEMAP_DECL_MSG(MSGID_DBHS_GETROOM_ADMIN_COUNT_RESP, (new CHallGetRoomAdminCountResp()))

MESSAGEMAP_DECL_MSG(MSGID_DBHS_SET_TITLE_RESP, (new CHallSetTitleResp()))

MESSAGEMAP_DECL_MSG(MSGID_SSRS_UPDATELEVEL_NOTI, (new CUpdateMemVipLevelNotify()))

MESSAGEMAP_DECL_MSG(MSGID_DBHS_SETSELFINFO_RESP, (new CDBSetSelfSelfInfoResp()))

MESSAGEMAP_DECL_MSG(MSGID_RSMS_CREATE_ROOM_NOTICE, (new CCreateRoomNotice()))

MESSAGEMAP_DECL_MSG(MSGID_TSHS_HALLBUILDPLAYER_NOTI, (new CBuildPlayerDataNoti()))

MESSAGEMAP_DECL_MSG(MSGID_TSHS_HALLBUILDROOM_NOTI, (new CRecoverHallRoomNoti()))

MESSAGEMAP_DECL_MSG(MSGID_TSHS_UPDATEDATANUM_NOTI, (new CTunnelUpdateDataNumNoti()))

MESSAGEMAP_DECL_MSG(MSGID_DAEMON_START_NOTIFY, (new CDaemonStartNotify()))

MESSAGEMAP_DECL_MSG(MSGID_RSMS_DESTORY_ROOM_NOTICE, (new CDestoryRoomNotice()))

MESSAGEMAP_DECL_END()

FRAME_HALLSERVER_NAMESPACE_END

#endif /* FROM_SERVER_MESSAGE_H_ */
