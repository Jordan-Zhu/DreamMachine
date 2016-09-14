%{
#include <string.h>
#include "util.h"
#include "tokens.h"
#include "errormsg.h"

int yywrap(void) {
 return 1;
}

void newline(void) {
  EM_line++;
  EM_col = 1;
}

void adjust(void) {
  EM_col += yyleng;
}

%}

id [a-zA-Z][a-zA-Z0-9_]*
%Start COMMENT
%%
<INITIAL>" "         {adjust(); continue;}
<INITIAL>\n          {adjust(); newline(); continue;}
<INITIAL>"+"         {adjust(); return plussym;}
<INITIAL>"-"         {adjust(); return minussym;}
<INITIAL>"*"         {adjust(); return multsym;}
<INITIAL>"/"         {adjust(); return slashsym;}
<INITIAL>"odd"       {adjust(); return oddsym;}
<INITIAL>"="         {adjust(); return eqsym;}
<INITIAL>"<>"        {adjust(); return neqsym;}
<INITIAL>"<="        {adjust(); return leqsym;}
<INITIAL>"<"         {adjust(); return lessym;}
<INITIAL>">="        {adjust(); return geqsym;}
<INITIAL>">"         {adjust(); return gtrsym;}
<INITIAL>"("         {adjust(); return lparentsym;}
<INITIAL>")"         {adjust(); return rparentsym;}
<INITIAL>","         {adjust(); return commasym;}
<INITIAL>";"         {adjust(); return semicolonsym;}
<INITIAL>"."         {adjust(); return periodsym;}
<INITIAL>":="        {adjust(); return becomessym;}
<INITIAL>"begin"     {adjust(); return beginsym;}
<INITIAL>"end"       {adjust(); return endsym;}
<INITIAL>"if"        {adjust(); return ifsym;}
<INITIAL>"then"      {adjust(); return thensym;}
<INITIAL>"while"     {adjust(); return whilesym;}
<INITIAL>"do"        {adjust(); return dosym;}
<INITIAL>"call"      {adjust(); return callsym;}
<INITIAL>"const"     {adjust(); return constsym;}
<INITIAL>"var"       {adjust(); return varsym;}
<INITIAL>"procedure" {adjust(); return procsym;}
<INITIAL>"write"     {adjust(); return writesym;}
<INITIAL>[0-9]+      {adjust(); yylval.ival=atoi(yytext); return numbersym;}
<INITIAL>{id}        {adjust(); yylval.sval=String(yytext); return identsym;}
<INITIAL>"/*"        {adjust(); BEGIN COMMENT;}
<INITIAL>.           {adjust(); EM_error("illegal token");}

<COMMENT>"*/"        {adjust(); BEGIN INITIAL;}
<COMMENT>.           {adjust();}
