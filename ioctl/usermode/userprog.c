#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdio.h>
#include "ioctlex.h"
#include <fcntl.h>
int main()
{
    int fd = open("/dev/ioctlex",O_RDWR);
    if(fd < 0)
    {
        printf("Cannot open file ..\n");
        return 0;
    }
    char buf[100];
    char *bufPtr = buf;
    printf("Enter Message to send to kernel \n");
    scanf("%s",buf);
    ioctl(fd, CMD_SEND_MSG,(int32_t *)bufPtr);
    

    char readbuf[100];
    printf("Trying to read the message back from kernel \n");
    ioctl(fd,CMD_RECV_MSG,(int32_t *)(&readbuf));
    printf("Read value from kernel is %s" ,readbuf);


    close(fd);
}
