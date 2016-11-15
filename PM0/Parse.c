#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parse.h"
#include "Lexer.h"
 
#define MAX 1000
#define MAX_CODE_LENGTH 500

typedef struct codePM0{
	
	int op;
	int l;
	int m;
	
}codePM0;
typedef struct symbol {
 int kind; // const = 1, var = 2, proc = 3
 char name[12]; // name up to 11 chars
 int val; // number (ASCII value)
 int level; // L level
 int m; // M address
} symbol;

//Code array to store pm0 code
static codePM0 code[MAX_CODE_LENGTH];
//Used inside emit finction
static int cx = 0;
//Global token array to store tokens.
static int Tokens[MAX];
//Global string array to store identfiers and digits.
static char buff[MAX][13];
//The max value of tokens will be stored here once computed.
static int RMAX = 0;
//Global token pointer to know which token we are currently on.
static int tokPointer = -1;
//Global symbol table pointer
static int symbPointer = 0;
//Current Token
static int token;
//Count to determine if errors were found in code.
static int errorFound = 0;
//Declare sysmbol table.
static symbol *symbolTable = NULL;

typedef enum {

nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,

multsym = 6 , slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11 , leqsym = 12,

gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,

periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,

whilesym = 25 , dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,

readsym = 32, elsesym =33 } token_type;

//Function signatures
void block();
void statement();
void condition();
void factor();
void term();
void expression();
void emit(int op, int l, int m);
void defineTable(symbol **tableStruct);
void pm0ToFile();
int find(char *str);

//Debugging function to see that all tokens were placed int the arrays correctly
void PrintToks(){
    int i;
    for(i = 0; i < RMAX; i++){
        printf("Value Tokens[%d]: = %d      Buff[%d]: = %s\n", i,Tokens[i],i,buff[i]);
    }

    return;
}

//Funtction to place tokens into the array.
void PlaceToks()
{
    FILE* toks = fopen("TokenFormat.txt", "r");
    int i = 0;

    while(fscanf(toks,"%d",&Tokens[i]) != EOF){

        if(Tokens[i] == 2){
            fscanf(toks,"%s",buff[i]);
        }else if(Tokens[i] == 3){
            fscanf(toks,"%s",buff[i]);
        }
        i++;
    }
	//This will store the max amount of tokens we will need to walk through.
    RMAX = i;
	defineTable(&symbolTable);

    return;

}

void enter(int kind, int level, int m)
{

	// type = const
	if(kind == 1)
	{
		symbolTable[symbPointer].kind = 1;
		strcpy(symbolTable[symbPointer].name,buff[tokPointer - 2]);
		symbolTable[symbPointer].val = atoi(buff[tokPointer]);
		
	}
	// type = var
	else if(kind == 2)
	{
		symbolTable[symbPointer].kind = 2;
		strcpy(symbolTable[symbPointer].name,buff[tokPointer]);
		symbolTable[symbPointer].level = level;
		symbolTable[symbPointer].m = m;
	}
	// type = proc
	else
	{
		symbolTable[symbPointer].kind = 3;
		strcpy(symbolTable[symbPointer].name,buff[tokPointer]);
		symbolTable[symbPointer].level = level;
		symbolTable[symbPointer].m = m;
	}
	
	symbPointer++;
}

void getToken(){
	
	//Pointer is incremented.
    tokPointer++;
	
	if((tokPointer > RMAX))
	{
		tokPointer--;
		return;
	}
	
	//Will set the token equal to current token
    token = Tokens[tokPointer];
	
	return;

}
void program(FILE* outputFile)
{
	
	//Get the token
	getToken();
	//Function call to Block
	block(0);
	
		if (token != periodsym)
		{
			error(9);
		}
		
	if(errorFound == 0){
		emit(9, 0, 2);
		printf("No errors, program is syntactically correct\n");
		pm0ToFile(outputFile);
	
	}
	
	return;
 }
 void block(int level)
 {
 
	 if (token == constsym)
	 {
		 do{
		 getToken();
		 if(token != identsym)
		 {
			 error(14);
		 }
		 getToken();
		 if (token != eqsym)
		 {
			
			error(3);
		 } 

		 getToken();
		 if (token != numbersym)
		 {		 
			error(2);
		 }else{
		
		 enter(1, level, 0);
		 }
		 getToken();   
		 }while (token == commasym);
		 
		 if (token != semicolonsym)
		 {
			 error(5);
		 } 
		 getToken();
		 
	 }

	 if (token == varsym)
	 {
		 int num_vars = 0;
		
		 do{
			getToken();
			if (token != identsym)
			{
			
				error(28);
			} 
			num_vars++;
			enter(2, level, 3 + num_vars);

			getToken();
	
		 }while(token == commasym);
		 
		 if(token != semicolonsym)
		 {
			 error(5);
		 } 
			getToken();
		emit(6, 0, 4 + num_vars);
	 }
	 while (token == procsym)
	 {
		getToken();
		if (token != identsym)
		{
			error(28);
		}

		enter(3, level, 0);

		getToken();
		if (token != semicolonsym)
		{
			error(5);
		} 
		getToken();
		block(level + 1);
		if(token != semicolonsym) 
		{
			error(5);
		}
		getToken();
	}
	statement();
	
	return;
 
 }
 void statement(){
	 if (token == identsym)
		{
		 int i = find(buff[tokPointer]);
		 if(i == -1)
		 {
			 error(11);
		 }
		 if(!((symbolTable[i].kind == 2) ||( symbolTable[i].kind == 1)))
		 {
			 error(11);
		 }
		 getToken();
		 if (token != becomessym)
		 {
			error(13);
		 }
		 getToken();
		 expression();
		 emit(4, symbolTable[i].level, symbolTable[i].m);
		}
	 else if (token == callsym)
		{ 
			 getToken();
			 if (token != identsym)
			 {
				error(14);
			 } 
			 getToken();
		}
	 else if (token == beginsym)
		{		 
			 getToken();
			 statement();
			 while(token == semicolonsym )
			 {
				 getToken();
				 statement();
			 }
			 if(token != endsym)
			 {
				 error(27);
			 }
			 getToken();
		}
	 else if (token == ifsym)
		{	 
			 getToken();
			 condition();
			 if(token != thensym)
			 { 
				 error(16);
			 }
			 else
			 {
			 getToken();
			 }
			 int ctemp = cx;
			 emit(8, 0, 0);
			 statement();
			 code[ctemp].m = cx;
		}
	 else if (token == whilesym)
		{	
			 int cx1 = cx;
			 getToken();
			 condition();
			 int cx2 = cx;
			 emit(8,0,0);
			 if (token != dosym)
			{			 
				error(18);
			}else
			{
			getToken();
			}
			statement();
			emit(7,0,cx1);
			code[cx2].m = cx;
		}
		else if(token == readsym)
		{
			getToken();
			if(token != identsym)
			{
				error(26);
			}
			int i = find(buff[tokPointer]);
			if(i == -1 )
			{
				error(11);
			}if(symbolTable[i].kind != 2)
			{
				error(11);
			}
			emit(9,0,1);
			emit(4, 0, symbolTable[i].m);
			
			getToken();
			if(token != semicolonsym)
			{
				error(5);
			}
			getToken();
			statement();
		}
		else if(token == writesym)
		{
			
			getToken();
			if(token != identsym)
			{
				error(26);
			}
			int i = find(buff[tokPointer]);
			if(i == -1 )
			{
				
				error(11);
			}else if(!((symbolTable[i].kind == 2)||(symbolTable[i].kind == 1)))
			{
				error(11);
			}if(symbolTable[i].kind == 2)
			{
				emit(3, 0, symbolTable[i].m);
				emit(9,0,0);
			}else if(symbolTable[i].kind == 1)
			{
				emit(3, 0, symbolTable[i].m);
				emit(9,0,0);
			}
			
			getToken();
			if(token != semicolonsym){
				error(5);
			}
			getToken();
			statement();
		}
	return;
 }
 void condition()
 {
	 int condition;
	 
	 if(token == oddsym){ 
		emit(2, 0 ,6);
		getToken();
		expression();
	 }
	 else
	 {
		 expression();
		 if(!((token == lessym) || (token == gtrsym) || (token == geqsym) || (token == leqsym) || (token == eqsym) || (token == neqsym)))
		 {
			 error(20);
		 }else
		 {
			 condition = token;
		 }
		 getToken();
		 expression();
		 if(condition == lessym)
		 {
			 
			 emit(2, 0 ,10);
		 }else if(condition == gtrsym)
		 {
			 emit(2, 0 ,12);
		 }
		 else if(condition == geqsym)
		 {
			 emit(2, 0 ,13);
		 }
		 else if(condition == leqsym)
		 {
			 emit(2, 0 ,11);
		 }
		 else if(condition == eqsym)
		 {
			 emit(2, 0 ,8);
		 }
		 else if(condition == neqsym)
		 {
			 emit(2, 0 ,9);
		 }
		 
	 }
	 
	 return;
 }
 void expression()
 {
	 int addop;
	 
	 if((token == plussym) || (token == minussym))
	 {
		 addop = token;
		 getToken();
		 term();
		 if(addop == minussym)
		 {
			 emit(2, 0 ,1);
		 }
	 }else
	 {
	 term();
	 }
	 while ((token == plussym) || (token == minussym))
	 { 
		 addop = token;
		 getToken();
		 term();
		 if(addop == plussym){
			 emit(2,0,2);//addition
		 }else
		 {
			 emit(2,0,3);//subtraction
		 }
	}
	
	return;

 }
void term()
{
	int mulop;
	factor();
	while ((token == multsym) || (token == slashsym))
	{		
		mulop = token;
		getToken();
		factor();
		if(mulop == multsym){
			emit(2,0,4); //multiplication
		}else
		{
			emit(2,0,5); //division
		}
	}
	
	return;
}
void factor(){
	 
	 if (token == identsym){
		 int i = find(buff[tokPointer]);
		 if(i == -1)
		 {
			error(11);
		 }
		 if(symbolTable[i].kind == 2)
		 {
			 emit(3, symbolTable[i].level, symbolTable[i].m);
		 }else if(symbolTable[i].kind == 1)
		 {
			 emit(1, 0 , symbolTable[i].m);
		 }
		 else
		 {
			 error(11);
		 }
		 getToken();
	 } 
	 else if(token == numbersym)
	 {
		emit(1, 0, atoi(buff[tokPointer]));
		getToken();
	 } 
	 else if (token == lparentsym)
	 {
		getToken();
		expression();
		if(token != rparentsym)
		{
			error(22);
		}
		getToken();
	 } 
	 else
	 {
		 error(24);
	 }
	 
	 return;
}
//Function to emit code to code array, which will later be printed to the output file
void emit(int op, int l, int m)
{
	//Check if cx is greater then Max_Code_Length
	if(cx > MAX_CODE_LENGTH)
	{
		//error message for cx exceeded code length
		error(29);
	}
	else
	{
		code[cx].op = op;   // opcode
		code[cx].l = l; 	// lexicographical level
		code[cx].m = m; 	// modifier
		cx++;
	}
}
//dynamicall allocates space for symbols in symbolTable
void defineTable( struct symbol **tableStruct)
{
	//dynamic allocation
	*tableStruct = malloc(RMAX * sizeof *tableStruct);
	
}
//Function to check if symbol is in the symbolTable
int find(char *str)
{
	int i;
	//Iteration through table to locate symbol
	for(i = 0; i < RMAX; i++){
		if(strcmp(symbolTable[i].name, str) == 0 )
		{
			//i is the position of the symbol in the symbol array.
			return i;
		}
	}
	//return -1 if symbol not found in table
	return -1;
}
//Function error takes in value and switches on it and will print the corresponding error message
void error(int errorNum){

    switch( errorNum){

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
             printf("const, var, procedure must be followed by identifier.\n");
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
            printf("Undeclared identifier.n");
            break;
        case 12:
            printf("Assignment to constant or procedure is not allowed.\n");
            break;
        case 13:
            printf("Assignment operator expected.\n");
            break;
        case 14:
            printf("call must be followed by an identifier.\n");
            break;
        case 15:
            printf("Call of a constant or variable is meaningless.\n");
            break;
        case 16:
            printf("then expected.\n");
            break;
        case 17:
            printf("Semicolon or } expected.\n");
            break;
        case 18:
            printf("do expected.\n");
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
            printf("This number is too large\n");
            break;
		case 26:
            printf("Read or Write must be followed by an identifier.\n");
            break;
		case 27:
            printf("End symbol was espected.\n");
            break;
		case 28: 
			printf("Identifier was espected.\n");
			break;
		case 29: 
			printf("Error code to long.\n");
			break;
		
        default:
            break;

        }
		
		//exit as error was found in code
		exit(EXIT_FAILURE);
		
		return;
}
//Function prints the code generated to the output file specified in command line arguments
void pm0ToFile(FILE* outputPM0){
	
	int i;
	
	for(i = 0; i < cx; i++){
		
		switch(code[i].op){
			
			//LIT
			case 1:
				fprintf(outputPM0,"1\t%d\t%d\n", code[i].l, code[i].m);
				break;
			//OPR
			case 2:
			
				switch(code[i].m){
					
						//RET
						case 0:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
						//NEG
						case 1:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
						//ADD
						case 2:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
						//SUB
						case 3:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
						//MUL
						case 4:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
						//DIV
						case 5:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
						//ODD
						case 6:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
						//MOD
						case 7:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
						//EQL
						case 8:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
						//NEQ
						case 9:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
						//LSS
						case 10:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
						//LEQ
						case 11:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
						//GTR
						case 12:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
						//GEQ
						case 13:
							fprintf(outputPM0,"2\t%d\t%d\n", code[i].l, code[i].m);
							break;
				}
				
				break;
			//LOD
			case 3:
				fprintf(outputPM0,"3\t%d\t%d\n", code[i].l, code[i].m);
				break;
			//STO
			case 4:
				fprintf(outputPM0,"4\t%d\t%d\n", code[i].l, code[i].m);
				break;
			//CAL
			case 5:
				fprintf(outputPM0,"5\t%d\t%d\n", code[i].l, code[i].m);
				break;
			//INC
			case 6:
				fprintf(outputPM0,"6\t%d\t%d\n", code[i].l, code[i].m);
				break;
			//JMP
			case 7:
				fprintf(outputPM0,"7\t%d\t%d\n", code[i].l, code[i].m);
				break;
			//JPC
			case 8:
				fprintf(outputPM0,"8\t%d\t%d\n", code[i].l, code[i].m);
				break;
			//SIO
			case 9:
			
				switch(code[i].m){
					
					//OUT
					case 0:
						fprintf(outputPM0,"9\t%d\t%d\n", code[i].l, code[i].m);
					break;
					//INP
					case 1:
						fprintf(outputPM0,"9\t%d\t%d\n", code[i].l, code[i].m);
					break;
					//HLT
					case 2:
						fprintf(outputPM0,"9\t%d\t%d\n", code[i].l, code[i].m);
					break;
				}
				break;
		}
	}
	return;
}