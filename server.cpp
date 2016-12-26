/*********************************************************
 * Author			: crazymad
 * Last modified	: 2016-11-20 22:18
 * Filename			: main.cpp
 * Description		: 模拟webserver发送response数据
 *********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <fstream>

using namespace std;

char buf[1024];

int main(int argc, char* argv[])
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);			// 服务端套接字
	int on = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
		printf("setsockopt error: %s\n", errno);
		exit(1);
	}
	sockaddr_in addr;			// 服务器地址信息
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8081);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(fd, (sockaddr*)&addr, sizeof(addr)) == -1) {
		printf("bind error: %s\n", strerror(errno));
		exit(1);
	}
	listen(fd, 20);		// 同时等待的最大队列长度
	int client_fd;		// 客户端套接字
	srand(time(0));
	while (1) {
		sockaddr_in client_addr;	// 客户端地址
		socklen_t len = sizeof(client_addr);
		memset(&client_addr, 0, sizeof(client_addr));
		client_fd = accept(fd, (sockaddr*)&client_addr, &len);
		int ret;
		while (1) {
			ret = recv(client_fd, buf, sizeof(buf), 0);
			if (ret > 0) {
				printf("%s\n", buf);
			} else {
				printf("连接中断\n");
				break;
			}
			if (!strcmp(buf, "create_food")) {
				memset(buf, 0, sizeof(buf));
				int x = rand() % 1000;
				int y = rand() % 1000;;
				sprintf(buf, "%d %d", x, y);
				printf("%s\n", buf);
			}
		} 
		close(client_fd);
	}

	close(fd);
	return 0;
}
