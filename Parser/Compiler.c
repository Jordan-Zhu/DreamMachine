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

int main(int argc, char *argv[])
{	
	if(argc != 3)
	{
		printf("Incorrect number of arguments.");
		return 0;
	}
		
	FILE *inputFile = fopen(argv[1], "r");
	FILE *outputFile = fopen(argv[2], "w");
	
	char* error = lex(inputFile);
	
	FILE* Tokens = fopen("TokenFormat.txt", "r");
	PlaceToks();
	PrintToks();
	program();
	
	fclose(inputFile);
	fclose(outputFile);
	
	return 0;
	
}