/*
 * main_frame.h
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_

#include "lightframe.h"
#include "common/common_errordef.h"
#include "common/common_singleton.h"
#include "def/server_namespace.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

class CMainFrame : public CLightFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

public:
	//¿ò¼Ü³õÊ¼»°
	int32_t Initialize();
	//»Ö¸´¿ò¼Ü
	int32_t Resume();
	//Ïú»Ù¿ò¼Ü
	int32_t Terminate();

	void Run();

	void Reload();

//	void OnTestRequest();
protected:
//	int32_t RegistSocket();

	void RegistMsg();
};

#define	CREATE_MAINFRAME_INSTANCE	CSingleton<CMainFrame>::CreateInstance
#define	GET_MAINFRAME_INSTANCE		CSingleton<CMainFrame>::GetInstance
#define	DESTROY_MAINFRAME_INSTANCE	CSingleton<CMainFrame>::DestroyInstance

#define g_Frame	GET_MAINFRAME_INSTANCE()


#define WRITE_LOG(level, fmt, ...) \
	do{ g_Frame.WriteLog(level, __FILE__,  __LINE__,""fmt"\n", ##__VA_ARGS__); } while (false)

#define WRITE_DEBUG_LOG(fmt, ...)		WRITE_LOG(enmLogLevel_Debug, fmt, ##__VA_ARGS__)
#define WRITE_INFO_LOG(fmt, ...)	WRITE_LOG(enmLogLevel_Info, fmt, ##__VA_ARGS__)
#define WRITE_NOTICE_LOG(fmt, ...)	WRITE_LOG(enmLogLevel_Notice, fmt, ##__VA_ARGS__)
#define WRITE_WARNING_LOG(fmt, ...)	WRITE_LOG(enmLogLevel_Warning, fmt, ##__VA_ARGS__)
#define WRITE_ERROR_LOG(fmt, ...)	WRITE_LOG(enmLogLevel_Error, fmt, ##__VA_ARGS__)

#define _WRITE_LOG(level, fmt, ...)\
		do{ g_Frame.WriteUserLog("user_online_statistic_", level, __FILE__,  __LINE__, ""fmt"\n", ##__VA_ARGS__); } while (false)

#define STATISTIC_WRITE_DEBUG_LOG(fmt, ...)		_WRITE_LOG(enmLogLevel_Debug, fmt, ##__VA_ARGS__)
#define STATISTIC_WRITE_INFO_LOG(fmt, ...)		_WRITE_LOG(enmLogLevel_Info, fmt, ##__VA_ARGS__)
#define STATISTIC_WRITE_NOTICE_LOG(fmt, ...)	_WRITE_LOG(enmLogLevel_Notice, fmt, ##__VA_ARGS__)
#define STATISTIC_WRITE_WARNING_LOG(fmt, ...)	_WRITE_LOG(enmLogLevel_Warning, fmt, ##__VA_ARGS__)
#define STATISTIC_WRITE_ERROR_LOG(fmt, ...)		_WRITE_LOG(enmLogLevel_Error, fmt, ##__VA_ARGS__)

FRAME_HALLSERVER_NAMESPACE_END


#endif /* MAIN_FRAME_H_ */
