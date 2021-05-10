/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
    if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
        YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

int comment_count;
int solve();


/*
 *  Add Your own definitions here
 */

%}

/*
 * Define names for regular expressions here.
 */

%x COMMENT STRING

DARROW          =>
LE              <=
ASSIGN          <-

DIGIT           [0-9]
UPPER           [A-Z]
LOWER           [a-z]
ALL             [A-Za-z0-9_]
SPECIAL         [;,:@.(){}+\-*/~<=]
WHITE           [ \f\t\v]

%%

 /*
  *  Nested comments
  */


 /*
  *  The multiple-character operators.
  */
{DARROW}        { return (DARROW); }
{LE}            { return (LE); }
{ASSIGN}        { return (ASSIGN); }  
{SPECIAL}       { return (int)yytext[0]; }

"(*"            {
                    comment_count = 1;
                    BEGIN COMMENT;
                }
             
"*)"            {
                    cool_yylval.error_msg = "Unmatched *)";
                    return ERROR;
                }
             
<COMMENT>"(*"   { comment_count++; }

<COMMENT>"*)"   { 
                    comment_count--;
                    if (!comment_count) 
                        BEGIN 0;
                }
<COMMENT>[\n]   { curr_lineno++; }
<COMMENT>.      { }

<COMMENT><<EOF>>    {
                        cool_yylval.error_msg = "EOF in comment";
                        BEGIN 0;
                        return ERROR;
                    }  
 
 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */

t(R|r)(U|u)(E|e)  {
                    cool_yylval.boolean = 1;
                    return BOOL_CONST;
                  }

f(A|a)(L|l)(S|s)(E|e)  {
                    cool_yylval.boolean = 0;
                    return BOOL_CONST;
                  }

(?i:CLASS)      { return (CLASS); }
(?i:ELSE)       { return (ELSE); }
(?i:FI)         { return (FI); }
(?i:IF)         { return (IF); }
(?i:IN)         { return (IN); }
(?i:INHERITS)   { return (INHERITS); }
(?i:LET)        { return (LET); }
(?i:LOOP)       { return (LOOP); }
(?i:POOL)       { return (POOL); }
(?i:THEN)       { return (THEN); }
(?i:WHILE)      { return (WHILE); }
(?i:CASE)       { return (CASE); }
(?i:ESAC)       { return (ESAC); }
(?i:OF)         { return (OF); }
(?i:NEW)        { return (NEW); }
(?i:LE)         { return (LE); }
(?i:NOT)        { return (NOT); }
(?i:ISVOID)     { return (ISVOID); }
 
 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  *
  */

 /*
  * Well, in the handout they say "Each call on the scanner returns the 
  * next token and lexeme from the input" ??? But it's OK to do nothing ?
  * That's it?
  */
{WHITE}+          { }

\n                { curr_lineno++; }

{UPPER}{ALL}*     {
                    cool_yylval.symbol = idtable.add_string(yytext);
                    return TYPEID;
                  }
   
{LOWER}{ALL}*     {
                    cool_yylval.symbol = idtable.add_string(yytext);
                    return OBJECTID;
                  }   

{DIGIT}+          {
                    cool_yylval.symbol = inttable.add_string(yytext);
                    return INT_CONST;
                  }     

\-\-[^\n]*[\n]       { curr_lineno++; }


\"[^"\n]*["\n]  {
                    if (yytext[yyleng - 1] == '\n')
                        curr_lineno++;
                    if (yytext[yyleng - 2] == '\\'){
                        BEGIN STRING;
                        yymore();
                    }
                    else{ 
                        if (yytext[yyleng - 1] == '\n'){
                            cool_yylval.error_msg = "Unterminated string constant";
                            return ERROR;
                        }
                        else
                            switch(solve()){
                                case 0:
                                    cool_yylval.symbol = stringtable.add_string(string_buf);
                                    return STR_CONST;
                                case 1:
                                    cool_yylval.error_msg = "String contains null character";
                                    return ERROR;
                                case 2:
                                    cool_yylval.error_msg = "String constant too long";
                                    return ERROR;
                            }   
                    }
                }
              
\"[^"\n]*       {
                    cool_yylval.error_msg = "EOF instring constant";
                    return ERROR;
                }
                
<STRING>[^"\n]*["\n]    {
                            if (yytext[yyleng - 1] == '\n')
                                curr_lineno++;
                            if (yytext[yyleng - 2] == '\\'){
                                yymore();
                            }
                            else{
                                BEGIN 0;
                                if (yytext[yyleng - 1] == '\n'){
                                    cool_yylval.error_msg = "Unterminated string constant";
                                    return ERROR;
                                } 
                                else
                                    switch(solve()){
                                        case 0:
                                            cool_yylval.symbol = stringtable.add_string(string_buf);
                                            return STR_CONST;
                                        case 1:
                                            cool_yylval.error_msg = "String contains null character";
                                            return ERROR;
                                        case 2:
                                            cool_yylval.error_msg = "String constant too long";
                                            return ERROR;
                                    }   
                            }
                        }

<STRING>[^"\n]*     {
                        cool_yylval.error_msg = "EOF instring constant";
                        return ERROR;
                    }
                    
                    

.                 {
                    cool_yylval.error_msg = yytext;
                    return ERROR;
                  }        

%%

int solve(){
    int count = 0;
    for (int i = 1; i < yyleng - 1; ++i){
        if (yytext[i] == '\0')
            return 1;
        if (yytext[i] == '\\'){
            i++;
            switch(yytext[i]){
                case 'b':
                    string_buf[count++] = '\b';
                    break;
                case 't':
                    string_buf[count++] = '\t';
                    break;
                case 'n':
                    string_buf[count++] = '\n';
                    break;
                case 'f':
                    string_buf[count++] = '\f';
                    break;
                default:
                    string_buf[count++] = yytext[i];
            }
        }
        else{
            string_buf[count++] = yytext[i];
        }
        if (count == 1024)
            return 2;
    }
    string_buf[count] = '\0';
    return 0;
}