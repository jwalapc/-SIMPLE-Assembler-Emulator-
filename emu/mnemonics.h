/*Author: Jwala Chorasiya   	Roll: 08 (TCET MUMBAI)
I hereby declare that I, Jwala Chorasiya from TCET MUMBAI, have authored this source code independently
that I have not used other than the declared sources/resources, and that I have explicitly marked
all material which has been quoted either literally or by content from the used sources.*/

/* opcodes have lower 8 bits and operands have upper 24 bits */
#define NUM_OF_OPCODES 18

int A,B;
int PC,SP;
int *memory;

void ldc(const void* value){
	// Load accumulator with the value specified 
	B = A;
	A = *(int*)value;
}

void adc(const void* value){
	// Add to the accumulator
	A += *(int*)value;
}

void ldl(const void* offset){
	// Load local 
	B = A;
	A = memory[SP + *(int*)offset];
}

void stl(const void* offset){
	// Store local 
	memory[SP + *(int*)offset] = A;
	A = B;
}

void ldnl(const void* offset){
	// Load non-local
	A = memory[A + *(int*)offset];
}

void stnl(const void* offset){
	// Store non-local 
	memory[A + *(int*)offset] = B;
}

void add(const void* off){
	// Addition 
	*(int*)off = 0;
	A = B+A;
}

void sub(const void* off){
	// Subtraction 
	*(int*)off = 0;
	A = B-A;
}

void shl(const void* off){
	// Shift left 
	*(int*)off = 0;
	A = B<<A;
}

void shr(const void* off){
	// Shift right 
	*(int*)off = 0;
	A = B>>A;
}

void adj(const void* value){
	// Adjust SP 
	SP = SP + *(int*)value;
}

void a2sp(const void* off){
	// Transfer A to SP; 
	*(int*)off = 0;
	SP = A;
	A = B;
}

void sp2a(const void* off){
	// Transfer SP to A 
	*(int*)off = 0;
	B = A;
	A = SP;
}

void call(const void* offset){
	// Call procedure 
	B = A;
	A = PC;
	PC = PC + *(int*)offset;
}

void ret(const void* offset){
	// Return from procedure
	*(int*)offset = 0;
	PC = A;
	A = B;
}

void brz(const void* offset){
	// If accumulator is zero, branch to
	// specified offset 
	if(A == 0)
		PC = PC + *(int*)offset;
}

void brlz(const void* offset){
	// If accumulator is less than zero,
	// branch to specified offset 
	if(A < 0)
		PC = PC + *(int*)offset;
}

void br(const void* offset){
	// Branch to specified offset 
	PC = PC + *(int*)offset;
}

void (*asm_func[])(const void*) = {ldc,adc,ldl,stl,ldnl,stnl,add,sub,shl,shr,adj,a2sp,sp2a,call,ret,brz,brlz,br};
char mnemonics[][7] = {"ldc","adc","ldl","stl","ldnl","stnl","add","sub","shl","shr","adj","a2sp","sp2a","call","return","brz","brlz","br","HALT"};
