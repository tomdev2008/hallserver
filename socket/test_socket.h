/*
 * test_socket.h
 *
 *  Created on: 2012-12-21
 *      Author: jimm
 */

#ifndef TEST_SOCKET_H_
#define TEST_SOCKET_H_

#include "common/common_codeengine.h"
#include "def/server_namespace.h"
#include "frame_outsidesocket.h"
#include "frame_socket_mgt.h"
#include "frame_protocolhead.h"
#include "public_typedef.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

//来自客户端的数据包经过封装后的最大大小
#define OUTSIDE_PACKET_SIZE			(0xffff + sizeof(ConnUin))

class CTestSocket : public COutsideSocket
{
public:
	CTestSocket();

	virtual ~CTestSocket();

	virtual int32_t OnRead(int32_t nErrorCode);

	virtual int32_t OnWrite(int32_t nErrorCode);

	virtual int32_t OnError(int32_t nErrorCode);

	virtual int32_t OnConnected();

	virtual int32_t OnDisconnect(int32_t nErrorCode);

	//接收连接回调
	virtual int32_t OnAccept(SocketFD nAcceptFD, struct sockaddr_in stPeerAddress,
			socklen_t nPeerAddressLen);

private:
	//发送缓存中剩余的数据
	int32_t SendRestData();

	//生成链接关闭事件
	int32_t MakeConnCloseEvent(uint16_t nEventID);

	//从接收缓存中提取一个消息包
	int32_t MakeMessage();

private:
	RoleID			m_nRoleID;
	uint16_t		m_nCurPacketSize;			//当前包的长度
	uint16_t		m_nPacketOffset;			//已经有多少数据了
	NetPacket		*m_pPacket;					//封装给应用层的网络包
};

FRAME_HALLSERVER_NAMESPACE_END


#endif /* TEST_SOCKET_H_ */
