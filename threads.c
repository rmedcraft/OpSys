#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 4

void *hello(void *arg)
{
    printf("hello Thread\n");
}

void *threadFunc(void *pArg)
{
    int *p = (int *)pArg;
    int myNum = *p;
    printf("thread number %d\n", myNum);
}

int main()
{
    pthread_t tid[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // pthread_create(&tid[i], NULL, hello, NULL);
        pthread_create(&tid[i], NULL, threadFunc, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }
}