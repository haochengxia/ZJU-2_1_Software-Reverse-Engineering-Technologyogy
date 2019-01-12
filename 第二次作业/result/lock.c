// lock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<stdlib.h>
#include<string.h>
 

int encrypt(FILE *in,FILE *out,FILE *tmp);
unsigned char atoh(char *hexstr);
void CreateShelldat(FILE *in,FILE *inb,FILE *outb,FILE *tmp);
int counts;
int main(int argc,char **argv)
{  int ret;
	if(argc > 1 && strcmp(argv[2],"--help") == 0)
	{
		printf("\nusage: linkrules_god  inputfile outputfile\n");
		printf("     encrypt inputfile to outputfile.\n");
		exit(0);
	}
	if( argc !=3)
	{
		fprintf(stderr,"%s\n","please using --help go get help.");
		exit(-1);
	}
 
    
	FILE *in = fopen(argv[1],"rb");
	FILE *out = fopen(argv[2],"wb");
	FILE *inb = fopen("shell.bin","rb");
	FILE *outb = fopen("shelldat.bin","wb");
	FILE *tmp = fopen("tmp.txt","wb");
    if(in == NULL || out == NULL)
	{
		fprintf(stderr,"%s\n","open file error!");
        exit(1);
	}
 
   
    CreateShelldat(in,inb,outb,tmp);    
	fclose(inb);
    fclose(outb);
	fclose(tmp);
	tmp = fopen("tmp.txt","rb");
    encrypt(in,out,tmp);
    fclose(in);
    fclose(out);

    fclose(tmp);
    ret = remove("tmp.txt");
	return 0;
}
 
int encrypt(FILE *in,FILE *out,FILE *tmp)
{	int count; //the location of the byte
	int flag,f; //flag used to judge if this byte need to be crypted 
	//initialize 
	int ip,cs;
	int fixL,fixH,secL,secH,segL,segH;
	FILE *save;
	save=out; 
	count = flag = 0;
    if(in == NULL || out == NULL)
    {
        fprintf(stderr,"%s\n","file error!\n");
        return -1;
    }
    fixH=counts%(2*16*16)/(16*16);
	fixL=counts%(2*16*16)-fixH*(16*16);
	secH=(counts/(2*16*16))/(16*16);
	secL=(counts/(2*16*16))-secH*(16*16)+1;  
 	
    unsigned char hex;
    unsigned char t;
    fseek(tmp,0L,0);
    fseek(in,0L,0);
	 
    while(fread(&hex,1,1,in)&&fread(&hex,1,1,tmp))
    {	count++;
   		t = hex;
     	/*if (count==3)  sfixL=t;
    	else if (count==4) sfixH=t;
    		else if (count==5) ssecL=t;
    			else if (count==6) ssecH=t;
    				else*/ if (count==9) segL=t;
    					else if (count==10) segH=t;
    	
    	//printf("%x %x %x %x %x %x",fixL,fixH,secL,secH,segL,segH) ;
        hex = hex^0x33;
        if(flag) fprintf(out,"%c",hex);
     	else fprintf(out,"%c",t);
		if (count>=10&&count==(segH*16*16+segL)*16)  flag=1;
    }
    while(fread(&hex,1,1,tmp)){
    fprintf(out,"%c",hex); 
	}
    out=save;

	 
	 
	 
	 //the part to change the specific items
 	t= fixL;
 
	 fseek(out, 2L, 0);//move to the position +02
 	fprintf(out,"%c",t);
	fseek(out, 3L, 0);//move to the position +03
	t= fixH;
 	
 	fprintf(out,"%c",t);//fwrite(target,1,1,out);
	fseek(out, 4L, 0);//move to the position +04
 	t= secL;
 
 	fprintf(out,"%c",t);//fwrite(target,1,1,out);
	fseek(out, 5L, 0);//move to the position +05
 	t= secH;

 	fprintf(out,"%c",t);//fwrite(target,1,1,out);
	fseek(out, 6L, 0);//change Relocation number to 0 
	t=0;

	fprintf(out,"%c",t);//fwrite(target,1,1,out);
	fseek(out, 7L, 0);	

    fprintf(out,"%c",t);//	fwrite(target,1,1,out);
//	printf("%x %x %x %x %x %x",fixL,fixH,secL,secH,segL,segH) ;
	
	//change hello2.exe 's cs:ip;
	fseek(in,2L,0);
	f=fread(&hex,1,1,in);
	ip=hex;
	fseek(out,20L,0);
	fprintf(out,"%c",ip);
//	printf("ip = %x",ip);
	f=fread(&hex,1,1,in);
	ip=hex;
	fprintf(out,"%c",ip);
	fseek(in,4L,0);
	f=fread(&hex,1,1,in);
	cs = 2*16*(hex-1);
	f=fread(&hex,1,1,in);
	cs = cs + 16*16*2*16*hex;
	fseek(in,8L,0);
	f=fread(&hex,1,1,in);
	cs = cs - hex;
	f=fread(&hex,1,1,in);
	cs = cs - hex*16*16;
//	printf("cs = %x",cs);
	fseek(out,22L,0);
	fprintf(out,"%c",cs);
	fprintf(out,"%c",(cs/(16*16)));
    return 0;
}
 

void CreateShelldat(FILE *in,FILE *inb,FILE *outb,FILE *tmp){    //this function is used to read hello2.exe's file head and add to shell.bin to generate shelldat.bin
	//first read the length of the file head
	int segL,segH,seg,i,f;
	unsigned char hex;	

	counts=0;
	while(fread(&hex,1,1,in))
    {counts++;
	 fprintf(tmp,"%c",hex);
	 
	}
	
	fseek(in, 8L, 0);
	f=fread(&hex,1,1,in);
	segL=hex;
	fseek(in, 9L, 0);
	f=fread(&hex,1,1,in);
	segH=hex;
	seg=segH*16*16+segL;
//	printf("seg= %x",seg);
	 while(fread(&hex,1,1,inb))
	 {counts++;
	 	fprintf(outb,"%c",hex);
	 	fprintf(tmp,"%c",hex); 
	 }	
	fseek(in, 0L, 0);
	for (i=0;i<seg*16;i++)
	{ counts++;
	 f=fread(&hex,1,1,in);
	 fprintf(outb,"%c",hex);
	
	 fprintf(tmp,"%c",hex);	 
	}
	
}




