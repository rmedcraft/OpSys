#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

#include <fcntl.h>

struct threadArgs
{
    FILE *fptr;
    int numThreads;
    int numLines;
    int threadID;
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
        lseek(args->fptr, (args->numLines / args->numThreads) * args->threadID, SEEK_SET); //(fptr, (size of file / number of threads) * threadID, SEEK_SET)

        // Read the content and print it
        int ct = 0;
        while (fgets(javaLine, 200, args->fptr))
        {
            ct++;
            for (int i = 0; i < 200; i++)
            {
                if (javaLine[i] == NULL)
                {
                    break;
                }
                if (javaLine[i] == ' ')
                {
                    numWords++;
                }
            }
            printf("%s\nNumber of words: %d\n", javaLine, numWords);
            totalWords += numWords;
            numWords = 0;
            if (ct >= args->numLines / args->numThreads)
            {
                break;
            }
        }
    }
    // printf("Total Words: %d", totalWords);
    // closes the file
    // fclose(fptr);
}

int main()
{
    clock_t start, end;
    double execution_time;

    start = clock();

    /*main program*/

    // using lseek: pass
    FILE *fptr;
    fptr = fopen("Java.txt", "r"); // opens java.txt on readonly mode

    pthread_t thread;
    int numThreads = 8;
    const int numLines = 17005;

    struct threadArgs args;
    args.fptr = fptr;
    args.numThreads = numThreads;
    args.numLines = numLines;

    for (int i = 0; i < numThreads; i++)
    {
        args.threadID = i;
        pthread_create(&thread, NULL, threadProgram, &args);
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(&thread, NULL);
    }

    end = clock();
    execution_time = ((double)(end - start));
    printf("Execution Time: %f", execution_time);
}