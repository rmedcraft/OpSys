#include <stdio.h>
#include <pthread.h>

void print_message_function(void *ptr);
int main()
{
    pthread_t thread1, thread2;
    char *message1 = "hello";
    char *message2 = "world";
    pthread_create(&thread1, pthread_attr_default, (void *)&print_message_function, (void *)message1);
    sleep(10);
    pthread_create(&thread2, pthread_attr_default, (void *)&print_message_function, (void *)message2);
    sleep(10);
    exit(0);
}

void print_message_function(void *ptr)
{
    char *message;
    message = (char *)ptr;
    printf("%s", message);
    pthread_exit(0);
}