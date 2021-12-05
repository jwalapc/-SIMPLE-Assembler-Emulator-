/*	
/*Author: Jwala Chorasiya   	Roll: 08 (TCET MUMBAI)
I hereby declare that I, Jwala Chorasiya from TCET MUMBAI, have authored this source code independently
that I have not used other than the declared sources/resources, and that I have explicitly marked
all material which has been quoted either literally or by content from the used sources.*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h" 		/*Contains the implementations of the linked lists used here*/
#define ERR_FILE "Error: Incorrect file type"

int data_size = 0;		/*Size of the data segment*/

char mnemonics[][7] = {"ldc","adc","ldl","stl","ldnl","stnl","add","sub","shl","shr","adj","a2sp","sp2a","call","return","brz","brlz","br","HALT"};

int *DataSeg; /*Store data in an array*/

/*Function to determine if a word is which mnemonic or return -1 */
int whichMnemonic(char *nm){
	int i = 0;
	for(;i<19;++i){
		if(!strcmp(nm,mnemonics[i]))
			return i;
	}
	return -1;
}

/*remove whitespaces from the end of the line*/
void rStrip(char* line){
	int len = strlen(line);
	len--;
	while(line[len]==' ' || line[len]=='\n')
		line[len--] = '\0';
}

int execute_first_pass(char* filename){
	int flen = strlen(filename);
	strcat(filename,".asm"); 	/*Open .asm file using fptr*/

	FILE *fptr = fopen(filename,"r");

	filename[flen] = '\0';
 	strcat(filename,".log");

	FILE *log = fopen(filename,"w");
	filename[flen] = '\0'; 		/*Opens .log file*/
	
	char line[200];				/*take a line from .asm file*/
	int line_num = 1;			/*Number of lines read*/
	int is_error = 0;  	 		/*If there is any error in the .asm syntax*/
	int PC = 0;  				/*Keep counter for the encountered instructions*/
	while(fgets(line,sizeof line,fptr) != NULL){
		rStrip(line);			/*Strip the whitespaces at the right for smooth line parsing*/
		char* parse = line; 	/*Pointer till where the line has been read*/
		char word[MAX_CHAR];	/*CString to store each token*/
		int cc; 	 			/*A variable to keep track of how many characters were read to take word[]*/
		int idx = 0;			/*Number of tokens encountered in a line*/
		int isLabel = 0; 		/*If the line contains a label*/
		int isData = 0;  		/*If the line is declaring a data*/
		int mnemonic = 0; 		/*the mnemonic used in the line*/
		char label[MAX_CHAR];
		while(parse[0] != '\0' && parse[0] != ';' && parse[0]!='\n'){
			sscanf(parse,"%s%n",word,&cc);   /*Take the word and the number of characters read*/
			int len = strlen(word);
			if(len == 0)
				break;
			if(word[0]==';') 				/*If a comment begins, break the loop to read next line*/
				break;
			if(word[len-1] == ';'){  		/*If the word is the last token*/
				cc--; 						/*decrement the characters read by 1*/
				word[--len] = '\0'; 		/*And decrease the word's length removing ; */
			}
			if(!idx){
				char* ptr = word; 			/*If it is the first token*/
				char* lbl = strtok_r(ptr,":",&ptr); /*Check if has label or not by finding : */
				int label_length = strlen(lbl);
				if(len != label_length){ 	/*If lengths are not equal then a : ie a label is encountered*/
					cc -= len; 				/* adjust the characters read, so that the next token is not ignored*/
					cc += label_length+1;   /* for eg: 'label:add', whole word will be read, using strtok_r(), label will be*/
											/* separated and cc will be adjusted so that the next time it points to add*/
					word[label_length]=':';
					word[label_length+1]='\0';

					len = label_length+1;
				}
			}
			if(word[len-1]==':'){ 		/*If it's a label then first char must be alphabet and must be the first token*/
				if(!idx && ((word[0]>='a' && word[0]<='z') || (word[0]>='A'&&word[0]<='Z'))){
					isLabel = 1; 		/*Mark the isLabel as True */
					word[len-1] = '\0';
					if(labelExists(word)==NULL && dataExists(word)==NULL)
						strcpy(label,word); /*Make a copy of the label*/
					else{
						is_error = 1;
						fprintf(log,"Error: line number %d: Reuse of same label\n",line_num);
					}
				}
				else{
					is_error = 1;
					fprintf(log, "Error: Wrong declaration of label at line %d\n",line_num);
				}
			}
			else if(idx-isLabel <= 1){ /*A valid syntax will have only 2 tokens or 3 tokens including label*/
				if(idx-isLabel == 0){  /*If it is the first token after label, check if data or mnemonic*/
					if(!strcmp("data",word)){
						data_size++;	/*Increase data_size and mark isData = True */
						isData = 1;
					}
					else{
						PC++; 					/*If mnemonic increase program counter and check which mnemonic*/
						mnemonic = whichMnemonic(word);
						if(mnemonic == -1){ 	/*If it is no mnemonic then it is an error*/
							is_error = 1;
							fprintf(log,"Error: Expected a valid mnemonic or 'data' keyword at line %d: %s\n",line_num,word);
						}
					}
				}     /*the token after mnemonic or data, but if the mnemonics are these, then there is an error*/
				else if(mnemonic < 0 || (mnemonic>=6 && mnemonic<=9) || (mnemonic==11 || mnemonic==12 || mnemonic==14 || mnemonic==18)){
						is_error = 1;
						fprintf(log,"Error: Unknown operand: %s at line %d. Use proper spacing and keywords\n",word,line_num);
				}
				else{ 							/*Else if data, add it to Data Labels*/
					if(isData){
						char* ptr = word; 		/*Take the word and take input using strtol*/
						int dt = strtol(ptr,&ptr,0); 
						if(ptr != word+len){ 	/*If whole word is consumed it is a valid number or else an error*/
							is_error = 1;
							fprintf(log,"Error: Expected a number at line: %d\n",line_num);
						}
						else if(isLabel){
							insertData(label,dt,data_size-1);
							isLabel = 0; 		/*Make isLabel False to avoid making it a common Label*/
							label[0] = '\0';
						}
					}
				}
			}
			else{  /*If more tokens are there then it is a syntactical error*/
				is_error = 1;
				fprintf(log,"Error: Unknown Syntax at line: %d\n",line_num);
			}
			++idx;
			parse += cc;
		}
		if(isLabel && label[0]!='\0') 		/*If isLabel then insert it into Labels' list */
			insertLabel(label,PC-(idx>1)); /*If instructions and labels are on the same line, Check the extra offset*/
		line_num++;
	}
	fclose(fptr);
	fclose(log);
	DataSeg = (int*)malloc(data_size*sizeof(int));  /*Allocate an array of data_size for data segment*/
	return !(is_error);						/*Return if error free or not*/
}

int execute_second_pass(char* filename){
	int flen = strlen(filename);
	strcat(filename,".asm"); 	/*Open .asm file using fptr*/

	FILE *fptr = fopen(filename,"r");

	filename[flen] = '\0';
 	strcat(filename,".l");

	FILE *list = fopen(filename,"w");
	filename[flen] = '\0'; 		/*Open .l file to write using list*/
	
 	strcat(filename,".log");
 	FILE *log = fopen(filename,"a");  /*Opens log file*/
 	filename[flen] = '\0';

	char line[200];
	int line_num = 1;
	int is_error = 0;
	int PC = 0;
	int data_ptr = 0;
	
	while(fgets(line,sizeof line,fptr) != NULL){
		rStrip(line);
		char* parse = line;
		char word[MAX_CHAR];
		int cc;
		int idx = 0;
		int isLabel = 0;
		int isData = 0;
		int mnemonic = 0;
		char label[MAX_CHAR];
		while(parse[0] != '\0' && parse[0] != ';' && parse[0]!='\n'){
			sscanf(parse,"%s%n",word,&cc);
			int len = strlen(word);
			if(len == 0)
				break;
			if(word[0]==';')
				break;
			if(word[len-1] == ';'){
				cc--;
				word[--len] = '\0';
			}
			if(!idx){
				char* ptr = word;
				char* lbl = strtok_r(ptr,":",&ptr);
				int label_length = strlen(lbl);
				if(len != label_length){
					cc -= len;
					cc += label_length+1;

					word[label_length]=':';
					word[label_length+1]='\0';

					len = label_length+1;
				}
			}
			if(word[len-1]==':'){
				isLabel = 1;
				word[--len] = '\0';
				strcpy(label,word);				
			}
			else if(idx <= 2){
				if(idx-isLabel == 0){
					if(!strcmp("data",word))
						isData = 1;
					else{		/*If a mnemonic then increase PC*/
						PC++;
						mnemonic = whichMnemonic(word); /*If the mnemonics dont require operand, insert into CodeSegment*/
						if(((mnemonic>=6 && mnemonic<=9) || (mnemonic==11 || mnemonic==12 || mnemonic==14 || mnemonic==18)))
						{
							insertCode(mnemonic);
							fprintf(list,"%06x %08x",CodeTail->address,CodeTail->binary);
						}
					}
				}
				else{
					char* ptr = word;
					int dt = strtol(ptr,&ptr,0);
					
					if(ptr != word+len){     /*label*/				
						if(mnemonic == 0){ 		/* Case for 'ldc' */
							Data* data = dataExists(word);
							Label* label = labelExists(word);
							if(data != NULL){  				/*If a data label is used, load its address in DS*/
								int val = ((data->add)<<8)|mnemonic;
								insertCode(val);
								fprintf(list,"%06x %08x",CodeTail->address,CodeTail->binary);
								isLabel = 0;
							}
							else if(label != NULL){ 		/*If a instruction label is used, load its address in CS*/
								fprintf(log, "Warning: A Label is being used: %s at line %d\n",word,line_num);
								int val = ((label->address)<<8)|mnemonic;
								insertCode(val);
								fprintf(list,"%06x %08x",CodeTail->address,CodeTail->binary);
								isLabel = 0;
							}
							else{ 								/*If the label doesn't exists*/
								is_error = 1;
								fprintf(log, "Error: Unknown data label: %s at line %d\n",word,line_num);
							}
						}
						else if(mnemonic == 13 || (mnemonic>=15 && mnemonic<=17)){
							Label* label = labelExists(word); 	/*Find Label*/
							if(label == NULL){ 					/*If not found, throw error*/
								is_error = 1;
								fprintf(log, "Error: Unknown label: %s used at line %d\n",word,line_num);
							}
							else{ 								/*Insert the difference between PC and the label address*/	
								int val = ((label->address-PC)<<8)|mnemonic;
								insertCode(val);
								fprintf(list,"%06x %08x",CodeTail->address,CodeTail->binary);
								isLabel = 0;
							}
						}
						else 									/*These mnemonics don't require any operand*/
							fprintf(log, "Error: Unknown operands used at line %d\n",line_num);
					}
					else{	/*number*/
						if(isData){ 						/*Insert the number into data Segment if isData = True*/
							DataSeg[data_ptr++] = dt;
							fprintf(list, "%08x", dt);
						}
						else if(!(mnemonic < 0 || (mnemonic>=6 && mnemonic<=9) || (mnemonic==11 || mnemonic==12 || mnemonic==14 || mnemonic==18))){
							int val = (dt<<8)|mnemonic; 	/*If other than these mnemonics insert into CodeSegment*/
							insertCode(val);
							fprintf(list,"%06x %08x",CodeTail->address,CodeTail->binary);
							isLabel = 0;
						}
						else 								/*These mnemonics don't require any operand*/
							fprintf(log, "Error: Unknown operands used at line %d\n",line_num);
					}
				}
				
			}
			++idx;
			parse += cc;
		}
		if(idx-isLabel==1){ 	/*Adjusting for the label, if there is no operand for the not specified mnemonics*/
			if(!(mnemonic < 0 || (mnemonic>=6 && mnemonic<=9) || (mnemonic==11 || mnemonic==12 || mnemonic==14 || mnemonic==18))){
				is_error = 1;	/*It is an error*/
				fprintf(log,"Error: No operand at line %d\n",line_num);
			}
		}
		if(isLabel && !isData)
			fprintf(list,"%06x",PC);
		fprintf(list,"\t\t%s\n",line);
		line_num++;
	}
	fclose(fptr);
	fclose(list);
	fclose(log);
	return !(is_error);				/*Return if error free or not*/
}

void generateObjectCode(char* filename){
	strcat(filename,".o");
	FILE *obj = fopen(filename,"wb"); 	/*Open .o file in binary mode to write*/
	
	Code* trav = CodeHead;				
	int sizes[] = {CodeTail->address+1,data_size}; /*Declare an array for storing sizes of CS and DS*/

	fwrite(sizes,sizeof(int),2,obj);  	/*Write in binary the sizes array*/

	int *CodeSeg = malloc(sizeof(int)*(sizes[0]));  /*Declare an array for the Code Segment*/

	int x = 0;
	while(trav != NULL){ 		/*Put the code into Code Codesegment from the reverse linked List*/
		CodeSeg[x++] = trav->binary;
		trav = trav->next;
	}
	fwrite(CodeSeg,sizeof(int),sizes[0],obj); 		/*Write the code segment first*/
	fwrite(DataSeg,sizeof(int),sizes[1],obj);		/*Then write the data segment*/
	fclose(obj);
}

int main(int argc,char* argv[]){
	LabelHead = LabelTail = NULL;
	CodeHead = CodeTail = NULL;
	DataHead = DataTail = NULL;

	if(argc != 2){ 		/*Insufficient arguments or wrong arguments*/
		fprintf(stderr,"Error: Only pass the .asm file");
		return 0;
	}
	/*Check if the file is .asm file or not*/
	int fname_len = strlen(argv[1]); 	
	if(fname_len<=3){
		fprintf(stderr,ERR_FILE);
		return 0;
	}
	if(strcmp("asm",argv[1]+fname_len-3)){
		fprintf(stderr,ERR_FILE);
		return 0;
	}
	char filename[150];
	strcpy(filename,argv[1]);
	filename[fname_len-4] = '\0';
	FILE* fptr = fopen(argv[1],"r");
	if(!fptr){
		fprintf(stderr,"Error: File doesn't exit!");
		return 0;
	}
	int fp = execute_first_pass(filename);
	int sp = execute_second_pass(filename);
	if(fp && sp) 	/*If both passes give no error, generate object code*/
		generateObjectCode(filename);
	return 0;
}	
