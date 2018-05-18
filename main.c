#include "common.h"
#include "sysutil.h"
#include "session.h"


int main(void)
{

	int listenfd = tcp_server(NULL, 5188);
	int conn;
	session_t sess =
	{
		//控制连接
		-1, "", "", "",
		-1, -1
	};

	while(1)
	{
		if ((conn = accept_timeout(listenfd, NULL, 0)) < 0)
			ERR_EXIT("accept_timeout");
		

		pid_t pid = fork();
		if (pid == -1)
		{
			ERR_EXIT("fork");
		}

		if (pid == 0)
		{
			sess.ctrl_fd = conn;
			close(listenfd);
			begin_session(&sess);
		}
		else if (pid > 0)
		{
			close(conn);
		}
	}

	return 0;
}

