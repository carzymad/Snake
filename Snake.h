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
    struct termios term_orig, term_vi;

private:
    void change_cur(int x, int y);
    void create_snake();
    void create_food();
    void initial();
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
};

#endif

