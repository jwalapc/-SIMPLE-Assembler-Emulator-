/*Author: Jwala Chorasiya   	Roll: 08 (TCET MUMBAI)
I hereby declare that I, Jwala Chorasiya from TCET MUMBAI, have authored this source code independently
that I have not used other than the declared sources/resources, and that I have explicitly marked
all material which has been quoted either literally or by content from the used sources.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mnemonics.h"

#define OFFSET 50
#define TRACE if(trace)printf("PC=%08X, SP=%08X, A=%08X, B=%08X",PC,SP,A,B)

int main(int argc,char* argv[]){
	PC = SP = A = B = 0;
	if(argc < 3){
		printf("Pass the object file and Use flags--\n");
		printf("-t trace registers each instruction\n");
		printf("-b memory dump before execution\n");
		printf("-a memory dump after execution\n");
		return 0;
	}
	if(argc != 3){
		printf("Use format= ./emu -[flag(s)] [object-file]\n Eg: ./emu -ab test.o for before and after dump\n");
		return 0;
	}
	int fname_len = strlen(argv[2]);
	if(fname_len <= 2 || strcmp(argv[2]+fname_len-2,".o")){
		printf("Pass only .o object files\n");
		return 0;
	}
	int trace = 0,before = 0,after = 0;
	int i;
	for(i=1;i<(int)strlen(argv[1]);++i){
		if('t' == argv[1][i])
			trace = 1;
		if('b' == argv[1][i])
			before = 1;
		if('a' == argv[1][i])
			after = 1;
	}
	int sizes[2];
	FILE *fptr = fopen(argv[2],"rb");

	fread(sizes,sizeof(int),2,fptr);

	int* Code = (int*)malloc(sizes[0]*sizeof(int));
	memory = (int*)malloc((sizes[1]+OFFSET)*sizeof(int));

	fread(Code,sizeof(int),sizes[0],fptr);
	fread(memory,sizeof(int),sizes[1],fptr);

	fclose(fptr);

	if(before){
		printf("\nData Segment before execution: ");
		for(i=0;i<sizes[1]+OFFSET;++i){
			if(i%4==0)
				printf("\n%08X",i);
			printf(" %08X",memory[i]);
		}
		printf("\n");
	}
	printf("\n");
	int counter = 0;
	for(;PC < sizes[0];++PC,++counter){
		TRACE;
		int op = (0xFF & Code[PC]);
		if(trace)printf(" %s",mnemonics[op]);
		
		int val = Code[PC]>>8;
		if((1<<31)&Code[PC])
			val = (0xFF<<24)|val;

		if(trace){
			if(op < 6 || (op == 10 || op == 13) || (op >= 15 && op <= 17))
				printf(" %08X\n",val);
			else
				printf("\n");
		}
		if(op == 18)
			break;
		(asm_func[op])(&val);
	}
	printf("\n%d instruction(s) executed\n",counter);

	if(after){
		printf("\nData Segment after execution: ");
		for(i=0;i<sizes[1]+OFFSET;++i){
			if(i%4==0)
				printf("\n%08X",i);
			printf(" %08X",memory[i]);
		}
		printf("\n");
	}
}
