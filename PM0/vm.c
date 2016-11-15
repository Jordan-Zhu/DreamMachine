// Compiler Builder 42
//  Alex Phillips
//  Jordan Zhu
//  Jamal Roberts
//  Nicholas Deleuze
#include <stdlib.h>
#include <stdio.h>

#define MAX_STACK_HEIGHT  2000
#define MAX_CODE_LENGTH  500
#define MAX_LEXI_LEVELS 3


static unsigned stack[MAX_STACK_HEIGHT];
//Keeps track of the amount of activation records
static int ARcount = 0;
//Stores all the base pointers
static int Bp[MAX_STACK_HEIGHT];
//Incrementer used to increment Bp array.
static int c = 0;

//Struct used to simplfy simplify accessing of instructions
typedef struct {
	
	int op; // opcode
	int l;	// L
	int m;  // M
	int line;//
	
}instruction;

//Function Used to print the stack
void printStack(int stack[],int sp,int bp){
		int i = 0;
		int j = 0;
		
		//Outer loop to loop through the stack
		for(i = 1; i < sp+1;i++){
			//If there is a activation record we enter this loop;
			for(j = 0; j < ARcount;j++ ){
				
				//If our base pointer array equals the location that we are at
				//in the stack we print "|" to seperate it from the other Ar
				if(Bp[j] == i){
					printf("|");
				}	
			}
				printf(" %d ", stack[i]);		
		}
}

//Function definitions//
int execute(int* sp, int* pc ,int* bp, instruction instructions[], int stack[]);

//Used to find base L levels down//
int base(int l, int base, int stack[]){
	
	int b1;
	b1 = base;
	while(l > 0){
		b1 = stack[b1 + 1];
		l--;
	}
	return b1;
	
}


int main(void){
	
	//Utilized in for loop//
	int b = 0;
	
	
	//Intialize the stack to 0;
	for(b = 0; b < MAX_STACK_HEIGHT; b++){	
		stack[b] = 0;
	}
	
	//Intialize Base Pointer
	unsigned bp = 1;
	//Intialize stack pointer
	unsigned sp = 0;
	//Initialize program counter
	unsigned pc = 0;
	//Intialize 

	//File opener to open instructions
	FILE* fp = fopen("mcode.pl0", "r");
	
	int instructionC = 0;
	int c = 0;
	int line = 0;
	
	//For loop counters
	int i = 0;
	int p = 0;
	int j = 0;
	
	instruction instructions[MAX_CODE_LENGTH]; 
	
	//Retrieves Instructions
	while(!feof(fp)){
		
		 fscanf(fp,"%d %d %d", &instructions[instructionC].op, &instructions[instructionC].l, &instructions[instructionC].m );
		
		instructionC ++;
		
		
	}
	
	instructionC--;
	
	printf( "PL/0 code:\n");
	
	//For loop to obtain all intructions from the input file and print them to the stack trace 
	//file.
	for(i = 0; i < instructionC;i++){
		
	
	//Switch statement will switch on the op code to indicate what type of instruction the system will perform
	//That is then printed to the stacktrace.txt
	
	switch(instructions[i].op){
		
		//LIT
		case 1:
			printf("\n%*d\tLIT\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//OPR
		case 2:
			
			switch( instructions[i].m){
				
				case 0:
					printf("\n%*d\tRET\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;
						
				case 1:
				
					printf("\n%*d\tNEG\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;
						

				case 2:
				
					printf("\n%*d\tADD\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;
						

				case 3:
					printf("\n%*d\tSUB\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;
						

				case 4:
					printf("\n%*d\tMUL\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;
						

				case 5:
					printf("\n%*d\tDIV\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;
						

				case 6:
					printf("\n%*d\tODDR\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;
						

				case 7:
					printf("\n%*d\tMOD\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;
						

				case 8:
					printf("\n%*d\tEQL\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;

				case 9:
					printf("\n%*d\tNEQ\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;
						

				case 10:
					printf("\n%*d\tLSS\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;
						

				case 11:
					printf("\n%*d\tLEQ\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;

				case 12:
					printf("\n%*d\tGTR\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;
						

				case 13:
					printf("\n%*d\tGEQ\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
					instructions[i].line = line;
					break;

				
				
				
				
			}
			
			break;
			
			//**Needs a switch to print the Different opr's**//
			//Need to format latter part with proper Format along with remove the printing of the 0's//
	
		//LOD
		case 3:
			printf("\n%*d\tLOD\t%*d\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].l,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//STO
		case 4:
			printf("\n%*d\tSTO\t%*d\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].l,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//CAL
		case 5:
			printf("\n%*d\tCAL\t%*d\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].l,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//INC
		case 6:
			printf("\n%*d\tINC\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//JMP
		case 7:
			printf("\n%*d\tJMP\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//JPC
		case 8:
			printf("\n%*d\tJPC\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//HLT
		case 9:
			printf("\n%*d\tHLT",MAX_LEXI_LEVELS,line);
			instructions[i].line = line;
			break;
			
			
			
	}
	
		line++;
	}
	
		printf( "\n\nExecution:");
		printf( "\n\t\t\t\t pc\t bp\t sp\t stack\t");
		printf( "\n\t\t\t\t  0\t  1\t  0");
		
		
	//We continue to loop through the function until it returns 1, this would signal us to stop
	while( execute(&sp, &pc , &bp, instructions,  stack)!= 1);
	
	printf("\n");
	
	fclose(fp);

}
//This function will process the instruction. It takes in the stack pointer the program counter the instructions
//the stack and  also the filewriter.
int execute(int* sp, int* pc ,int* bp, instruction instructions[],int stack[]){
		
		int temp = 0;
		int flag = 0;
	
		//Switch statement on the opcode will indicate what type of intruction type the system will execute
		//The system then executes it.
		switch(instructions[*pc].op){
			//LIT
			case 1:
				*sp = *sp + 1;
				stack[*sp] = instructions[*pc].m;
				printf("\n%*d\tLIT\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
				printStack(stack, *sp, *bp);
				*pc = *pc +1;
				break;
		//Case that it is an OPR the switch is done on the M instead of the OP.
			//OPR
			case 2:
					switch(instructions[*pc].m){
						//RET
						case 0:
							temp = *pc;
							*sp = *bp - 1;
							*pc = stack[*sp + 4];
							*bp = stack[*sp + 3];
							printf("\n%*d\tRET\t\t\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[temp].line,MAX_LEXI_LEVELS, *pc,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							return 1;
							break;
						//NEG
						case 1:
							*sp = *sp - 1;
							stack[*sp] = -stack[*sp];
							printf("\n%*d\tNEG\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
						//ADD
						case 2:
							*sp = *sp - 1;
							stack[*sp]= stack[*sp] + stack[*sp + 1];
							printf("\n%*d\tADD\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
						//SUB
						case 3:
							*sp = *sp - 1;
							stack[*sp]= stack[*sp] - stack[*sp + 1];
							printf("\n%*d\tSUB\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
						//MUL
						case 4:
							*sp = *sp - 1;
							stack[*sp]= stack[*sp] * stack[*sp + 1];
							printf("\n%*d\tMUL\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
						//DIV
						case 5:
							*sp = *sp - 1;
							stack[*sp]= (stack[*sp] / stack[*sp + 1]);
							printf("\n%*d\tDIV\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
						//ODD
						case 6:
							*sp = *sp - 1;
							 if(stack[*sp] % 2){
								 
								 stack[*sp] = 1;
							 }else{
								 stack[*sp] = 0;
							 }
							printf("\n%*d\tODD\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
						//MOD
						case 7:
							*sp = *sp - 1;
							 if(stack[*sp] % stack[*sp+1 ]){
								 
								 stack[*sp] = 1;
							 }else{
								 stack[*sp] = 0;
							 }
							printf("\n%*d\tMOD\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
						//EQL
						case 8:
							*sp = *sp - 1;
							 if(stack[*sp] == stack[*sp+1 ]){
								 
								 stack[*sp] = 1;
							 }else{
								 stack[*sp] = 0;
							 }
							printf("\n%*d\tEQL\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
						//NEQ
						case 9:
							*sp = *sp - 1;
							 if(stack[*sp] != stack[*sp+1 ]){
								 
								 stack[*sp] = 1;
							 }else{
								 stack[*sp] = 0;
							 }
							printf("\n%*d\tNEQ\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
						//LSS
						case 10:
							*sp = *sp - 1;
							 if(stack[*sp] < stack[*sp+1 ]){
								 stack[*sp] = 1;
							 }else{
								 stack[*sp] = 0;
							 }
							printf("\n%*d\tLSS\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
						//LEQ
						case 11:
							*sp = *sp - 1;
							 if(stack[*sp] <= stack[*sp+1 ]){ 
								 stack[*sp] = 1;
							 }else{
								 stack[*sp] = 0;
							 }
							printf("\n%*d\tLEQ\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
						//GTR
						case 12:
							*sp = *sp - 1;
							 if(stack[*sp] > stack[*sp+1 ]){
								 
								 stack[*sp] = 1;
							 }else{
								 stack[*sp] = 0;
							 }
							printf("\n%*d\tGTR\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
						//GEQ
						case 13:
							*sp = *sp - 1;
							 if(stack[*sp] >= stack[*sp+1 ]){
								 
								 stack[*sp] = 1;
							 }else{
								 stack[*sp] = 0;
							 }
							printf("\n%*d\tGEQ\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
							printStack(stack, *sp, *bp);
							*pc = *pc +1;
							break;
					}
				break;
			//LOD
			case 3:
				*sp = *sp + 1;
				stack[*sp] = stack[base(instructions[*pc].l,*bp, stack) + instructions[*pc].m];
				printf("\n%*d\tLOD\t%*d\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line, MAX_LEXI_LEVELS,instructions[*pc].l,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
				printStack(stack, *sp, *bp);
				*pc = *pc + 1;	
				break;
			//STO
			case 4:	
				stack[base(instructions[*pc].l,*bp,stack) + instructions[*pc].m ] = stack[*sp];
				*sp = *sp - 1;
				printf("\n%*d\tSTO\t%*d\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line, MAX_LEXI_LEVELS,instructions[*pc].l,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
				printStack(stack, *sp, *bp);
				*pc = *pc +1;
				break;
			//CAL
			case 5:
				temp = *pc;
				stack[*sp + 1] = 0;
				stack[*sp + 2] = base(instructions[*pc].l,*bp, stack);
				stack[*sp + 3] = *bp;
				stack[*sp + 4] = *pc + 1;
				*bp = *sp+1;
				*pc = instructions[*pc].m;
				printf("\n%*d\tCAL\t%*d\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[temp].line, MAX_LEXI_LEVELS,instructions[temp].l,MAX_LEXI_LEVELS, instructions[temp].m,MAX_LEXI_LEVELS, *pc,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
				printStack(stack, *sp, *bp);
				Bp[c] = *bp;
				c = c + 1;
				ARcount += 1;
				while(flag != 1){
					//Does a recursive call on execute as a new Ar was created.
					flag = execute(sp, pc ,bp,instructions, stack); 
				}
				break;
			//INC
			case 6:
				*sp = *sp + instructions[*pc].m;
				printf("\n%*d\tINC\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1 ,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
				printStack(stack, *sp, *bp);
				*pc = *pc +1;
				break;
			//JMP
			case 7:
				temp = *pc;
				*pc = instructions[*pc].m;
				printf("\n%*d\tJMP\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[temp].line,MAX_LEXI_LEVELS, instructions[temp].m,MAX_LEXI_LEVELS, *pc,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
				if(instructions[temp].line != 0){
				printStack(stack, *sp, *bp);
				}
				break;
			//JPC
			case 8:
				temp = *pc;
				if(stack[*sp] == 0){
					*pc = instructions[*pc].m;
				}else{
					*pc = *pc +1;
				}
				printf("\n%*d\tJPC\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[temp].line,MAX_LEXI_LEVELS, instructions[temp].m,MAX_LEXI_LEVELS, *pc,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
				*sp = *sp - 1;
				printStack(stack, *sp,*bp);
				break;
			//SIO
			case 9:
				
				
				if( 9 + instructions[*pc].m == 9){
        // OUT
				*sp = *sp - 1;
				printf("\n%*d\tOUT\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
				printStack(stack, *sp, *bp);
				*pc = *pc +1;
				break;
					
					
					
				}else if( 9 + instructions[*pc].m == 10 ){
        // INP
				*sp = *sp + 1;
				printf("Enter Value to be placed on stack: ");
				scanf("%d", &stack[*sp]);
				printf("\n%*d\tINP\t\t%*d\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, instructions[*pc].m,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
				printStack(stack, *sp, *bp);
				*pc = *pc +1;
				break;
					
					
				}else if( 9 + instructions[*pc].m == 11 ){
					
        // HLT
				printf("\n%*d\tHLT\t\t\t%*d\t%*d\t%*d\t",MAX_LEXI_LEVELS,instructions[*pc].line,MAX_LEXI_LEVELS, *pc+1,MAX_LEXI_LEVELS,*bp,MAX_LEXI_LEVELS,*sp);
				printStack(stack, *sp, *bp);
				//Returns one to indicate the system halt.
				return 1;
				break;
					
					
				}
				

				
		}
			return 0;
		}

		