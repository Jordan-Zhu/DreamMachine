#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parse.h"
#include "Lexer.h"
#include "tokens.h"
#include "util.h"
 
#define MAX 1000
#define CONST_KIND 1
#define VAR_KIND 2
#define PROC_KIND 3

typedef struct symbol {
 int kind; // const = 1, var = 2, proc = 3
 char name[12]; // name up to 11 chars
 int val; // number (ASCII value)
 int level; // L level
 int addr; // M address
} symbol;

// Contains details for token: int tok, char *id, int num.
static tokenStruct tokens[MAX];
static symbol symbolTable[MAX];
// For every instruction, gives the opcode, L, and M.
static instruction code[MAX];

//Global token array to store tokens.
static int Tokens[MAX];
//Global string array to store identfiers and digits.
static char buff[MAX][13];
//The max value of tokens will be stored here once computed.
static int RMAX;
//Current Token
static int token;

//Count to determine if errors were found in code.
// static int errorFound = 0;

// code index, token index, and symbol index
static int cx, symbolIndex, tokenIndex = -1;
static int level = 0, jump = 0;


//Function signatures
void block();		// Checks for constant, variable, or procedure delcaration in the PL/0 code input.
void statement();
void condition();
void factor();
void term();
void expression();
void error();	// Halts the program and prints an error message.

void getNextToken();
void const_decl();
void var_decl();
void proc_decl();
void enter(int kind, int level, int m);	// Stores a new symbol in the symbol table.

// Code gen
void emit(int op, int l, int m);		// Inserts a new instruction into the code list.

void PrintToks(){
    int i;
    for(i = 0; i < RMAX; i++){
        // printf("Value Tokens[%d]: = %d      Buff[%d]: = %s\n", i,Tokens[i],i,buff[i]);
        if (tokens[i].tok == 2) {
        	printf("%d. Token = %d 			Value = %s\n", i, tokens[i].tok, tokens[i].id);
        }
        else if (tokens[i].tok == 3) {
        	printf("%d. Token = %d 			Value = %d\n", i, tokens[i].tok, tokens[i].num);
        }
        else {
        	printf("%d. Token = %d\n", i, tokens[i].tok);
        }
    }

    return;
}

//Funtction to place tokens into the array.
void PlaceToks()
{
    FILE* toks = fopen("TokenFormat.txt", "r");
    int i = 0;

    while(fscanf(toks,"%d",&Tokens[i]) != EOF){
    	tokens[i].tok = Tokens[i];

        if(Tokens[i] == 2){
            fscanf(toks,"%s",buff[i]);
            tokens[i].id = buff[i];
        }else if(Tokens[i] == 3){
            fscanf(toks,"%s",buff[i]);
            tokens[i].num = atoi(buff[i]);
        }
        i++;
    }
	//This will store the max amount of tokens we will need to walk through.
    RMAX = i;

    return;

}
void getNextToken(){
	
	//Pointer is incremented.
    tokenIndex++;
	
	if((tokenIndex > RMAX))
	{
		tokenIndex--;
		return;
	}
	
	//Will set the token equal to current token
    token = Tokens[tokenIndex];
	
	return;

}
void program() {
	getNextToken();
	level = 0;
	block();
	if (token != periodsym) {
		error(9);		// Error: period expected
	}
	else {
		printf("No errors, program is syntactically correct\n");
	}
}

void enter(int kind, int level, int m)
{
	// symbolTable[symbolIndex] = malloc(sizeof(symbol));

	// Every symbol's kind is stored
	symbolTable[symbolIndex].kind = kind;
	// strcpy(symbolTable[symbolIndex].name, tokens[symbolIndex].id);
	int nameLen = strlen(tokens[symbolIndex].id) + 1;
	// char name[nameLen];
	// int i;
	// for(i=0; i < nameLen; i++)
 //  	{
 //    	name[i] = tokens[symbolIndex].id[i];
 //  	}
	// symbolTable[symbolIndex].name = name;
	memcpy(symbolTable[symbolIndex].name, tokens[symbolIndex].id, nameLen);

	switch(kind) {
		// const
		// For constants, you must store kind, name and value.
		case 1:
			symbolTable[symbolIndex].val = tokens[symbolIndex].num;
			break;
		// var
		// For variables, you must store kind, name, L and M.
		case 2:
			symbolTable[symbolIndex].level = level;
			symbolTable[symbolIndex].addr = m;
			break;
		// proc
		// For procedures, you must store kind, name, L and M.
		case 3:
			symbolTable[symbolIndex].level = level;
			symbolTable[symbolIndex].addr = m;
			break;
	}
	symbolIndex++;
}

void emit(int op, int l, int m) {
	if(cx > CODE_SIZE) {
		error(25);		// error: index out of bounds
	}
	else {
		code[cx].op = op;	// opcode
		code[cx].l = l;		// lexicographical level
		code[cx].m = m;		// modifier
		cx++;
	}
}

void block() {
	if (token == constsym) {
		// Handle constant declaration error checking and getting next token
		const_decl();
	}
	if (token == varsym) {
		var_decl();
	}
	while (token == procsym) {
		proc_decl();
	}
	statement();
}

void const_decl() {
	if (token != constsym) return;

	do{
		getNextToken();
		if(token != identsym)
		{
			error(14);	// error: call must be followed by an identifier.
		}
		getNextToken();
		if (token != eqsym)
		{
			error(3);	// error: Identifier must be followed by =.
		} 
		getNextToken();
		if (token != numbersym)
		{		 
			error(2);	// error: = must be followed by a number.
		}
		enter(CONST_KIND, level, 0);	// constant => kind = 1
		getNextToken();
	}while (token == commasym);
		 
	if (token != semicolonsym)
	{
		error(5);		// error: Semicolon or comma missing.
	} 
	getNextToken();
}

void var_decl() {
	do {
		getNextToken();
		if (token != identsym) {
			error(14);		// error: call must be followed by an identifier.
		} 
		enter(VAR_KIND, level, 0);
		getNextToken();
	} while(token == commasym);
		 
	if(token != semicolonsym) {
		error(5);			// error: Semicolon or comma missing.
	} 
	
	getNextToken();
}

void proc_decl() {
	getNextToken();

	if (token != identsym) {	
			error(14);		// error: call must be followed by an identifier.
	}
	enter(PROC_KIND, level, 0);
	getNextToken();

	if (token != semicolonsym) {
			error(5);		// error: Semicolon or comma missing.
	}

	getNextToken();
	level++;
	block();

	if(token != semicolonsym) {
		error(5);			// error: Semicolon or comma missing.
	}

	getNextToken();
}

void statement() {
	if (token == identsym) {
		getNextToken();
		if (token != becomessym) {
			// error: becomes sym missing in statement
		}
		getNextToken();
		expression();
		// gen(STO, symbollevel(i), symboladdress(i));
	}
	else if (token == callsym) {
		getNextToken();
		if (token != identsym) {
			error(14);
		}
		getNextToken();
	}
	else if (token == beginsym) {
		getNextToken();
		statement();
		while (token == semicolonsym) {
			getNextToken();
			statement();
		}
		if (token != endsym) {
			error(27);
		}
		getNextToken();
	}
	else if (token == ifsym) {
		getNextToken();
		condition();
		if (token != thensym) {
			// error()
		}
		getNextToken();
		statement();
	}
	else if (token == whilesym) {
		getNextToken();
		condition();
		if (token != dosym) {
			// error()
		}
		getNextToken();
		statement();
	}
	else if(token == readsym) {
		getNextToken();
		if(token != identsym) {
			error(26);
		}
		getNextToken();
		if(token != semicolonsym) {
				//then error
		}
		getNextToken();
		statement();
	}
	else if(token == writesym) {
		getNextToken();
		if(token != identsym) {
			error(26);
		}
		getNextToken();
		if(token != semicolonsym) {
			//then error
		}
		getNextToken();
		statement();
	}
}

void condition()
{
	if(token == oddsym) {
		getNextToken();
		expression();
	}
	else
	{
		expression();
		if((token != lessym) || (token != gtrsym) || (token != geqsym) || (token != leqsym) || (token != eqsym) || (token != neqsym))
		{
			//then error
		}
		getNextToken();
		expression();
	}
}

// Parses an expression
void expression()
{
	if((token == plussym) || (token == minussym))
	{
		getNextToken();
	}
	term();
	while ((token == plussym) || (token == minussym))
	{ 
		getNextToken();
		term();
	}
}

void term()
{
	factor();
	while ((token == multsym) || (token == slashsym))
	{		
		getNextToken();
		factor();
	}
}

void factor()
{
	if (token == identsym) 
	{
		getNextToken();
	} 
	else if(token == numbersym)
	{
		getNextToken();
	} 
	else if (token == lparentsym)
	{
		getNextToken();
		expression();
		if(token != rparentsym)
		{
			//then error
		}
		getNextToken();
	} 
	else
	{
		//error
	}
}

// Prints error message and halts the program.
void error(int errorCase) {
	printf("Error number %d, ", errorCase);
	switch(errorCase) {
		case 1:
			printf("Use = instead of :=.\n");
			break;
		case 2:
			printf("= must be followed by a number.\n");
			break;
		case 3:
			printf("Identifier must be followed by =.\n");
			break;
		case 4:
			printf("'const', 'var', and 'procedure' must be followed by identifier.\n");
			break;
		case 5:
			printf("Semicolon or comma missing.\n");
			break;
		case 6:
			printf("Incorrect symbol after procedure declaration.\n");
			break;
		case 7:
			printf("Statement expected.\n");
			break;
		case 8:
			printf("Incorrect symbol after statement part in block.\n");
			break;
		case 9:
			printf("Period expected.\n");
			break;
		case 10:
			printf("Semicolon between statements missing.\n");
			break;
		case 11:
			printf("Undeclared identifier.\n");
			break;
		case 12:
			printf("Assignment to constant or procedure is not allowed.\n");
			break;
		case 13:
			printf("Assignment operator expected.\n");
			break;
		case 14:
			printf("'call' must be followed by an identifier.\n");
			break;
		case 15:
			printf("Call of a constant or variable is meaningless.\n");
			break;
		case 16:
			printf("'then' expected.\n");
			break;
		case 17:
			printf("Semicolon or '}' expected.\n");
			break;
		case 18:
			printf("'do' expected.\n");
			break;
		case 19:
			printf("Incorrect symbol following statement.\n");
			break;
		case 20:
			printf("Relational operator expected.\n");
			break;
		case 21:
			printf("Expression must not contain a procedure identifier.\n");
			break;
		case 22:
			printf("Right parenthesis missing.\n");
			break;
		case 23:
			printf("The preceding factor cannot begin with this symbol.\n");
			break;
		case 24:
			printf("An expression cannot begin with this symbol.\n");
			break;
		case 25:
			printf("This number is too large.\n");
			break;
		case 26:
            printf("Read or Write must be followed by an identifier.\n");
            break;
		case 27:
            printf("End symbol was expected.\n");
            break;
		default:
			break;
	}
	printf("Current Tok = %d", tokens[tokenIndex].tok);
	fflush(stdout);
	exit(EXIT_FAILURE);

	// Stops program when error occurs.
	// return 1;

	// Or we can choose to keep checking for additional errors
	// and then, wouldn't return anything. 
}


