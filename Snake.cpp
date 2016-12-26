/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2016-12-25 21:00
 * Filename         : Snake.cpp
 * Description      : 
 *********************************************************/

#include "Snake.h"

void Game::change_cur(int x, int y) {
    char comm[50] = {0};
    sprintf(comm, "\033[%d;%dH", x+10, y+30);
    write(1, comm, strlen(comm));
}
void Game::create_snake() {
    int x, y;
    x = rand() % ROWS + 1;
    y = rand() % COLS + 1;
    statu[x][y] = true;
    player.body[0].x = x;
    player.body[0].y = y;
    change_cur(x, y);
    //printf("@");
    write(1, "@", 1);
}
void Game::create_food() {
    int x, y;
    do {
        x = rand() % ROWS + 1;
        y = rand() % COLS + 1;
    } while (statu[x][y]);
    food.x = x;
    food.y = y;
    change_cur(x, y);
    Write(40, 33, "$");
}
void Game::initial() {
    tcgetattr(0, &term_orig);
    term_vi = term_orig;
    term_vi.c_lflag &= (~ICANON & ~ECHO);	// leave ISIG ON- allow intr's
    term_vi.c_iflag &= (~IXON & ~ICRNL);
	term_vi.c_oflag &= (~ONLCR);
#ifndef linux
	term_vi.c_cc[VMIN] = 1;
	term_vi.c_cc[VTIME] = 0;
#endif
	tcsetattr(0, TCSANOW, &term_vi);

    direct = 0;
    write(1, "\033[?25l", strlen("\033[?251"));    // 隐藏光标
    srand(time(0));
    create_snake();
    create_food();
    for (int i = 0; i <= ROWS+1; i++) {
        statu[i][0] = statu[i][COLS+1] = true;
        change_cur(i, COLS+1);
        write(1, "|", 1);
        change_cur(i, 0);
        write(1, "|", 1);
    }
    for (int i = 0; i <= COLS+1; i++) {
        statu[0][i] = statu[ROWS+1][i] = true;
        change_cur(ROWS+1, i);
        write(1, "-", 1);
        change_cur(0, i);
        write(1, "-", 1);
    }
}
void Game::move() {
    int x = player.body[player.length-1].x;
    int y = player.body[player.length-1].y;
    int len = player.length;
    for (int i = len-1; i > 0; i--) {
        player.body[i].x = player.body[i-1].x;
        player.body[i].y = player.body[i-1].y;
    }
    if (food.x == player.body[0].x && food.y == player.body[0].y) {
        player.length++;
        player.body[len].x = x;
        player.body[len].y = y;
        create_food();
    } else {
        change_cur(x, y);
        write(1, " ", 1);
        statu[x][y] = false;
    }
    if (player.length != 1) {
        change_cur(player.body[0].x, player.body[0].y);
        Write(40, 32, "#");
    }
    if (direct == 1) {
        player.body[0].x--;
    } else if (direct == 2) {
        player.body[0].x++;
    } else if (direct == 3) {
        player.body[0].y--;
    } else if (direct == 4) {
        player.body[0].y++;
    }
    x = player.body[0].x;
    y = player.body[0].y;
    if (statu[x][y]) {
        flag = false;
    }
    change_cur(x, y);
    statu[x][y] = true;
    Write(40, 31, "@");
}
/*Game::Game(int) : player(1) {
    write(1, "\033[1J", strlen("\033[1J"));
    initial();
    flag = true;
}*/
Game::~Game() {
    change_cur(ROWS+2, 1);
    write(1, "\033[?25h", strlen("\033[?251"));     // 显示光标
    write(1, "game over", strlen("game over"));
    change_cur(ROWS+3, -29);
    tcsetattr(0, TCSANOW, &term_orig);
}
void Game::run() {
    do {
        usleep(SPEED);
        change_cur(ROWS+2, 1);
        char s[100] = {0};
        char op = buf[0];
        if (op == 'w' || op == 'W') op = 1;
        else if (op == 's' || op == 'S') op = 2;
        else if (op == 'a' || op == 'A') op = 3;
        else if (op == 'd' || op == 'D') op = 4;
        else op = 0;
        int ret = op ^ direct;
        sprintf(s, "分数:%d", player.length);
        write(1, s, strlen(s));
        if (direct == 0 || (ret != 7 && ret != 3)) {
            direct = op;
        }
        move();
    } while (flag);
}

void Game::Write(int bg, int font, char *str) {
    char buf_[256] = {0};
    sprintf(buf_, "\033[%d;%dm%s\033[0m", bg, font, str);
    write(1, buf_, strlen(buf_));
}
