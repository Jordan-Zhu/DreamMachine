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

/* void spaceFormat(int m)
{
  if(m < 10)
   	printf(fw, " ");
    
  if(m < 100)
   	printf(fw, " ");
} */

//Function Used to print the stack
void printStack(int stack[],int sp, FILE* fw,int bp){
		int i = 0;
		int j = 0;
		
		//Outer loop to loop through the stack
		for(i = 1; i < sp+1;i++){
			//If there is a activation record we enter this loop;
			for(j = 0; j < ARcount;j++ ){
				
				//If our base pointer array equals the location that we are at
				//in the stack we print "|" to seperate it from the other Ar
				if(Bp[j] == i){
					fprintf(fw,"|");
				}	
			}
				fprintf(fw," %d ", stack[i]);		
		}
}

//Function definitions//
int NewAr(int* sp, int* pc ,int* bp, instruction instructions[],int stack[], FILE* fw, int t);
int execute(int* sp, int* pc ,int* bp, instruction instructions[], int stack[],FILE* fw);

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
	FILE* fp = fopen("add.pl0", "r");
	
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
	
	//File opener that creates text document called stacktrace.txt
	FILE* fw = fopen("trace.txt", "w");
	
	
	fprintf(fw, "PL/0 code:\n");
	
	//For loop to obtain all intructions from the input file and print them to the stack trace 
	//file.
	for(i = 0; i < instructionC;i++){
		
		
		/*printf(" (%d) instruction count %d\n", i,instructionC);
		fflush(stdout);*/
	
	//Switch statement will switch on the op code to indicate what type of instruction the system will perform
	//That is then printed to the stacktrace.txt
	
	switch(instructions[i].op){
		
		//LIT
		case 1:
			fprintf(fw,"\n%*d\tLIT\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//OPR
		case 2:
			fprintf(fw,"\n%*d\tOPR\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//LOD
		case 3:
			fprintf(fw,"\n%*d\tLOD\t%*d\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].l,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//STO
		case 4:
			fprintf(fw,"\n%*d\tSTO\t%*d\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].l,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//CAL
		case 5:
			fprintf(fw,"\n%*d\tCAL\t%*d\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].l,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//INC
		case 6:
			fprintf(fw,"\n%*d\tINC\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//JMP
		case 7:
			fprintf(fw,"\n%*d\tJMP\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//JPC
		case 8:
			fprintf(fw,"\n%*d\tJPC\t\t%*d",MAX_LEXI_LEVELS,line,MAX_LEXI_LEVELS, instructions[i].m);
			instructions[i].line = line;
			break;
		//HLT
		case 9:
			fprintf(fw,"\n%*d\tHLT",MAX_LEXI_LEVELS,line);
			instructions[i].line = line;
			break;
			
			
			
	}
	
		fflush(fw);
	
		line++;
	}
	
		fprintf(fw, "\n\nExecution:");
		fprintf(fw, "\n\t\t\t\tpc\tbp\tsp\tstack\t");
		fprintf(fw, "\n\t\t\t\t0\t1\t0");
		
		
	//We continue to loop through the function until it returns 1, this would signal us to stop
	while( execute(&sp, &pc , &bp, instructions,  stack,fw)!= 1);
	
	fprintf(fw,"\n");
	
	fclose(fw);
	fclose(fp);

}
//This function will process the instruction. It takes in the stack pointer the program counter the instructions
//the stack and  also the filewriter.
int execute(int* sp, int* pc ,int* bp, instruction instructions[],int stack[], FILE* fw){
		
		int temp = 0;
		int flag = 0;
	
		//Switch statement on the opcode will indicate what type of intruction type the system will execute
		//The system then executes it.
		switch(instructions[*pc].op){
			//LIT
			case 1:
				*sp = *sp + 1;
				stack[*sp] = instructions[*pc].m;
				fprintf(fw,"\n%d\tLIT\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
				printStack(stack, *sp,fw, *bp);
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
							fprintf(fw,"\n%d\tRET\t%d\t%d\t%d\t%d\t%d\t",instructions[temp].line, instructions[temp].l, instructions[temp].m, *pc,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
							return 1;
							break;
						//NEG
						case 1:
							*sp = *sp - 1;
							stack[*sp] = -stack[*sp];
							fprintf(fw,"\n%d\tNEG\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
							*pc = *pc +1;
							break;
						//ADD
						case 2:
							*sp = *sp - 1;
							stack[*sp]= stack[*sp] + stack[*sp + 1];
							fprintf(fw,"\n%d\tADD\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
							*pc = *pc +1;
							break;
						//SUB
						case 3:
							*sp = *sp - 1;
							stack[*sp]= stack[*sp] - stack[*sp + 1];
							fprintf(fw,"\n%d\tSUB\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
							*pc = *pc +1;
							break;
						//MUL
						case 4:
							*sp = *sp - 1;
							stack[*sp]= stack[*sp] * stack[*sp + 1];
							fprintf(fw,"\n%d\tMUL\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
							*pc = *pc +1;
							break;
						//DIV
						case 5:
							*sp = *sp - 1;
							stack[*sp]= (stack[*sp] / stack[*sp + 1]);
							fprintf(fw,"\n%d\tDIV\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
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
							fprintf(fw,"\n%d\tODD\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
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
							fprintf(fw,"\n%d\tMOD\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
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
							fprintf(fw,"\n%d\tEQL\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
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
							fprintf(fw,"\n%d\tNEQ\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
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
							fprintf(fw,"\n%d\tLSS\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
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
							fprintf(fw,"\n%d\tLEQ\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
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
							fprintf(fw,"\n%d\tGTR\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
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
							fprintf(fw,"\n%d\tGEQ\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
							printStack(stack, *sp,fw, *bp);
							*pc = *pc +1;
							break;
					}
				break;
			//LOD
			case 3:
				*sp = *sp + 1;
				stack[*sp] = stack[base(instructions[*pc].l,*bp, stack) + instructions[*pc].m];
				fprintf(fw,"\n%d\tLOD\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
				printStack(stack, *sp,fw, *bp);
				*pc = *pc + 1;	
				break;
			//SIO
			case 4:	
				stack[base(instructions[*pc].l,*bp,stack) + instructions[*pc].m ] = stack[*sp];
				*sp = *sp - 1;
				fprintf(fw,"\n%d\tSIO\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
				printStack(stack, *sp,fw, *bp);
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
				fprintf(fw,"\n%d\tCAL\t%d\t%d\t%d\t%d\t%d\t",instructions[temp].line, instructions[temp].l, instructions[temp].m, *pc,*bp,*sp);
				printStack(stack, *sp,fw, *bp);
				Bp[c] = *bp;
				c = c + 1;
				ARcount += 1;
				while(flag != 1){
					//Does a a recursive call on execute as a new Ar was created.
					flag = execute(sp, pc ,bp,instructions, stack, fw); 
				}
				break;
			//INC
			case 6:
				*sp = *sp + instructions[*pc].m;
				fprintf(fw,"\n%d\tINC\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
				printStack(stack, *sp,fw, *bp);
				*pc = *pc +1;
				break;
			//JMP
			case 7:
				temp = *pc;
				*pc = instructions[*pc].m;
				fprintf(fw,"\n%d\tJMP\t%d\t%d\t%d\t%d\t%d\t",instructions[temp].line, instructions[temp].l, instructions[temp].m, *pc,*bp,*sp);
				if(instructions[temp].line != 0){
				printStack(stack, *sp,fw, *bp);
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
				fprintf(fw,"\n%d\tjpc\t%d\t%d\t%d\t%d\t%d\t",instructions[temp].line, instructions[temp].l, instructions[temp].m, *pc,*bp,*sp-1);
				*sp = *sp - 1;
				printStack(stack, *sp,fw, *bp);
				break;
			//SIO
			case 9:
				
				
				if( 9 + instructions[*pc].m == 9){
					
				*sp = *sp - 1;
				fprintf(fw,"\n%d\tOUT\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
				printStack(stack, *sp,fw, *bp);
				*pc = *pc +1;
				break;
					
					
					
				}else if( 9 + instructions[*pc].m == 10 ){
					
				*sp = *sp + 1;
				printf("Enter Value to be placed on stack: ");
				scanf("%d", &stack[*sp]);
				fprintf(fw,"\n%d\tINP\t%d\t%d\t%d\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m, *pc+1,*bp,*sp);
				printStack(stack, *sp,fw, *bp);
				*pc = *pc +1;
				break;
					
					
				}else if( 9 + instructions[*pc].m == 11 ){
					
					
				fprintf(fw,"\n%d\tHLT\t%d\t%d\t",instructions[*pc].line, instructions[*pc].l, instructions[*pc].m);
				//Returns one to indicate the system halt.
				return 1;
				break;
					
					
				}
				

				
		}
			return 0;
		}

		