#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void func()
{
    printf("1\n");
    sleep(3);
    printf("end\n");
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, &func, NULL);
    pthread_create(&t2, NULL, &func, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}