/* we will use this program to perform IOCTL with device file in order to read/write ops between kernel and user */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  // to open the device file
#include <unistd.h> // to close the device file
#include<sys/ioctl.h>


// this is the same definition that we have defined in kernel space,

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)
 
int main()
{
        int fd;
        int32_t value, number;
 
        fd = open("/dev/my_device", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
 
        printf("Enter Value to send to kernel\n");
        scanf("%d",&number);
        printf("Writing Value to Driver\n");

	// ioctl(file descriptore, read/write command, argument);

        ioctl(fd, WR_VALUE, (int32_t*) &number); 
 
        
	printf("Reading Value from Driver\n");
        
	ioctl(fd, RD_VALUE, (int32_t*) &value);
	printf("Value is %d\n", value);

        close(fd);
}
