#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
/*
This program provides a possible solution for Domino-Piazz problem using
mutex and semaphore.
You use 5 kitchenGuys and 5 deliverGuys to demonstrate the solution.
*/
#define MaxItems 5    // Maximum items a kitchenGuy can produce or a deliverGuy can deliver
#define StackerSize 5 // Size of the stacker
sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int stacker[StackerSize];
pthread_mutex_t mutex;

// producer
void *kitchenGuy(void *pno)
{
    int item;
    for (int i = 0; i < MaxItems; i++)
    {
        item = rand(); // Produce an random item
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        stacker[in] = item;
        printf("KitchenGuy %d: Insert Item %d at %d\n", *((int *)pno), stacker[in], in);
        in = (in + 1) % StackerSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

// consumer
void *deliverGuy(void *cno)
{
    int item;
    for (int i = 0; i < MaxItems; i++)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        // out is the index of where the delivery guy is taking from
        item = stacker[out];
        printf("DeliveryGuy %d: Remove Item %d at %d\n", *((int *)cno), item, out);
        // out + 1, but not bigger than the stacker size
        out = (out + 1) % StackerSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main()
{
    pthread_t pro[5], con[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, StackerSize);
    sem_init(&full, 0, 0);
    int a[5] = {1, 2, 3, 4, 5}; // Just used for numbering the kitchenGuy and deliverGuy

    // creates 5 threads for kitchen guys (producers)
    for (int i = 0; i < 5; i++)
    {
        // parameters are the thread, the function the thread is running, and the parameter passed to the function
        pthread_create(&pro[i], NULL, kitchenGuy, (void *)&a[i]);
    }
    // creates 5 threads for delivery guys (consumers)
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&con[i], NULL, deliverGuy, (void *)&a[i]);
    }
    // join the kitchen guys
    for (int i = 0; i < 5; i++)
    {
        pthread_join(pro[i], NULL);
    }
    // join the delivery guys
    for (int i = 0; i < 5; i++)
    {
        pthread_join(con[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}