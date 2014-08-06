/*
 * main.cpp
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#endif
#include "frame_errordef.h"
#include "frame_logengine.h"
#include "main_frame.h"
#include "def/dal_def.h"
#include "lightframe.h"


using namespace FRAME_NAMESPACE;
using namespace FRAME_HALLSERVER_NAMESPACE;

#define LOGPREFIX			"log/"SERVER_NAME_STRING

#ifndef WIN32

void sigusr1_handle(int32_t nSigVal)
{
	g_eServerCommand = enmServerCommand_Reload;
	signal(SIGUSR1, sigusr1_handle);
}

void sigusr2_handle(int32_t nSigVal)
{
	g_eServerCommand = enmServerCommand_Terminate;
	signal(SIGUSR2, sigusr2_handle);
}

#endif



void print_version()
{
	/*
	printf("%s, %s\n", SERVER_NAME, SERVER_VERSION);
#ifdef DEBUG
	printf("only for debug\n");
#else
	printf("for release\n");
#endif
	*/
	char version[128] = { 0 };
	GET_VERSION(version);

	printf("%s, %s\n", SERVER_NAME_STRING, version);
#ifdef DEBUG
	printf("Build for debug\n");
#else
	printf("Build for release\n");
#endif

#ifdef SVNVER
	printf("svn version no:%d\n", SVNVER);
#endif

#ifdef SVNDIR
	printf("source dir in svn:%s\n", SVNDIR);
#endif

	printf("\n");

	print_version_common_lib();
	print_version_frame_lib();
}

void print_help()
{
	printf("directories:\n");
	printf("\t./config: include all config files\n");
	printf("\t./log: include all log file\n");
	printf("config files:\n");
	printf("\t"SERVER_NAME_STRING".xml: main config file of "SERVER_NAME_STRING"\n");
	printf("log files:\n");
	printf("\t"SERVER_NAME_STRING"_yyyymmdd.log\n");
	printf("\t"SERVER_NAME_STRING"_main_s_s_yyyymmdd.log\n");
	printf("\t"SERVER_NAME_STRING"_netio_s_s_yyyymmdd.log\n");
}

int32_t main(int argc, char** argv)
{
	bool bResume = false;
	bool bDaemon = false;

	//�������ʵ������
	CREATE_MAINFRAME_INSTANCE();

	if(argc <= 1)
	{
		g_bTestMode = true;
	}
	//��ȡ�����в���
	if (argc > 1)
	{
		if (0 == strcasecmp((const char*)argv[1], "-v"))
		{
			print_version();
			exit(0);
		}

		if (0 == strcasecmp((const char*)argv[1], "-h"))
		{
			print_help();
			exit(0);
		}

		if (0 == strcasecmp((const char*)argv[1], "-rd"))
		{
			bResume = true;
			bDaemon = true;
		}

		if (0 == strcasecmp(argv[1], "-d"))
		{
			bResume = false;
			bDaemon = true;
		}
	}
#ifndef WIN32
	int lock_fd = open((const char *)argv[0], O_RDONLY);
	if (lock_fd < 0)
	{
		exit(1);
	}

	if (flock(lock_fd, LOCK_EX | LOCK_NB) < 0)
	{
		CFrameLogEngine::WriteLog(LOGPREFIX, enmLogLevel_Error, SERVER_NAME_STRING" was launched!\n");
		exit(1);
	}
#endif

#ifndef WIN32
	//��ʼ��Ϊ�ػ�����
	if (bDaemon)
	{
		g_Frame.SetDaemon();
	}

	//��װ�źŴ�����
	signal(SIGUSR1, sigusr1_handle);
	signal(SIGUSR2, sigusr2_handle);
#endif


	int32_t ret = S_OK;

	//��ʼ��������
	ret = g_Frame.Initialize();
	if (0 <= ret)
	{
		//����������
		ret = g_Frame.Start();
		if (0 <= ret)
		{
			//���߳̽�������״̬
			g_Frame.Run();
		}
	}

	//����������
	ret = g_Frame.Uninitialize();
	if (0 > ret)
	{
		exit(1);
	}

	CFrameLogEngine::WriteLog(LOGPREFIX, enmLogLevel_Notice, SERVER_NAME_STRING" exited safe!\n\n\n");

	exit(0);
}
