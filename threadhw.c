#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int myglobal;
void *thread_function(void *arg) {
    int j;
    for(int i = 0; i < 20; i++) {
        j = myglobal;
        j++;
        printf(".");
        fflush(stdout);
        sleep(1);
        myglobal = j;
        // myglobal++; // ?????? 
    }
    return NULL;
}

int main(void) {
    printf("%d\n", myglobal);
    fflush(stdout);
    
    pthread_t mythread;
    if(pthread_create(&mythread, NULL, thread_function, NULL)) { // returning a 1 is an error code in C
        printf("ldquo;error creating thread");
        abort();
    }

    for(int i = 0; i < 20; i++){
        myglobal++;
        printf("o");
        fflush(stdout);
        sleep(1);
    }

    if(pthread_join (mythread, NULL)) {
        printf("error joining thread.");
        abort();
    }
    printf("\nmyglobal equals %d\n", myglobal);
    exit(0);
}