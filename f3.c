/* the final process to read the modified data from 2nd file */


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
		if((*f).id == 3)
		{
			// the final modified data will be read from here.

			printf("\nthe length of data: %ld and message : %s\n",strlen((*f).message),(*f).message);
		
			(*f).id=1; // again 1st process will ask to enter data at runtime.
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
        
        // destroy the shared memory 
      //   shmctl(shmid,IPC_RMID,NULL); 
        
        return 0; 
} 


