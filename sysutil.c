#include "sysutil.h"

/**
 * tcp_server - 启动tcp服务器
 * @host: 服务器IP地址或者服务器主机名
 * @port: 服务器端口
 * 成功返回监听套接字
 */
 // port = 0 会随机选择一个端口

 int tcp_server(const char* host, unsigned short port)
 {
	int listenfd;
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		ERR_EXIT("socket");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;

	if (host != NULL)
	{
		if (inet_aton(host, &servaddr.sin_addr) == 0)//说明是主机名
		{
			struct hostent *hp;
			if ((hp = gethostbyname(host)) == NULL)
				ERR_EXIT("gethostbyname");
			
			servaddr.sin_addr = *(struct in_addr*)hp->h_addr;
		}

	}
	else //绑定主机的所有ip地址
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	servaddr.sin_port = htons(port);
	
	int on = 1;
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
		ERR_EXIT("setsockopt");

	if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
		ERR_EXIT("bind");

	if (listen(listenfd, SOMAXCONN) == -1)
		ERR_EXIT("listen");

	return listenfd;
	
 }


// inet_aton将 点分十进制串cp 转换为一个网络字节顺序的32位整数 IP地址  
// 例如将串cp "192.168.33.123 "  
// 转为 1100 0000   1010 1000    0010 0001     0111 1011   
// 成功返回1，出错返回0  
// 转换成功，结果保存在结构体指针ap中  


/**
 * accept_timeout - 带超时的accept
 * @fd: 套接字
 * @addr: 输出参数，返回对方地址
 * @wait_seconds: 等待超时秒数，如果为0表示正常模式
 * 成功（未超时）返回已连接套接字，超时返回-1并且errno = ETIMEDOUT
 */
int accept_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds)
{
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if (wait_seconds > 0)
	{
		fd_set accept_fdset;
		struct timeval timeout;
		FD_ZERO(&accept_fdset);
		FD_SET(fd, &accept_fdset);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do
		{
			ret = select(fd + 1, &accept_fdset, NULL, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret == -1)
			return -1;
		else if (ret == 0)
		{
			errno = ETIMEDOUT;
			return -1;
		}
	}

	if (addr != NULL)
		ret = accept(fd, (struct sockaddr*)addr, &addrlen);
	else
		ret = accept(fd, NULL, NULL);
/*	if (ret == -1)
		ERR_EXIT("accept");
		*/

	return ret;
}


ssize_t writen(int fd, const void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nwritten;
	char *bufp = (char*)buf;

	while (nleft > 0)
	{
		if ((nwritten = write(fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if (nwritten == 0)
			continue;

		bufp += nwritten;
		nleft -= nwritten;
	}

	return count;
}
