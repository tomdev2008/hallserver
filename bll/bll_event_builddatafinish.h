/*
 * bll_event_builddatafinish.h
 *
 *  Created on: 2013-1-18
 *      Author: liufl
 */

#ifndef BLL_EVENT_BUILDDATAFINISH_H_
#define BLL_EVENT_BUILDDATAFINISH_H_
#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "ctl/server_datacenter.h"
#include "client_message_define.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"

#include "hallserver_message_define.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

//֪ͨ��Ҫ�ָ������ݸ����������������ݺ��������
class CBuilddatafinishMessageEvent : public CBllBase
{
public:
	CBuilddatafinishMessageEvent()
	{
	}

	virtual ~CBuilddatafinishMessageEvent()
	{
	}

	//��Ϣ�¼�
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

};

#define	CREATE_BUILDDATADINISHEVENT_INSTANCE	CSingleton<CBuilddatafinishMessageEvent>::CreateInstance
#define	GET_BUILDDATADINISHEVENT_INSTANCE		CSingleton<CBuilddatafinishMessageEvent>::GetInstance
#define	DESTROY_BUILDDATADINISHEVENT_INSTANCE	CSingleton<CBuilddatafinishMessageEvent>::DestroyInstance

FRAME_HALLSERVER_NAMESPACE_END

#endif /* BLL_EVENT_BUILDDATAFINISH_H_ */
