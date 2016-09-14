#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "util.h"
#include "errormsg.h"

static string fileName = "";

extern FILE *yyin;

int EM_line;
int EM_col;

void EM_newline(void) { }

void EM_error(char *message,...) {
  printf("%s at %d:%d", message, EM_line, EM_col);
}

void EM_reset(string fname) {
  fileName = fname; 
  EM_line = 1;
  EM_col = 1;
  yyin = fopen(fname, "r");
  if (!yyin) {
    EM_error(0, "cannot open"); 
    exit(1);
  }
}

