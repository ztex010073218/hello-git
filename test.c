//it is a tset c file for git 
#include <stdio.h>

int gcd(int a,int b)
{
	if(b==0)
	{
		return a;
	}
	else
	{
		return(b,a%b);
	}
}

int main(void)
{
	int num1,num2;
	printf("hello it is gcd function, input two numbers,please\n");
	scanf("%d %d",&num1,&num2);
	printf("%d\n",gcd(num1,num2));
	return 0;
}
