// typedef enum {nulsym = 0, idsym, numsym, lparensym, rparensym, plussym, multsym} token_type;
typedef enum {

nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,

multsym = 6 , slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11 , leqsym = 12,

gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,

periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,

whilesym = 25 , dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,

readsym = 32, elsesym =33 } token_type;

typedef union {
  char *id;
  int num;   
} LVAL; 

//struct used to contain all tokens details
typedef struct {
	// The token's corresponding value
    token_type tok;
    // Number value if it's a numbersym
    int num;
    // Name if it's an identsym
    char *id;
}tokenStruct;