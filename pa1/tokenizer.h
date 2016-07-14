#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct TokenizerT_ {
	char type[30];
	char token[100];
	struct TokenizerT_ *next;
};

typedef struct TokenizerT_ TokenizerT;
TokenizerT* TKCreate( char * ts );
void plus(char *ts, TokenizerT* node);
void and(char * ts, TokenizerT* node);
void word(char * ts, TokenizerT* node);
void zero(char * ts, TokenizerT* node);
void greaterthan(char* ts, TokenizerT* node);
void lessthan(char* ts, TokenizerT* node);
void printtokens(TokenizerT* root);
void minus(char*ts, TokenizerT* node);
void multiply(char*ts, TokenizerT* node);
void divide(char *ts, TokenizerT* node);
void mod(char * ts, TokenizerT* node);
void negate(char * ts, TokenizerT* node);
void or(char *ts, TokenizerT* node);
char *TKGetNextToken( TokenizerT * tk );
void escape(char* ts);
void other(char* ts, TokenizerT* node);
void digit(char*ts, TokenizerT* node);
void ckeywordcmp(TokenizerT* node);
