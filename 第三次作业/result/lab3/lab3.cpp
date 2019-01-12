// lab3.cpp : Defines the entry point for the console application.
//

// lab3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdlib.h>
#include<string.h>

//output both in console and file
int OutImport(FILE *in,FILE *out);
int atoh(unsigned char hexstr);

int main(int argc,char **argv)
{  
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
 
    //attention: open in binary
	FILE *in = fopen(argv[1],"rb");
	FILE *out = fopen(argv[2],"w+");


    if(in == NULL || out == NULL)
	{
		fprintf(stderr,"%s\n","open file error!");
        exit(1);
	}
 
    OutImport(in,out);
    fclose(in);
    fclose(out);


	return 0;
}


int OutImport(FILE *in,FILE *out){
	int F0,F1,F2,F3;
	//R means rva, F means fileoffset
	int ImportR,SectionR;//Import
	int ImportL,SectionL;//Sectoin
	int section,FileOff,ImportF;
	int head,DLLPF,DLLPR,APITR ,APITF,APIPR,APIPF;
	//actually, a int can save the address
	
	unsigned char hex[4],tmp[3];
	tmp[1]=10;
	tmp[2]=':';
	//first need to locate 3C get the PE position we need 2 bytes
	fseek(in,0L,0);
    fseek(in,60L,0);
	fread(&hex[0],1,1,in); F0 = hex[0];
	fread(&hex[1],1,1,in); F1 = hex[1];
	fread(&hex[2],1,1,in); F2 = hex[2];
	fread(&hex[3],1,1,in); F3 = hex[3];
	head = F0 + F1 *16 *16 + F2 *16 *16 *16 *16 + F3 *16 *16 *16 *16 *16 *16; 

	fseek(in,128L+(long)head,0);
	fread(&hex[0],1,1,in); F0 = hex[0];
	fread(&hex[1],1,1,in); F1 = hex[1];
	fread(&hex[2],1,1,in); F2 = hex[2];
	fread(&hex[3],1,1,in); F3 = hex[3];
	ImportR = F0 + F1 *16 *16 + F2 *16 *16 *16 *16 + F3 *16 *16 *16 *16 *16 *16; 

	fread(&hex[0],1,1,in); F0 = hex[0];
	fread(&hex[1],1,1,in); F1 = hex[1];
	fread(&hex[2],1,1,in); F2 = hex[2];
	fread(&hex[3],1,1,in); F3 = hex[3];
	ImportL = F0 + F1 *16 *16 + F2 *16 *16 *16 *16 + F3 *16 *16 *16 *16 *16 *16; 

	//next we need to find the import table is in which section
	//locate the describtion for every section
	section = 0;
	long count=0;
	while (section <= ImportR){
	fseek(in,248L+head+count * 40 + 8,0);
	fread(&hex[0],1,1,in); F0 = hex[0];
	fread(&hex[1],1,1,in); F1 = hex[1];
	fread(&hex[2],1,1,in); F2 = hex[2];
	fread(&hex[3],1,1,in); F3 = hex[3];
	SectionL = F0 + F1 *16 *16 + F2 *16 *16 *16 *16 + F3 *16 *16 *16 *16 *16 *16;
	fread(&hex[0],1,1,in); F0 = hex[0];
	fread(&hex[1],1,1,in); F1 = hex[1];
	fread(&hex[2],1,1,in); F2 = hex[2];
	fread(&hex[3],1,1,in); F3 = hex[3];
	SectionR = F0 + F1 *16 *16 + F2 *16 *16 *16 *16 + F3 *16 *16 *16 *16 *16 *16; 
	section = SectionR+SectionL;
    count++;
	}
	fseek(in,4L,1);
	fread(&hex[0],1,1,in); F0 = hex[0];
	fread(&hex[1],1,1,in); F1 = hex[1];
	fread(&hex[2],1,1,in); F2 = hex[2];
	fread(&hex[3],1,1,in); F3 = hex[3];
	FileOff = F0 + F1 *16 *16 + F2 *16 *16 *16 *16 + F3 *16 *16 *16 *16 *16 *16; 
	ImportF= ImportR - SectionR + FileOff;
    
	//got the address of import table in the file
	// now we need to read the import table events by events   
	count = 0;
	APITR = 1;
	while(APITR!=0){
	fseek(in,(long)ImportF+20*count,0);
	fread(&hex[0],1,1,in); F0 = hex[0];
	fread(&hex[1],1,1,in); F1 = hex[1];
	fread(&hex[2],1,1,in); F2 = hex[2];
	fread(&hex[3],1,1,in); F3 = hex[3];
	APITR = F0 + F1 *16 *16 + F2 *16 *16 *16 *16 + F3 *16 *16 *16 *16 *16 *16; 
	APITF = APITR - SectionR + FileOff;
	fseek(in,(long)ImportF+20*count+12,0);
	
	//output dllNAME
	fread(&hex[0],1,1,in); F0 = hex[0];
	fread(&hex[1],1,1,in); F1 = hex[1];
	fread(&hex[2],1,1,in); F2 = hex[2];
	fread(&hex[3],1,1,in); F3 = hex[3];
	DLLPR = F0 + F1 *16 *16 + F2 *16 *16 *16 *16 + F3 *16 *16 *16 *16 *16 *16;
	DLLPF = DLLPR - SectionR + FileOff;
	fseek(in,(long)DLLPF,0);
	fread(&tmp[0],1,1,in);
	while(tmp[0]!=0 && APITR!=0){
	printf("%c",tmp[0]);
	fprintf(out,"%c",tmp[0]);
	fread(&tmp[0],1,1,in);
	}
	if (APITR!=0) {printf(":\n");
	fprintf(out,"%c",tmp[2]);
	fprintf(out,"\n");
	}
	count++;
	//then we need to output the name of API
	fseek(in,(long)APITF,0);
	//printf("myapitf= %x\n",APITF);
	int count2=1;
	fread(&hex[0],1,1,in); F0 = hex[0];
	fread(&hex[1],1,1,in); F1 = hex[1];
	fread(&hex[2],1,1,in); F2 = hex[2];
	fread(&hex[3],1,1,in); F3 = hex[3];
	APIPR = F0 + F1 *16 *16 + F2 *16 *16 *16 *16 + F3 *16 *16 *16 *16 *16 *16;
	APIPF = APIPR - SectionR + FileOff +2;
	//printf("myapipf= %x\n",APIPF);
    while (APIPR != 0){
		if(F3<128){
			
			fseek(in,(long)APIPF,0);
			fread(&tmp[0],1,1,in);	
			while(tmp[0]!=0 && APITR!=0){
				printf("%c",tmp[0]);
				fprintf(out,"%c",tmp[0]);
				fread(&tmp[0],1,1,in);
			}
			if (APITR!=0) {printf("\n");
			fprintf(out,"\n");
			}
		}
		
		else {printf("00%02x%02x%02x\n",hex[2],hex[1],hex[0]);
		fprintf(out,"00%02x%02x%02x\n",hex[2],hex[1],hex[0]);
		}
		fseek(in,(long)APITF+4*count2,0);
		fread(&hex[0],1,1,in); F0 = hex[0];
		fread(&hex[1],1,1,in); F1 = hex[1];
		fread(&hex[2],1,1,in); F2 = hex[2];
		fread(&hex[3],1,1,in); F3 = hex[3];
		APIPR = F0 + F1 *16 *16 + F2 *16 *16 *16 *16 + F3 *16 *16 *16 *16 *16 *16;
		if (APIPR==0) break;
		APIPF = APIPR - SectionR + FileOff +2;
		count2++;
	}

	
	
	}
	//printf ("RVA = %x",ImportR);
	//printf ("%c,%c,%c,%c",hex[0],hex[1],hex[2],hex[3]);
	//printf ("%x,%x,%x,%x",F0,F1,F2,F3);
	//fprintf(out,"%c",ImportR); 
	//printf ("A = %x   %x ",ImportF);
	return 0;
}
 

 
 
