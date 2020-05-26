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

struct data user_data;
 
int main()
{
        int fd;
 
        fd = open("/dev/my_device", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
 
        printf("------Enter user data------\n");
	printf("enter name : ");
	scanf("%s",user_data.name);

	printf("enter age: ");
	scanf("%d",&user_data.age);
	

        printf("Writing Value to Driver\n");

	// ioctl(file descriptore, read/write command, argument);

        ioctl(fd, WR_VALUE, (int32_t *) &user_data);
	


	printf("Reading Value from Driver\n");
        
	ioctl(fd, RD_VALUE, (int32_t *) &user_data);
	
	printf("----------Data Recieved from Kernel------------ \nname: %s age: %d\n",user_data.name,user_data.age);



        close(fd);
}
