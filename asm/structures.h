/*
/*Author: Jwala Chorasiya   	Roll: 08 (TCET MUMBAI)
I hereby declare that I, Jwala Chorasiya from TCET MUMBAI, have authored this source code independently
that I have not used other than the declared sources/resources, and that I have explicitly marked
all material which has been quoted either literally or by content from the used sources.*/
#define MAX_CHAR 50

typedef struct Label{
	char name[MAX_CHAR];
	unsigned int address;
	struct Label* next;
}Label;

Label *LabelHead, *LabelTail;

void insertLabel(char* nm, unsigned int add){
	Label* newLabel = (Label*)malloc(sizeof(Label));
	strcpy(newLabel->name,nm);
	newLabel->address = add;
	newLabel->next = NULL;

	if(LabelTail == NULL)
		LabelHead = LabelTail = newLabel;
	else{
		LabelTail->next = newLabel;
		LabelTail = newLabel;
	}
}

Label* labelExists(char *nm){
	Label *tmp = LabelHead;
	while(tmp!=NULL){
		if(!strcmp(tmp->name,nm))
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}
 
typedef struct Code{
	unsigned int binary;
	unsigned int address;
	struct Code* next;
	struct Code* prev;
}Code;

Code *CodeHead,*CodeTail;

void insertCode(unsigned int bin){
	Code* newCode = (Code*)malloc(sizeof(Code));
	newCode->binary = bin;
	newCode->address = 0;
	newCode->next = NULL;
	newCode->prev = NULL;
	if(CodeTail == NULL)
		CodeHead = CodeTail = newCode;
	else{
		CodeTail->next = newCode;
		newCode->prev = CodeTail;
		newCode->address = CodeTail->address+1;
		CodeTail = newCode;
	}
}

typedef struct Data{
	char name[MAX_CHAR];
	int data;
	int add;
	struct Data* next;
}Data;

Data *DataHead, *DataTail;

void insertData(char* nm,int data,int add){
	Data* newData = (Data*)malloc(sizeof(Data));
	strcpy(newData->name,nm);
	newData->data = data;
	newData->next = NULL;
	newData->add = add;
	if(DataTail == NULL){
		DataHead = DataTail = newData;
	}
	else{
		DataTail->next = newData;
		DataTail = newData;
	}
}
Data* dataExists(char *nm){
	Data *tmp = DataHead;
	while(tmp!=NULL){
		if(!strcmp(tmp->name,nm))
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}
