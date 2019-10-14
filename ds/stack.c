#include<stdio.h>
#include<stdlib.h>
#define MAX 5

int stack[MAX];
int ele;
int top=-1;

int isfull()
{
	if(top==4)
		return 1;
	else
		return 0;
}

int isempty()
{
	if(top==-1)
		return 1;
	else
		return 0;
}
void push(int ele)

{	
	if(isfull())
		printf("stack is full\n");
	else
	{
		top++;
		stack[top]=ele;
		printf("\n%d is inserted\n",ele);
	}
}
int pop()
{
	if(isempty())
		printf("stack is empty\n");
	else
	
		return stack[top--];
}
void peek()
{
	if(isempty())
		printf("stack is empty\n");
	else
		printf("peek is element is %d \n",stack[top]);
}
void traverse()
{
	if(isempty())
		printf("stack is empty\n");
	else
	{
		int i;
		printf("stack elements are : \n");
		for(i=0;i<=top;i++)
			printf("\n%d\t",stack[i]);	
	}
}

void display()
{
        if(isempty())
                printf("stack is empty\n");
        else
        {
                int i;
                printf("stack elements are : \n");
                for(i=0;i<5;i++)
                        printf("\n%d\t\n",stack[i]);        
        }
}
int main()
{
	printf("elements:\n");
	push(1);
	push(2);
	push(3);
	push(4);
	push(5);
	push(6);
	push(7);
	pop();
	pop();
	push(10);
	traverse();
	pop();
	push(20);

	traverse();
	push(30);
	display();
	return 0;
}
