#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "errormsg.h"
#include "tokens.h"

YYSTYPE yylval;

int main(int argc, char* argv[]) {
  char* fname; 
  int tok;
  
  if (argc != 2) {
    fprintf(stderr, "usage: a.out filename\n"); 
    exit(1);
  }
  fname = argv[1];

  EM_reset(fname);

  for(;;) {
    tok = yylex();
    if (tok == 0) break;
    switch (tok) {
    case identsym :
      printf("identsym\n");
      printf("%d:%d %3d %s\n", EM_line, EM_col, tok, yylval.sval);
      break;
    case numbersym :
      printf("numbersym\n");
      printf("%d:%d %3d %d\n", EM_line, EM_col, tok, yylval.ival);
      break;
    default :
      printf("default\n");
      printf("%d:%d %3d \n", EM_line, EM_col, tok);
    }
  }
  return 0;
}


