#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Parse.h"
#include "Lexer.h"

typedef struct symbol {
 int kind; // const = 1, var = 2, proc = 3
 char name[12]; // name up to 11 chars
 int val; // number (ASCII value)
 int level; // L level
 int addr; // M address
} symbol;

// String array of the four error types
static char* errors[] = {"Error: identifier does not start with a letter", 
                         "Error: identifier too long", 
                         "Error: number greater than 2^16-1",
                         "Error: invalid token",
                     	 "Empty file given"};


int main(int argc, char *argv[])
{	
	if(argc != 3)
	{
		printf("Incorrect number of arguments.");
		return 0;
	}
		
	FILE *inputFile = fopen(argv[1], "r");
	FILE *outputFile = fopen(argv[2], "w");
	
	int errorFlag = lex(inputFile);

	if(errorFlag >= 0)
	{
		printf("%s\n", errors[errorFlag]);
		return 0;
	}
	
	FILE* Tokens = fopen("TokenFormat.txt", "r");
	PlaceToks();
	PrintToks();
	program();
	
	fclose(inputFile);
	fclose(outputFile);
	
	return 0;
	
}