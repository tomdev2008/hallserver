/*
 * bll_event_buildplayerdata.h
 *
 *  Created on: 2012-5-16
 *      Author: fuz
 */

#ifndef BLL_EVENT_BUILDPLAYERDATA_H_
#define BLL_EVENT_BUILDPLAYERDATA_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "client_message_define.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_HALLSERVER_NAMESPACE_BEGIN

//�ָ��������,�Ȼָ��������ݣ�Ȼ��ָ��������
class CBuildPlayerDataMessageEvent : public CBllBase
{
public:
	CBuildPlayerDataMessageEvent()
	{
	}

	virtual ~CBuildPlayerDataMessageEvent()
	{
	}

	//��Ϣ�¼�
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

};

#define	CREATE_BUILDPLAYEREVENT_INSTANCE	CSingleton<CBuildPlayerDataMessageEvent>::CreateInstance
#define	GET_BUILDPLAYEREVENT_INSTANCE		CSingleton<CBuildPlayerDataMessageEvent>::GetInstance
#define	DESTROY_BUILDPLAYEREVENT_INSTANCE	CSingleton<CBuildPlayerDataMessageEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_BUILDPLAYERDATA_H_ */
