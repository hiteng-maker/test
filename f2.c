/* in this 2nd process program will read data from 1st file and modify the existing buffer which will be read form 3rd process later. */

#include <stdlib.h>
#include <sys/shm.h> 
#include <stdio.h> 
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include<semaphore.h>
#include<fcntl.h>




struct shmseg{
	int id;
	char message[256];
};

void * func_write(void * args)
{
	
	struct shmseg *f = (struct shmseg *)args;
	while(1)
	{
		if((*f).id == 2)
		{
		
		printf("\ndata is : %s",(*f).message);
		
		printf("\nenter new data: ");
		scanf("%s",(*f).message);
		
		(*f).id=3; // to switch in 3rd process.
		
		}
		
	}
}


int main() 
{ 
	struct shmseg* buffer;
	pthread_t tid;

	
	// ftok to generate unique key 
	key_t key = ftok("shmfile",65); 

	// shmget returns an identifier in shmid 
	int shmid = shmget(key,sizeof(struct shmseg),0666|IPC_CREAT); 

	// shmat to attach to shared memory 
	buffer = (struct shmseg *) shmat(shmid,NULL,0);
	
	
	pthread_create(&tid,NULL,func_write,(void *)buffer);

	pthread_join(tid,NULL);
	
	
	
	

	//detach from shared memory 
	shmdt(buffer); 
	
	
	return 0; 
} 

