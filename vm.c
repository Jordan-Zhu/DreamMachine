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

void printInstructions(instruction *input, int length)
{
  for(int i = 0; i < length; i++)
  {
		printf("%d %d %d\n", input[i].op, input[i].l, input[i].m);
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
