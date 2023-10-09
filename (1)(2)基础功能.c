%{
/*********************************************
YACC file
基础程序
Date:2023/9/19
forked SherryXiye
**********************************************/
#include<stdio.h>
#include<stdlib.h>
#ifndef YYSTYPE
#define YYSTYPE double
#endif
int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char* s);
%}
//注意先后定义的优先级区别
%token ADD SUB MUL DIV NUMBER LKO RKO
%left LKO  
%left ADD SUB
%left MUL DIV
%right UMINUS
%right RKO


%%

lines :  lines expr ';' {printf("%g\n",$2);}
      |  lines ';'
      |
      ;

expr  :   expr ADD expr {$$=$1+$3;}
      |  expr SUB expr {$$=$1-$3;}
      |  expr MUL expr {$$=$1*$3;}
      |  expr DIV expr {$$=$1/$3;}
      |  LKO expr RKO  {$$=$2;}
      |  SUB expr %prec UMINUS {$$=-$2;}
      |  NUMBER 
      ;
%%


// programs section

int yylex()
{
    int c = getchar();
    while (c == ' ' || c == '\t' || c == '\n') {
        c = getchar(); // 跳过空白字符
    }
    if(c >= '0' && c <= '9'){
        yylval = c - '0';
        c = getchar();
        while(c >= '0' && c <= '9'){
            yylval = yylval * 10 + c -'0';
            c = getchar();
        }
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
    yyin=stdin;
    do{
        yyparse();
    }while(!feof(yyin));
    return 0;
}
void yyerror(const char* s){
    fprintf(stderr,"Parse error: %s\n",s);
    exit(1);
}