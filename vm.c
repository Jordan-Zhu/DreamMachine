// Compiler Builder 42
//  Alex Phillips
//  Jordan Zhu
//  Jamal Roberts
//  Nicholas Deleuze

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

typedef struct instruction{
  int op;   // opcode
  int  l;   // L
  int  m;   // M
} instruction;

void OprStringSwitch(int s){
  
  switch (s){
    	case 0:
    			printf("RET   ");
    		break;
   		case 1:
    			printf("NEG   ");
    		break;
   	 	case 2:
    			printf("ADD   ");
    		break;
    	case 3:
    			printf("SUB   ");
    		break;
    	case 4:
    			printf("MUL   ");
    		break;
    	case 5:
    			printf("DIV   ");
    		break;
    	case 6:
    			printf("ODD   ");
    		break;
    	case 7:
    			printf("MOD   ");
    		break;
    	case 8:
    			printf("EQL   ");
    		break;
   		case 9:
    			printf("NEQ   ");
    		break;
    	case 10:
          printf("LSS   ");
    		break;
    	case 11:
    			printf("LEQ   ");
    		break;
    	case 12:
    			printf("GTR   ");
    		break;
    	case 13:
    			printf("GEQ   ");
    default:
    	break;
    
    
  }
  
}

void spaceFormat(int m)
{
  if(m < 10)
   	 printf(" ");
    
  if(m < 100)
   	 printf(" ");
}

void printInstructions(instruction *input, int length)
{
  
  int i;
  
  printf("PL/0 code:\n\n");
  
  for(i = 0; i < length; i++){
    
    spaceFormat(i);
    printf("%d  ", i);
	
	switch (input[i].op){
    			
    			case 1:
    						printf("LIT       ");
    						spaceFormat(input[i].m);
    						printf("%d\n", input[i].m);
    				break;
 	   			case 2:
    						OprStringSwitch(input[i].m);
                printf("%d\n", input[i].m);
    
    				break;
    			case 3:
    						printf("LOD   ");
                printf("%d   ", input[i].l);
    						spaceFormat(input[i].m);
    						printf("%d\n", input[i].m);
    				break;
    			case 4:
    						printf("STO   ");
    						printf("%d   ", input[i].l);
    						spaceFormat(input[i].m);
    						printf("%d\n", input[i].m);
    				break;
    			case 5:
    						printf("CAL   ");
    						printf("%d   ", input[i].l);
    						spaceFormat(input[i].m);
    						printf("%d\n", input[i].m);
    				break;
    			case 6:
    						printf("INC       ");
    						spaceFormat(input[i].m);
    						printf("%d\n", input[i].m);
    				break;
   				case 7:
    						printf("JMP       ");
    						spaceFormat(input[i].m);
    						printf("%d\n", input[i].m);
    				break;
   				case 8:
    						printf("JPC       ");
    						spaceFormat(input[i].m);
    						printf("%d\n", input[i].m);
    				break;
   			 	case 9:
    						if(input[i].m == 0)
                {
                  printf("OUT\n");
                }
                else if(input[i].m == 1)
                {
                  printf("INP\n");
                }
                else if(input[i].m == 2)
                {
                  printf("HLT\n");
                }
    				break;
  	 			default:
  	   			break;
  
    
  }
    
    
  }
  
  
  
}

int main(void)
{
	

    int sp = 0; // Stack pointer
    int bp = 1; // Base pointer
		int pc = 0; // Program Counter
		int ir = 0; //

    int *stack = (int*)calloc(MAX_STACK_HEIGHT, sizeof(int));
	instruction *input = malloc(sizeof(instruction)*MAX_CODE_LENGTH);

    //Open the mcode.pl0 file opener
    FILE *mo = fopen("mcode.pl0", "r");
    char buffer[100];
    int i = 0;
    while(!feof(mo)) {
        fscanf(mo, "%d %d %d", &input[i].op, &input[i].l, &input[i].m);
        
        i++;
    }
	i--;
	
	printInstructions(input, i);
	
	return 0;
}