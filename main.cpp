/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2016-12-25 16:17
 * Filename         : main.cpp
 * Description      : 
 *********************************************************/

#include "def.h"
#include "Snake.h"

char buf[256] = {0};
void* listen_func(void*) {
    while (1) {
        read(1, buf, sizeof(buf));
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    //system("stty -echo -icanon");
    pthread_t thread_listen;
    pthread_create(&thread_listen, NULL, listen_func, NULL);
    Game *a = new Game(1);
    a->run();
    delete a;
    //system("stty echo icanon");

	return 0;
}
