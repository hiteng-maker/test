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

#define WR_VALUE _IOW('a','a',struct data*)
#define RD_VALUE _IOR('a','b',struct data*)


struct data
{
	char name[10];
	int age;
};

struct data p1,p2;
 
int main()
{
        int fd;
 
        fd = open("/dev/my_device", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
 
        printf("------Enter data of p1------\n");
	printf("enter name : ");
	scanf("%s",p1.name);

	printf("enter age: ");
	scanf("%d",&p1.age);

	printf("------Enter data of p2------\n");
	printf("enter name : ");
	scanf("%s",p2.name);

	printf("enter age: ");
	scanf("%d",&p2.age);
	

        printf("Writing Value to Driver\n");

	// ioctl(file descriptore, read/write command, argument);

        ioctl(fd, WR_VALUE, (struct data) p1);
 //       ioctl(fd, WR_VALUE, (struct data) p2);
	

/*        
	printf("Reading Value from Driver\n");
        
	ioctl(fd, RD_VALUE, (int*) value);
	for(i=0 ; i<10 ; i++)
	{
		printf("Value from driver is: value[%d]=%d\n",i+1,value[i]);
	}
*/
        close(fd);
}
