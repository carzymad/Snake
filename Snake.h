/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2016-12-25 20:57
 * Filename         : Snake.h
 * Description      : 
 *********************************************************/

#ifndef _SNAKE_H
#define _SNAKE_H

#include "def.h"

extern char buf[256];

struct Tcp {
	int fd;			// 套接字
	sockaddr_in addr;
	int port;
	char ip[20];
};

struct Udp {
	
};

struct pos {
    int x, y;
};

struct Snake {
    Snake(int) {
        length = 1;
        memset(body, 0, sizeof(body));
    }
    pos body[100];
    int length;
};

class Game {
private:
    int direct;     // 当前前进方向
    pos food;
    Snake player;
    Snake player2;
    bool statu[ROWS+10][COLS+10];
    bool flag;
	bool is_net;			// 判断是否是联机状态
    struct termios term_orig, term_vi;
	Tcp tcp;				// tcp连接信息

private:
    void change_cur(int x, int y);
    void create_snake();
    void create_food();
    void initial();
	void link_server_tcp();		// 连接服务器,tcp连接
	void break_server_tcp();	// 断开服务器连接，tcp连接
	void link_server_udp();		// 连接服务器，udp连接
	void break_server_udp();	// 断开服务器连接，udp连接
    void move();

    void Write(int bg, int font, char *str);

public:
    Game(int) : player(1), player2(1) {
        write(1, "\033[1J", strlen("\033[1J"));
        initial();
        flag = true;
    }
    ~Game();
    void run();
	void Send(char *buf_);

public:
	static void* tcp_listen(void*); 
};

#endif

