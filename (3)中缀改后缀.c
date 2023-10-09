%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define YYSTYPE char*
char num[100];
int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char*s);
%}
%token NUMBER
%token ADD SUB MUL DIV LKO RKO
%left LKO
%left ADD SUB
%left MUL DIV
%right UMINUS
%right RKO
%%

lines : lines expr '\n'{printf("%s\n",$2);}
      | lines '\n'
      |
      ;

expr : expr ADD expr {$$=(char*)malloc(100*sizeof(char));
                      strcpy($$,$1);
                      strcat($$,$3);strcat($$,"+"); }
     | expr SUB expr {$$=(char*)malloc(100*sizeof(char)); 
                      strcpy($$,$1);
                      strcat($$,$3);strcat($$,"-"); }
     | expr MUL expr {$$=(char*)malloc(100*sizeof(char)); 
                      strcpy($$,$1);
                      strcat($$,$3);strcat($$,"*"); }
     | expr DIV expr {$$=(char*)malloc(100*sizeof(char)); 
                      strcpy($$,$1);
                      strcat($$,$3);strcat($$,"/"); }
     | LKO expr RKO {$$=(char*)malloc(100*sizeof(char));
                      strcpy($$,$2);}
     | NUMBER   {$$ = (char*)malloc(100*sizeof(char)); 
                 strcpy($$,$1);
                 strcat($$," ");}


%%


int yylex()
{
    int c = getchar();
    while (c == ' ' || c == '\t') {
        c = getchar(); // 跳过空白字符
    }
    if(c >= '0' && c <= '9'){
        num[0] = c;
        c = getchar();
        int i = 1;
        while(c >= '0' && c <= '9'){
            num[i] = c;
            c = getchar();
            i++;
        }
        num[i] = '\0';
        yylval = num;
        ungetc(c,stdin);
        return NUMBER;
    }
    if(c == '+')
        return ADD;
    if(c == '-')
        return SUB;
    if(c == '*')
        return MUL;
    if(c == '/')
        return DIV;
    if(c == '(')
        return LKO;
    if(c == ')')
        return RKO;
}

int main(void)
{
   yyin = stdin;
  do{
      yyparse();
    }while(!feof(yyin));
  return 0;
}
void yyerror(const char* s)
{
   fprintf(stderr,"Parse error:%s\n",s);
   exit(1);
}
