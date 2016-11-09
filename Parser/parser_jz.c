#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CODE_SIZE 1000
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
FILE * inputFile;
FILE * outputFile;
int cx, token;

// Function signatures
void emit(int op, int l, int m);
void enter(int type, char* name, int params);	// Stores a new symbol into the symbol table.
void error();	// stops parsing and shows an error message.
void expression();
void getNextToken();
void factor();
void program();

void program() {
	getNextToken();
	block();
	if (token != periodsym) {
		error(9);		// Error: period expected
	}
}

void emit(int op, int l, int m) {
	if(cx > CODE_SIZE) {
		error(25);
	}
	else {
		code[cx].op = op	// opcode
		code[cx].l = l;		// lexicographical level
		code[cx].m = m;		// modifier
		cx++;
	}
}

void term() 
{
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
		default:
			break;
	}

	// Stops program when error occurs.
	return EXIT_FAILURE;
}