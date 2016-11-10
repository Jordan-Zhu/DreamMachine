#include <assert.h>

typedef char *string;
typedef char bool;

#define TRUE 1
#define FALSE 0
#define CODE_SIZE 1000

void *checked_malloc(int);
string String(char *);

typedef enum {

nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,

multsym = 6 , slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11 , leqsym = 12,

gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,

periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,

whilesym = 25 , dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,

readsym = 32, elsesym =33 } token_type;

//for connecting parser to vm, add "extern instruction code[CODE_SIZE];
//as well as instruction struct below

typedef struct {
    int op; //Opcode
    int l;  //L
    int m;  //M
} instruction;

// This line connects the parser to vm
extern instruction code[CODE_SIZE];

