/*
 * def_message_id.h
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#ifndef DEF_MESSAGE_ID_H_
#define DEF_MESSAGE_ID_H_

//��½��������
#define MSGID_CLHS_LOGIN_REQ			0x00018001

//��½������Ӧ
#define MSGID_HSCL_LOGIN_RESP			0x00020401

//��½�����ǳ�
#define MSGID_CLHS_LOGOUT_REQ			0x00018002

//�߳��û�
#define MSGID_HSCL_KICKUSER_NOTI		0x00030405

//���ӹ���Ȩ��֪ͨ(to client)
#define MSGID_HSCL_ADD_TITLE_NOTI		0x00030719

//���ӹ���Ȩ������
#define MSGID_CLHS_ADD_TITLE_REQ		0x00018019

//���ӹ���Ȩ����Ӧ
#define MSGID_HSCL_ADD_TITLE_RESP		0x00020719

//�޸�Ȩ��֪ͨ(to server)
#define MSGID_HSTA_MODIFY_TITLE_NOTIFY	0x00130407

//ɾ��Ȩ������
#define MSGID_CLHS_DEL_TITLE_REQ		0x0001801a

//ɾ��Ȩ����Ӧ
#define MSGID_HSCL_DEL_TITLE_RESP		0x0002071a

//ɾ��Ȩ��֪ͨ(to client)
#define MSGID_HSTA_DEL_TITLE_NOTIFY		0x0003071a

//�������������Ϣ������
#define MSGID_CLHS_SETSELFINFO_REQ		0x00018004

//�������������Ϣ����Ӧ
#define MSGID_HSCL_SETSELFINFO_RESP 	0x00020404

//��������ǳƺ��Ա�֪ͨ
#define MSGID_HSCL_UPDATEUSERINFONOTI	0x00030408

#endif /* DEF_MESSAGE_ID_H_ */
