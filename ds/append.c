// single linkd list operations



#include<stdio.h>
#include<stdlib.h>
struct node
{
	int data;
	struct node *link;
};

struct node * root=NULL;
int len;
int length();
void display();
int append();
int delete();
int main()
{
	int ch;
	while(1)
	{
		printf("single linked list operations: \n------------------------------------\n");
		printf("1. append\n");
		printf("2. length\n");
		printf("3. display\n");
		printf("4. delete\n");
		printf("5. quit\n");

		printf("enter your choice: ");
		scanf("%d",&ch);

		switch(ch)
		{
			case 1 : append();
				 break;

			case 2 : len= length();
				 printf("length is %d\n\n",len);
				 break;

			case 3 : display();
				 break;

			case 4 : delete();
				 break;

			case 5 : exit(1);

			default : printf("invalid input\n\n");
		}
	}
}


int append()
{
	struct node *tmp;
	tmp=(struct node*)malloc( sizeof(struct node));

	printf("enter node data:");
	scanf("%d",&tmp->data);

	tmp->link=NULL;

	if(root==NULL)
	{
		root=tmp;
	}
	else
	{
		struct node *p;
		p=root;

		while(p->link!=NULL)
		{
			p=p->link;
		}

		p->link=tmp;
	}
}

int length()
{
	int count=0;
	struct node* tmp;
	tmp=root;

	while(tmp!=NULL)
	{
		count++;
		tmp=tmp->link;
	}
	return count;
}

void display()
{
	struct node* tmp;
	tmp=root;
	if(tmp==NULL)
		printf("\nlist is empty\n\n");
	else
	{

		while(tmp!=NULL)
		{
			printf("%d-->",tmp->data);
			tmp=tmp->link;
		}
		printf("\n\n");
	}
}

int delete()
{
	struct node*tmp;
	int location;
	printf("enter location of node\n");
	scanf("%d",&location);
	
	if(location>length())
		printf("invalid location\n");
	else if(location==1)
	{
		tmp=root;
		root=tmp->link;
		tmp->link=NULL;
		free(tmp);
	}

	else
	{
		struct node*p,*q;
		p=root;
		int i=1;
		while(i<location-1)
		{
			p=p->link;
			i++;
		}
		q=p->link;
		p->link=q->link;
		q->link=NULL;
		free(q);
	}
}






























