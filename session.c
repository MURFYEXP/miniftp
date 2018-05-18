#include "session.h"
#include "nobody.h"
#include "ftpproto.h"
#include "sysutil.h"

void begin_session(session_t* sess)
{	
	struct passwd *	pw = getpwnam("nodoby");
	if(pw == NULL)
		return;
	setgid(pw->pw_gid);
	setuid(pw->pw_uid);

	int sockfds[2];
	if ((socketpair(PF_UNIX, SOCK_STREAM, 0, sockfds)) < 0)
		ERR_EXIT("socketpair");

	pid_t pid = fork();
	if (pid == -1)
		ERR_EXIT("fork");

	

	if (pid == 0)
	{
		// ftp协议解析进程
		close(sockfds[0]); // 子进程使用sockfds[1]与父进程进行通信
		sess->ftp_fd = sockfds[1];
		handle_ftp(sess);

	}
	else if (pid > 0)
	{
		// nobody进程是父进程
		close(sockfds[1]); // 父进程使用sockfds[0]与子进程进行通信
		sess->nobody_fd = sockfds[0];
		handle_nobody(sess);

	}

}
