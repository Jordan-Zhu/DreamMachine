#include <assert.h>

typedef char *string;
typedef char bool;

#define TRUE 1
#define FALSE 0
#define CODE_SIZE 1000

void *checked_malloc(int);
char * String(char *);

//for connecting parser to vm, add "extern instruction code[CODE_SIZE];
//as well as instruction struct below

typedef struct {
    int op; //Opcode
    int l;  //L
    int m;  //M
} instruction;

typedef enum {
	LIT = 1, OPR = 2, LOD = 3, STO = 4, CAL = 5, INC = 6, JMP = 7, JPC = 8, SIO = 9
} instruction_type;

typedef enum {
	OUT = 0, INP = 1, HLT = 2, RTN = 0, OPR_NEG = 1, OPR_ODD = 6,
	OPR_ADD = 2, OPR_SUB = 3, OPR_MUL = 4, OPR_DIV = 5, OPR_MOD = 6,
	OPR_EQL = 8, OPR_NEQ = 9, OPR_LSS = 10, OPR_LEQ = 11, OPR_GTR = 12, OPR_GEQ = 13
} instruction_modifier;



// This line connects the parser to vm
// extern instruction code[CODE_SIZE];

