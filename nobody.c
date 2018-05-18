#include "nobody.h"
#include "common.h"

void handle_nobody(session_t* sess)
{
	
	char cmd;

	while (1)
	{
		read(sess->nobody_fd, &cmd, 1);
		//解析内部命令
		//处理内部命令
	}
}
