// reverse01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void initialize(int *key);
int main(int argc, char* argv[])
{	int a[8];
	int key[8];
	char s[8];
	int i,tmp,c1,c2;
	int flag=0;
	initialize(key);
	c1=0;
	c2=0;//reset
	printf("please enter your MachineCode:");
	while (flag==0)	{
		scanf("%s",s);
		for (i=0;i<8;i++) {
			flag=1;
			if ('0'<s[i]+1&&s[i]-1<'9') a[i]=s[i]-48;
				else if('A'<s[i]+1&&s[i]-1<'Z')  a[i]=s[i]-55;
					else {
						flag=0;
						printf("Wrong Input!!!\n");
						printf("please enter your MachineCode again:");
						break;
					}
			key[0]=1;//for solve the error
			//printf("%d %d\n",a[i],key[i]);
			a[i]=a[i]^key[i];
		
			}
	}
	//exchange the number of second digit and the number of third digit
	tmp=a[1];
	a[1]=a[2];
	a[2]=tmp;
	//process the seventh digit and the eighth digit
	if(a[7]%2==1) c2=1;//judge number is even or odd
	if(a[6]%2==1) c1=1;
	if(c1==1) a[7]=a[7]/2+8;
	else a[7]=a[7]/2;
	if(c2==1) a[6]=a[6]/2+8;
	else a[6]=a[6]/2;
	//process the fifth digit and the sixth digit
	c1=a[4]/4;
	a[4]=a[4]%4*4+a[5]/4;
	a[5]=a[5]%4*4+c1;

	printf("That is your register code: ");
	for (i=0;i<8;i++){
		printf("%X",a[i]);
	}
	while(1){};//for stop to copy the code
	return 0;
}

void initialize(int *key){
	key[0]=1;
	key[1]=3;
	key[2]=5;
	key[3]=7;
	key[4]=2;
	key[5]=4;
	key[6]=6;
	key[7]=8;

}