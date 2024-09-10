#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
int main()
{
    char buffer[19];
    int file = 0;
    if ((file = open("opsys-1.tst", O_RDONLY)) < -1)
        return 1;
    if (read(file, buffer, 19) != 19)
        return 1;
    printf("%s\n", buffer);
    if (lseek(file, 10, SEEK_SET) < 0)
        return 1;
    if (read(file, buffer, 19) != 19)
        return 1;
    printf("%s\n", buffer);
    return 0;
}