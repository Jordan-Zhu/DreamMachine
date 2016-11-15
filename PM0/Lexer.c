//Nicholas Deleuze
//Alex Phillips
//Jamal Roberts
//Jordan Zhu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Lexer.h"


// String array of reserved words and their respective integer identifiers
static char* reserved[] = {"const", "var", "procedure", "call", "begin", "end", "if", "then",
                          "else", "while", "do", "read", "write", "odd"};
static int reservedTokens[14] = {28, 29, 30, 27, 21, 22, 23, 24, 33, 25, 26, 32, 31, 8};


// String array of arithmatic tokens and their respective integer identifiers
static char* arith[] = {"+", "-", "*", "/", "=", "<>", "<", "<=", ">", ">=", "(", ")", ",", ";", ".", ":="};
static int arithTokens[16] = {4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

// String array of the four error types
static char* errors[] = {"Error: identifier does not start with a letter", 
                         "Error: identifier too long", 
                         "Error: number greater than 2^16-1",
                         "Error: invalid token"};


// Function to create a cleaned File without comments from input file
char clean(FILE *input, FILE *cleanFile)
{
	// Checkes if next char is a star, if not moves pointer two chars back and returns '/'
	char star = fgetc(input);
	if(star != '*')
	{
		fseek(input, -2, SEEK_CUR);
		char c = fgetc(input);
		return c;
	}
	
	// Prints two spaces if star is found after backslash
	fprintf(cleanFile, "  ");
	
	// Replaces star/backslash and everything in between comments with a space
	char holder1 = fgetc(input);
	char holder2;
	while(1)
	{
		holder2 = fgetc(input);
		if(holder1 == '*')
		{
			if(holder2 == '/')
			{
				fprintf(cleanFile, "  ");
				break;
			}
		}
		fprintf(cleanFile, " ");
		holder1 = holder2;
	}

	// Returns next token after comment
	return fgetc(input) ;
}

// Returns 1 if reserved word found, adds reserved word to the tokenFile, 0 otherwise
int checkReserved(char word[], FILE* tokenFile)
{
	int i;
	for(i = 0; i < 14; i++)
	{
		if(!strcmp(word, reserved[i]))
		{
			fprintf(tokenFile,"%s\t\t%d\n", word, reservedTokens[i]);
			return 1;
		}
	}

	return 0;
}

// Returns 1 if arithmatic token found, adds token to the tokenFile, 0 otherwise
int checkArith(char word[], FILE* tokenFile)
{
	int i;
	for(i = 0; i < 16; i++)
	{
		if(!strcmp(word, arith[i]))
		{
			fprintf(tokenFile,"%s\t\t%d\n", word, arithTokens[i]);
			return 1;
		}
	}

	return 0;
}
void TokenFormat(FILE *f)
{     
	FILE *tokenf = fopen("TokenFormat.txt", "w");
    char word[12];
    int val;
	int i;
	
    while( fscanf(f, "%s", word) != EOF)
	{   
		fscanf(f,"%d", &val);
	
        if((val == 3) || (val == 2))         
		{             
			fprintf(tokenf,"%d", val);
			fprintf(tokenf," ");
			fprintf(tokenf,"%s", word);
			fprintf(tokenf," ");
		}         
		else         
		{             
			fprintf(tokenf,"%d", val);
            fprintf(tokenf," ");
		}     
	}     
	fclose(tokenf);
    return;
}

// Function to tokenize every string from the input file, and add repsective tokens to tokenFile
int tokenize(char word[], FILE* tokenFile)
{
	// Checks if word is reserved or arithmatic
	if(checkReserved(word, tokenFile))
		return -1;
	
	if(checkArith(word, tokenFile))
		return -1;

	// If not reserved/arithmatic, splits word into substrings of (letters and numbers) or (symbols)
	int i = 0;
	int j;
	int buffPos = 0;
	int strPos = 0;
	char buffs[100][100];
	char tempStr[3];
	int errorFlag = -1;
	

	while(i < strlen(word))
	{   
		// Builds buffer from letter/number string
	    while(i < strlen(word) && (isalpha(word[i]) || isdigit(word[i])))
	    {
	      	buffs[buffPos][strPos] = word[i];
	      	strPos++;
	      	i++;
	    }
	    
	    if(strPos > 0)
	    {
	    	buffs[buffPos][strPos] = '\0';
	      	buffPos++;
			strPos = 0;      
	    }
	    
	    // Builders buffer from symbol string
	    while(i < strlen(word) && !isalpha(word[i]) && !isdigit(word[i]))
	    {
		    buffs[buffPos][strPos] = word[i];
		    strPos++;
		    i++;
	    }	

	    if(strPos > 0)
	    {
	    	buffs[buffPos][strPos] = '\0';
	      	buffPos++;
			strPos = 0;      
	    }  
	}

	// Checks for more reserved/arithmatic tokens, and for errors
	for(i = 0; i < buffPos; i++)
	{	
		// If string does not start with a number
		if(!isdigit(buffs[i][0]))
		{
			// If reserved/arithmatic add to tokenFile and move onto next substring
			if(checkReserved(buffs[i], tokenFile))
				continue;
			if(checkArith(buffs[i], tokenFile))
				continue;
			
			

			// If substring does not start with number or letter and is not reserved/arithmatic, 
			// must be an invalid token, error thrown. Move to next substring 
			if(!isalpha((buffs[i][0])))
			{
				for(j = 0; j < strlen(buffs[i]); j++)
				{
					if(buffs[i][j] == ':')
					{
						if(j + 1 < strlen(buffs[i]) && buffs[i][j+1] == '=')
						{
							tempStr[0] = buffs[i][j];
							tempStr[1] = buffs[i][j+1];
							tempStr[2] = '\0';
							checkArith(tempStr, tokenFile);
							j++;
							continue;
						}
					}
					else if(buffs[i][j] == '<')
					{
						if(j + 1 < strlen(buffs[i]) && buffs[i][j+1] == '=')
						{
							tempStr[0] = buffs[i][j];
							tempStr[1] = buffs[i][j+1];
							tempStr[2] = '\0';
							checkArith(tempStr, tokenFile);
							j++;
							continue;
						}
						else if(j + 1 < strlen(buffs[i]) && buffs[i][j+1] == '>')
						{
							tempStr[0] = buffs[i][j];
							tempStr[1] = buffs[i][j+1];
							tempStr[2] = '\0';
							checkArith(tempStr, tokenFile);
							j++;
							continue;
						}
					}
					else if(buffs[i][j] == '>')
					{
						if(j + 1 < strlen(buffs[i]) && buffs[i][j+1] == '=')
						{
							tempStr[0] = buffs[i][j];
							tempStr[1] = buffs[i][j+1];
							tempStr[2] = '\0';
							checkArith(tempStr, tokenFile);
							j++;
							continue;
						}
					}

					tempStr[0] = buffs[i][j];
					tempStr[1] = '\0';
					tempStr[2] = '\0';

					if(!checkArith(tempStr, tokenFile))
						return 3;
				}
			}
			else if(isalpha((buffs[i][0])))
			{
				// I variable name larger than 12 characters, error thrown. Move to next substring
				if(strlen(buffs[i]) > 12)
				{
					return 1;
				}

				// Add valid variable name to tokenFile
				fprintf(tokenFile,"%s\t\t%d\n", buffs[i], 2);
			}
		}

		// If string starts with a number
		if(isdigit(buffs[i][0]))
		{
			// If any letter found in number, error thrown
			for(j = 0; j < strlen(buffs[i]); j++)
			{
				if(isalpha(buffs[i][j]))
				{
					return 0;
				}
			}

			// If number larger than 2^16 - 1, error thrown
			if(atoi(buffs[i]) > 65535)
			{
				return 2;
			}

			// Add valid integer to tokenFile
			fprintf(tokenFile,"%s\t\t%d\n", buffs[i], 3);
		}
		
	}

	return errorFlag;
}

// Creates cleanFile without comments
void run(FILE *input)
{
	// Creates the clean file
  FILE *cleanFile = fopen("CleanOutput.txt", "w");
  
  char holder; 
  
  // create cleanFile
  while(!feof(input))
	{

		holder = fgetc(input);
    
		if(holder == '/')
		{
			holder = clean(input, cleanFile);
		}
		
		if(holder != EOF){
			fprintf(cleanFile,"%c", holder);
		}
	}
  
  // Closes file, and opens file to be read from
  fclose(cleanFile);
  cleanFile = fopen("CleanOutput.txt", "r");
}


int lex(FILE *pl0File)
{
	int errorFlag = -1;
	char buff[1024];
	char holder;
	FILE *input;

	// Creates tokenFile named token.txt to be writen to
	FILE* tokenFile = fopen("tokens.txt", "w");

	// Opens cleanFile to be read
	FILE* cleanFile = fopen("CleanOutput.txt", "r");
	
	run(pl0File);
	
	// Adds null sysbol to tokenFile and ends program if empty file given
	if(fscanf(cleanFile, "%s", buff) == EOF)
	{
		fprintf(tokenFile,"%s\t\t%d\n", "/0", 1);
		return 4; 
	}
	
	// Parses tokens from the beginning of cleanFile
	else
  	{
		rewind(cleanFile);

		while(fscanf(cleanFile, "%s", buff)!= EOF)
		{
			errorFlag = tokenize(buff, tokenFile);

			if(errorFlag >= 0)
				return errorFlag;
		}

		// Closes and opens tokenFile to be read from
		//fclose(tokenFile);
		//tokenFile = fopen("tokens.txt", "r");

		// Prints tokens if no errors found
		// if(errorFlag == 0)
		// {
			// printf("\ntokens:\n");
			// printf("-------\n");
    
		    // while(!feof(tokenFile))
		  	// {
		    	// holder = fgetc(tokenFile);
		    
		    	// if(holder != EOF)
						// printf("%c", holder);
		    
		    // }
		// }
	}
	fclose(tokenFile);
	tokenFile = fopen("tokens.txt", "r");
	TokenFormat(tokenFile);

	return -1;
}
