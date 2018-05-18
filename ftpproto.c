#include "ftpproto.h"
#include "sysutil.h"
#include "common.h"

void handle_ftp(session_t* sess)
{
	
	writen(sess->ctrl_fd, "220 (miniftp 0.1)\r\n", strlen("220 (miniftp 0.1)\r\n"));
	
	while (1)
	{
		memset(sess->cmdline, 0, sizeof(sess->cmdline));
		memset(sess->cmd, 0, sizeof(sess->cmd));
		memset(sess->args, 0, sizeof(sess->args));
		
		// 解析命令与参数
	}
}
