#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void threadProgram() {
 /*main program*/
}

int main()
{
    clock_t start, end;
    double execution_time;

    start = clock();

    /*main program*/
    threadProgram();
    
    end = clock();
    execution_time = ((double)(end - start));
}