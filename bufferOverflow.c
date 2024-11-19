#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

int main(int argc, char *argv[])
{
    int valid = FALSE;
    char str1[8];
    char str2[8];
    printf("enter a string 1: ");
    gets(str1);
    printf("enter a string 2: ");
    gets(str2);
    if (strncmp(str1, str2, 0) == 0)
    {
        valid = TRUE;
    }
    printf("buffer1: str1(%s), str2(%s), valid(%d) \n", str1, str2, valid);
}