#include "game.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <pthread.h>
#include <stdbool.h>
#include <locale.h>
#include <wchar.h>

void* input_thread_func(void* vargp)
{
    (void)vargp;

    int c;
    static struct termios oldt, newt;
    
    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while(running)
    {
        c = getchar();
        key_down(c);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return NULL;
}

int mssleep(long miliseconds)
{
   struct timespec rem;
   struct timespec req= {
       (int)(miliseconds / 1000),
       (miliseconds % 1000) * 1000000
   };

   return nanosleep(&req , &rem);
}

int main(void)
{
    setlocale(LC_ALL, "");
    printf("\x1b[s"); // Save cursor position
    init();

    pthread_t input_thread;
    pthread_create(&input_thread, NULL, input_thread_func, NULL);

    while(running)
    {
        loop();
        mssleep(200);
    }

    pthread_join(input_thread, NULL);

    return 0;
}
