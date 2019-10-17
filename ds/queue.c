// queue operations in C

#include<stdio.h>
#include<stdlib.h>

int insert();
int delete();
int traverse();

int queue[5];
int front=0;
int rear=0;

int main()
{	
	int ch;
	while(1)
	{

		printf("what operation do you want to perform?\n------------------------------------------\n\n");
		printf("1. insert\n");
		printf("2. delete\n");
		printf("3. traverse\n");

		scanf("%d",&ch);

		switch(ch)
		{

			case 1 : insert();
				 break;

			case 2 : delete();
				 break;

			case 3 : traverse();
				 break;

			default : printf("invalid input\n");
		}
	}
	
}

int insert()
{
	if(rear==5)
		printf("queue is full");
	else
	{
		int element;
		printf("enter element to be added: ");
		scanf("%d",&element);
		queue[rear]=element;
		rear++;
	}
}

int delete()

{
	if(rear==front)
		printf("queue is empty\n");
	else
	{
		printf("deleted : %d\n",queue[front]);
		for(int i=0;i<rear;i++)
			queue[i]=queue[i+1];
	}
	rear--;
}

int traverse()
{
	if(front==rear)
		printf("queue is empty\n");
	else
	{
		printf("queue elements are: \n\n");
		for(int i=0;i<rear;i++)
			printf("%d\n",queue[i]);
	}
}





























