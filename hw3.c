#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

#include <fcntl.h>

#define MAX_THREADS 128

struct threadArgs
{
    FILE *fptr;
    int numThreads;
    int numLines;
    int threadID;
    void *wordLock;
    void *fileLock;
};

int totalWords = 0;
void *threadProgram(void *threadArgs)
{
    struct threadArgs *args = (struct thread_args *)threadArgs;

    if (args->fptr == NULL)
    {
        printf("Not able to open the file: java.txt does not exist!");
    }
    else
    {

        // a place to store the contents of the file
        char javaLine[200];
        int numWords = 0;

        // lseek to the right place in the file
        // divides the lines of the file into numThreads parts, then moves the thread to the threadID's partition
        pthread_mutex_lock(args->fileLock);
        lseek(args->fptr, (args->numLines / args->numThreads) * args->threadID, SEEK_SET); //(fptr, (size of file / number of threads) * threadID, SEEK_SET)
        pthread_mutex_unlock(args->fileLock);

        // Read the content and print it
        int ct = 0;
        while (fgets(javaLine, 200, args->fptr))
        {
            ct++;
            for (int i = 0; i < 200; i++)
            {
                if (javaLine[i] == '\0')
                {
                    break;
                }
                if (javaLine[i] == ' ')
                {
                    numWords++;
                }
            }

            pthread_mutex_lock(args->wordLock);
            totalWords += numWords;
            pthread_mutex_unlock(args->wordLock);
            numWords = 0;
            if (ct >= args->numLines / args->numThreads)
            {
                break;
            }
        }
    }
}

int main()
{
    clock_t start, end;
    double execution_time;

    /*main program*/
    FILE *fptr;
    fptr = fopen("Java.txt", "r"); // opens java.txt on readonly mode

    pthread_mutex_t wordsLock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t fileLock = PTHREAD_MUTEX_INITIALIZER;

    pthread_t thread[MAX_THREADS];
    int threadCounts[] = {8, 32, 64, 128};

    for (int j = 0; j < 4; j++)
    {
        start = clock();
        int numThreads = threadCounts[j];

        fseek(fptr, 0, SEEK_END);
        const int numLines = ftell(fptr); // 17005;
        fseek(fptr, 0, SEEK_SET);

        struct threadArgs args;
        args.fileLock = &fileLock;
        args.wordLock = &wordsLock;
        args.fptr = fptr;
        args.numThreads = numThreads;
        args.numLines = numLines;

        for (int i = 0; i < numThreads; i++)
        {
            args.threadID = i;
            pthread_create(&thread[i], NULL, threadProgram, &args);
        }

        for (int i = 0; i < numThreads; i++)
        {
            pthread_join(thread[i], NULL);
        }

        end = clock();
        printf("Threads: %d\n", numThreads);
        printf("Total Words: %d\n", totalWords);
        execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Execution Time: %fs\n", execution_time);
        totalWords = 0;
    }

    fclose(fptr);
    return 0;
}