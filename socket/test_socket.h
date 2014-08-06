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

//���Կͻ��˵����ݰ�������װ�������С
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

	//�������ӻص�
	virtual int32_t OnAccept(SocketFD nAcceptFD, struct sockaddr_in stPeerAddress,
			socklen_t nPeerAddressLen);

private:
	//���ͻ�����ʣ�������
	int32_t SendRestData();

	//�������ӹر��¼�
	int32_t MakeConnCloseEvent(uint16_t nEventID);

	//�ӽ��ջ�������ȡһ����Ϣ��
	int32_t MakeMessage();

private:
	RoleID			m_nRoleID;
	uint16_t		m_nCurPacketSize;			//��ǰ���ĳ���
	uint16_t		m_nPacketOffset;			//�Ѿ��ж���������
	NetPacket		*m_pPacket;					//��װ��Ӧ�ò�������
};

FRAME_HALLSERVER_NAMESPACE_END


#endif /* TEST_SOCKET_H_ */
