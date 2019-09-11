
#include<stdio.h>
void check(int num);
main()
{
	int n;
	printf("enter a number\n");
	scanf("%d",&n);
	check(n);

}

void check(int num)
{
	int sum_e=0,sum_o=0,count=0,rem,temp;
	temp=num;
	while(num>0)
	{
		rem=num%10;
		count++;
		if(count%2==0)
			sum_e=sum_e+rem;
		else
			sum_o=sum_o+rem;
	
		num=num/10;
	}
	if(sum_e==sum_o)
		printf("%d is divisle by 11\n",temp);
	else
		printf("%d is not divisible by 11\n",temp);
}
