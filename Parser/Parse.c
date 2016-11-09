#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parse.h"
#include "Lexer.h"
 
#define MAX 1000

typedef struct symbol {
 int kind; // const = 1, var = 2, proc = 3
 char name[12]; // name up to 11 chars
 int val; // number (ASCII value)
 int level; // L level
 int addr; // M address
} symbol;

//Global token array to store tokens.
static int Tokens[MAX];
//Global string array to store identfiers and digits.
static char buff[MAX][13];
//The max value of tokens will be stored here once computed.
static int RMAX;
//Global token pointer to know which token we are currently on.
static int tokPointer = -1;
//Current Token
static int token;
//Count to determine if errors were found in code.
static int errorFound = 0;
//Declare sysmbol table.
static struct symbol table[MAX];

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

void PrintToks(){
    int i;
    for(i = 0; i < RMAX; i++){
        printf("Value Tokens[%d]: = %d      Buff[%d]: = %s\n", i,Tokens[i],i,buff[i]);
    }

    return;
}
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
        default:
            break;

        }
		printf("Current Tok = %d", Tokens[tokPointer]);
		fflush(stdout);
		exit(EXIT_FAILURE);
		//Increment error numbers.
		//errorFound++;
		//Decreases token despite errors to continue checking for more.
		//tokPointer--;
		
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

    return;

}
void enter(){
	
	
	
	
	
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
void program()
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
		printf("No errors, program is syntactically correct\n");
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
		
		 do{
			getToken();
			if (token != identsym)
			{
			
				error(28);
			} 
			getToken();
	
		 }while(token == commasym);
		 
		 if(token != semicolonsym)
		 {
			 error(5);
		 } 
			getToken();
	 }
	 while (token == procsym)
	 {
		getToken();
		if (token != identsym)
		{
			error(28);
		}
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
		 getToken();
		 if (token != becomessym)
		 {
			error(13);
		 }
		 getToken();
		 expression();
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
			getToken();
			statement();
		}
	 else if (token == whilesym)
		{	
			 getToken();
			 condition();
			 if (token != dosym)
				{			 
				error(18);
				}
			getToken();
			statement();
		}
		else if(token == readsym)
		{
			getToken();
			if(token != identsym)
			{
				error(26);
			}
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
	 if(token == oddsym){ 
		getToken();
		expression();
	 }
	 else
	 {
		 expression();
		 if(!((token == lessym) || (token == gtrsym) || (token == geqsym) || (token == leqsym) || (token == eqsym) || (token == neqsym)))
		 {
			 error(20);
		 }
		 getToken();
		 expression();
	 }
	 
	 return;
 }
 void expression()
 {
	 if((token == plussym) || (token == minussym))
	 {
		 getToken();
	 }
	 term();
	 while ((token == plussym) || (token == minussym))
	 { 
		 getToken();
		 term();
	}
	
	return;

 }
void term()
{
	factor();
	while ((token == multsym) || (token == slashsym))
	{		
		getToken();
		factor();
	}
	
	return;
}
void factor(){
	 
	 if (token == identsym){
		 getToken();
	 } 
	 else if(token == numbersym)
	 {
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


