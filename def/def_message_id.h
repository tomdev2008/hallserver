/*
 * def_message_id.h
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#ifndef DEF_MESSAGE_ID_H_
#define DEF_MESSAGE_ID_H_

//登陆大厅请求
#define MSGID_CLHS_LOGIN_REQ			0x00018001

//登陆大厅响应
#define MSGID_HSCL_LOGIN_RESP			0x00020401

//登陆大厅登出
#define MSGID_CLHS_LOGOUT_REQ			0x00018002

//踢出用户
#define MSGID_HSCL_KICKUSER_NOTI		0x00030405

//增加管理权限通知(to client)
#define MSGID_HSCL_ADD_TITLE_NOTI		0x00030719

//增加管理权限请求
#define MSGID_CLHS_ADD_TITLE_REQ		0x00018019

//增加管理权限响应
#define MSGID_HSCL_ADD_TITLE_RESP		0x00020719

//修改权限通知(to server)
#define MSGID_HSTA_MODIFY_TITLE_NOTIFY	0x00130407

//删除权限请求
#define MSGID_CLHS_DEL_TITLE_REQ		0x0001801a

//删除权限响应
#define MSGID_HSCL_DEL_TITLE_RESP		0x0002071a

//删除权限通知(to client)
#define MSGID_HSTA_DEL_TITLE_NOTIFY		0x0003071a

//设置玩家自身信息的请求
#define MSGID_CLHS_SETSELFINFO_REQ		0x00018004

//设置玩家自身信息的响应
#define MSGID_HSCL_SETSELFINFO_RESP 	0x00020404

//更新玩家昵称和性别通知
#define MSGID_HSCL_UPDATEUSERINFONOTI	0x00030408

#endif /* DEF_MESSAGE_ID_H_ */
