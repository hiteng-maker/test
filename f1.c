/* This is c application to perfrom synchronization among 3 different processes.
 * Steps are very simple as mentioned below
 * 1. compile all the 3 files and create executable files for all with -lpthread in 3 different terminals as we will monitor runtime outputs for all.
 * 2. enter data in 1st file which will be read from 2nd file. (the data will be stored in shared memory)
 * 3. read that data from buffer from 2nd file and modify the buffer in 2nd file.
 * 4. the modified data will be reflected in 3rd file.
 */




#include <stdlib.h>
#include <sys/shm.h> // used while playing with shared memomry 
#include <stdio.h> 
#include <unistd.h>
#include <pthread.h> // when we perform anything to threads
#include <string.h>
#include<semaphore.h>
#include<fcntl.h>


// the below structure will be responsible for data passing among processes

struct shmseg{
	int id;
	char message[256];
};

void * func_write(void * args)
{
	struct shmseg *f = (struct shmseg *)args;
	while(1)
	{	
		if((*f).id == 1)
		{
			printf("\nenter data: ");
			scanf("%s",(*f).message);
		
			(*f).id=2;	// after this statement the flow of program will jump to 2nd process.
		}
		
	}
}


int main() 
{ 
	struct shmseg* buffer; // pointer to the structure which is responsible for data passing
	pthread_t tid;

	

	// this will generate unique key for shmfile file and id 65.
	key_t key = ftok("shmfile",65); 

	// shmget returns an identifier in shmid 
	int shmid = shmget(key,sizeof(struct shmseg),0666|IPC_CREAT); 

	// shmat to attach to shared memory 
	buffer = (struct shmseg *) shmat(shmid,NULL,0);

	// the below statement is responsible for passing the data according to id values. aka, sync processes.
	(*buffer).id = 1;
	
	// The API create new thread in shared memory.
	pthread_create(&tid,NULL,func_write,(void *)buffer);
	
	// join with terminated thread. ie, it will wait for tid thread.
	pthread_join(tid,NULL);

	
	
	//detach from shared memory 
	shmdt(buffer); 

	return 0; 
} 

