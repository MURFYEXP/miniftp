#ifndef SESSION_H_
#define SESSION_H_
#include "common.h"

typedef struct session
{
	//控制连接
	int ctrl_fd;
	char cmdline[MAX_COMMAND_LINE];
	char cmd[MAX_COMMAND];
	char args[MAX_ARG];

	// ftp协议进程与nobody进程通信
	int ftp_fd;
	int nobody_fd;

} session_t;


void begin_session(session_t* sess);

#endif // session.h
