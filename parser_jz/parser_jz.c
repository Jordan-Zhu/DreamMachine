#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser_jz.h"
#include "Lexer.h"
#include "util.h"

#define CODE_SIZE 1000
#define MAX 1000
// Constants for the symbol table
#define const_kind = 1
#define var_kind = 2
#define proc_kind = 3

typedef struct symbol {
 int kind; // const = 1, var = 2, proc = 3
 char name[12]; // name up to 11 chars
 int val; // number (ASCII value)
 int level; // L level
 int addr; // M address
} symbol;

// Global variables
// FILE * inputFile;	// Input file is given already from compile.c
FILE * outputFile;		// May want to write instructions to file. 

static char ident_digs[MAX][13];	//string array to store identfiers and digits.
static int tokens[MAX];		// array of tokens from lexer
static int cx;	// code index
static int token;	// current token, updates from getNextToken()
static int table_index, lvl, errorCount, numToks, tokIndex = 0;
static symbol *symbol_table = NULL;	// initialize array later
static instruction code[CODE_SIZE];	// PM/0 instructions


/* Function signatures */
void error();	// stops parsing and shows an error message.
void getNextToken();
void factor();
void condition();

// Exclusively parsing functions
void create_array();
void program();
void block();
void const_decl();
void var_decl();
void proc_decl();
void statement();

// Procedures for doing stuff with the symbol table.
void emit(int op, int l, int m);
void enter(int type, char* name, int params);	// Stores a new symbol into the symbol table.
void expression();		// Parses an expression

// Procedures for code generation
void gen(int a, int b, int c);	// inserts a new instruction into the code list
int find(int ident);	// returns the position of a symbol in the Symbol Table, or 0 if not found.
// symbol_type *get_symbol(char* name);	// looks up a symbol in symbol table by name and returns pointer if found
void symboltype(int symtype);	// returns the type of a symbol (constant, variable, or procedure).
void symbollevel(int a);	// Returns the level of a symbol.
void symboladdress(int adr);	// Returns the address of a symbol.
void put_symbol(int kind, char* name, int num, int level, int modifier);

// ------- FUNCTIONS CREATED / MODIFIED ON 11/10 --------
void create_array() {
	FILE * tokFile = fopen("TokenFormat.txt", "r");

	int i = 0;
	while(fscanf(tokFile, "%d", &tokens[i]) != EOF) {
		if(Tokens[i] == 2) {
            fscanf(toks,"%s", ident_digs[i]);
        }
        else if(Tokens[i] == 3) {
            fscanf(toks,"%s", ident_digs[i]);
        }
        i++;
	}
	// Max amount of tokens we will need to walk through.
	numToks = i;
}

void PrintToks() {
    int i;
    for(i = 0; i < numToks; i++){
        printf("Value Tokens[%d]: = %d      Buff[%d]: = %s\n", i, tokens[i], i, ident_digs[i]);
    }
}

void parse() {
	/* We probably want these to be globally declared. 
	 * Saves the cost of passing around it as a parameter and so many of the functions use them.
	 * Not sure if the statically declared arrays up there work tho
	 */
	// Declaring symbol_table array
	symbol_table[MAX_SYMBOL_TABLE_SIZE] = {0};
	// Holds the instructions in PM/0 code to be passed to the virtual machine.
	instruction code[CODE_SIZE];

	// Run program
	program();

	//prints instructions to screen; feel free to change this to print to file.
	int i = 0;
	// cx is the code size
    for (i=0; i<cx;i++) {
        printf(ofp, "%d %d %d\n", code[i].op, code[i].l, code[i].m);
    }
}

void program() {
	create_array();
	getNextToken();
	block();
	if (token != periodsym) {
		error(9);		// Error: period expected
	}

	if (errorCount == 0) {
		printf("No errors, program is syntactically correct\n");
	}
}

void getNextToken() {
	while (tokIndex < numToks + 1) {
		// Set the global token to current token at the index in the tokens array.
		token = tokens[tokIndex];
		tokIndex++;
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

// Other declaration functions have more checking
// which we don't need for the parser at the moment
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
		enter(const_kind, )
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

	getNextToken();

	if (token != semicolonsym) {
			error(5);		// error: Semicolon or comma missing.
	}

	getNextToken();
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

void condition() {
	if (token == oddsym) {
		getNextToken();
		expression();
	}
	else {
		expression();
		if((token != lessym) || (token != gtrsym) || (token != geqsym) || (token != leqsym) || (token != eqsym) || (token != neqsym))
		{
			//then error
		}
		getNextToken();
		expression();
	}
}

// Function to parse an expression
void expression() {
	int addop;
	if (token == plussym || token == minussym) {
		addop = token;
		getNextToken();
		term();
		if (addop == minussym) {
			emit(OPR, 0, OPR_NEG);	// negate
		}
	}
	else {
		term();
	}

	while (token == plussym || token == minussym) {
		addop = token;
		getNextToken();
		term();
		if (addop == plussym) {
			emit(OPR, 0, OPR_ADD);	// addition
		}
		else {
			emit(OPR, 0, OPR_SUB);	// subtraction	
		}
	}
}

// ------------- END ON 11/10 -----------------------
void enter(int type, char* name, int params) {
	symbol_table[table_index].kind = type;
}

void const_declaration() {
	if (token != constsym) return;
	string id;

	do {
		advance();

		if (token != identsym) {
			printf("expected identifier in constant declaration\n");
			return 1;
		}
		id = lval.id;
		advance();

		if (token != eqsym) {
			printf("expected '=' after identifier in constant declaration\n");
			return 1;
		}
		put_symbol(const_kind, id, lval.num, 0, 0);	// constant => kind = 1
		advance();
	} while (token == commasym);

	if (token != semicolonsym) {
		printf("expected ';' at the end of constant declaration\n");
		return 1;
	}
	advance();
}

void var_declaration() {

	int num_vars = 0;
	if (token == varsym) {
		do {
			advance();
			
			if (token != identsym) {
				printf("expected identifier in variable declaration");
				return 1;
			}

			num_vars++;
			put_symbol(2, lval.id, 0, 0, 3 + num_vars); // variable => kind = 2
			advance();
		} while (token == commasym);
		
		if (token != semicolonsym) {
			printf("expected ';' at the end of variable declaration");
			return 1;
		}
		advance();
	}

	emit(INC, 0, 4 + num_vars); // emit is defined on the following slides
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

void term() {
	int mulop;
	factor();
	while(token == multsym || token == slashsym)
	{
		mulop = token;
		getNextToken();
		factor();
		if(mulop == multsym)
			emit(OPR, 0, OPR_MUL); // multiplication
		else
			emit(OPR, 0, OPR_DIV); // division
	}
}

void factor() {
	if (token == identsym || token == numbersym) {
		getNextToken();
	}
	else if (token == lparentsym) {
		getNextToken();
		expression();
		if (token != rparentsym) {
			error(22);		// Error: right parentheses was expected
		}
		getNextToken();
	}
	else {
		// error
	}
}

// Prints error message and halts the program.
void error(int errorCase) {
	switch(errorCase) {
		case 1:
			printf("Error number %d, ", errorCase);
			printf("Use = instead of :=.\n");
			break;
		case 2:
			printf("Error number %d, ", errorCase);
			printf("= must be followed by a number.\n");
			break;
		case 3:
			printf("Error number %d, ", errorCase);
			printf("Identifier must be followed by =.\n");
			break;
		case 4:
			printf("Error number %d, ", errorCase);
			printf("'const', 'var', and 'procedure' must be followed by identifier.\n");
			break;
		case 5:
			printf("Error number %d, ", errorCase);
			printf("Semicolon or comma missing.\n");
			break;
		case 6:
			printf("Error number %d, ", errorCase);
			printf("Incorrect symbol after procedure declaration.\n");
			break;
		case 7:
			printf("Error number %d, ", errorCase);
			printf("Statement expected.\n");
			break;
		case 8:
			printf("Error number %d, ", errorCase);
			printf("Incorrect symbol after statement part in block.\n");
			break;
		case 9:
			printf("Error number %d, ", errorCase);
			printf("Period expected.\n");
			break;
		case 10:
			printf("Error number %d, ", errorCase);
			printf("Semicolon between statements missing.\n");
			break;
		case 11:
			printf("Error number %d, ", errorCase);
			printf("Undeclared identifier.\n");
			break;
		case 12:
			printf("Error number %d, ", errorCase);
			printf("Assignment to constant or procedure is not allowed.\n");
			break;
		case 13:
			printf("Error number %d, ", errorCase);
			printf("Assignment operator expected.\n");
			break;
		case 14:
			printf("Error number %d, ", errorCase);
			printf("'call' must be followed by an identifier.\n");
			break;
		case 15:
			printf("Error number %d, ", errorCase);
			printf("Call of a constant or variable is meaningless.\n");
			break;
		case 16:
			printf("Error number %d, ", errorCase);
			printf("'then' expected.\n");
			break;
		case 17:
			printf("Error number %d, ", errorCase);
			printf("Semicolon or '}' expected.\n");
			break;
		case 18:
			printf("Error number %d, ", errorCase);
			printf("'do' expected.\n");
			break;
		case 19:
			printf("Error number %d, ", errorCase);
			printf("Incorrect symbol following statement.\n");
			break;
		case 20:
			printf("Error number %d, ", errorCase);
			printf("Relational operator expected.\n");
			break;
		case 21:
			printf("Error number %d, ", errorCase);
			printf("Expression must not contain a procedure identifier.\n");
			break;
		case 22:
			printf("Error number %d, ", errorCase);
			printf("Right parenthesis missing.\n");
			break;
		case 23:
			printf("Error number %d, ", errorCase);
			printf("The preceding factor cannot begin with this symbol.\n");
			break;
		case 24:
			printf("Error number %d, ", errorCase);
			printf("An expression cannot begin with this symbol.\n");
			break;
		case 25:
			printf("Error number %d, ", errorCase);
			printf("This number is too large.\n");
			break;
		case 26:
			printf("Error number %d, ", errorCase);
            printf("Read or Write must be followed by an identifier.\n");
            break;
		case 27:
			printf("Error number %d, ", errorCase);
            printf("End symbol was expected.\n");
            break;
		default:
			break;
	}
	errorCount++;
	tokIndex--;

	// Stops program when error occurs.
	// return 1;

	// Or we can choose to keep checking for additional errors
	// and then, wouldn't return anything. 
}