#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokenizer.h"
	int i=0;
	int j;
	int tokencount=0;
	int numtokens=100;
	TokenizerT *root;
	TokenizerT *node;

/*main accepts a string of chars
 *  checks for correct number of arguments
 *  mallocs root node and calls TKCreate*/
int main(int argc, char** argv){



	if(argc != 2){
		fprintf(stderr, "Invalid number of arguments\n");
		return 1;
	}

	 root = malloc(sizeof(TokenizerT));
	 node = malloc(sizeof(TokenizerT));
	 root->next = node;
	 node->next = NULL;

	 int len = strlen(argv[1]);
	 char str[len];
	 strcpy(str, argv[1]);
	 str[len]= '\0';
	 TKCreate(str);
return 0;
}

/*TKCreate takes in a string and goes through it
 * to call functions and create tokens*/
TokenizerT* TKCreate( char * ts ){
	while(ts[i] != '\0'){

		int num = ts[i];


		if(ts[i] == '0'){
			zero(ts, node);
			node->next = malloc(sizeof(TokenizerT));
			node = node->next;
			node->next = NULL;
			i--;
		}
		else if(ts[i]>= '1' && ts[i]<= '9'){
			digit(ts, node);
			node->next = malloc(sizeof(TokenizerT));
			node = node->next;
			node->next = NULL;
			i--;
		}
		else if((ts[i] >= 'a' && ts[i] <= 'z') || (ts[i] >= 'A' && ts[i] <= 'Z')){
			word(ts, node);
			ckeywordcmp(node);
			node->next = malloc(sizeof(TokenizerT));
			node = node->next;
			node->next = NULL;
			i--;
		}
		else if(ts[i] == '>'){
			greaterthan(ts, node);
			node->next = malloc(sizeof(TokenizerT));
			node = node->next;
			node->next = NULL;
			i--;
		}
		else if(ts[i] == '<'){
			lessthan(ts, node);
			node->next = malloc(sizeof(TokenizerT));
			node = node->next;
			node->next = NULL;
			i--;
		}
		else if(ts[i] == '&'){
			and(ts, node);
			node->next = malloc(sizeof(TokenizerT));
			node = node->next;
			node->next = NULL;
			i--;
		}
		else if(ts[i] == '+'){
			plus(ts, node);
			node->next = malloc(sizeof(TokenizerT));
			node = node->next;
			node->next = NULL;
			i--;
		}
		else if(ts[i] == '-'){
				minus(ts, node);
				node->next = malloc(sizeof(TokenizerT));
				node = node->next;
				node->next = NULL;
				i--;
			}
			else if(ts[i] == '*'){
				multiply(ts, node);
				node->next = malloc(sizeof(TokenizerT));
				node = node->next;
				node->next = NULL;
				i--;
			}
			else if(ts[i] == '/'){
				divide(ts, node);
				node->next = malloc(sizeof(TokenizerT));
				node = node->next;
				node->next = NULL;
				i--;
			}
			else if(ts[i] == '!'){
				negate(ts, node);
				node->next = malloc(sizeof(TokenizerT));
				node = node->next;
				node->next = NULL;
				i--;
			}
			else if(ts[i] == '%'){
				mod(ts, node);
				node->next = malloc(sizeof(TokenizerT));
				node = node->next;
				node->next = NULL;
				i--;
			}
			else if(ts[i] == '|'){
				or(ts, node);
				node->next = malloc(sizeof(TokenizerT));
				node = node->next;
				node->next = NULL;
				i--;
			}
			else if(num == 0x20 || num == 0x0b || num == 0x0c|| num == 0x0d || num == 0x09 || num == 0x0a){

			}
			else if((num >= 0x00 && num <= 0x1f) || num == 0x7f){
				escape(ts);
			}
			else{
				other(ts, node);
				node->next = malloc(sizeof(TokenizerT));
				node = node->next;
				node->next = NULL;
				i--;
			}
		i++;
	}
		printtokens(root);

  return node;

}

void escape(char* ts){
	int num = ts[i];
	fprintf(stderr, "Error: Escape Char [%02x]\n", num);
	return;
}




void minus(char*ts, TokenizerT* node){
	int cnt = 0;
	node->token[cnt] = '-';
	cnt++;
	i++;
	if(ts[i] == '='){
		node->token[cnt]= '=';
		strcpy(node->type, "minus equals");
		cnt++;
		i++;
	}
	else if(ts[i]=='-'){
		node->token[cnt]= '-';
		strcpy(node->type, "decrement");
		cnt++;
		i++;
	}
	else if(ts[i] == '>'){
		node->token[cnt] = '>';
		strcpy(node->type, "structure pointer");
		cnt++;
		i++;
	}
	else{
		strcpy(node->type, "minus");
	}
	node->token[cnt] = '\0';
  return;
}

void digit(char*ts, TokenizerT* node){
	int cnt = 0;
	int count2=1;
		node->token[cnt] = ts[i];
		strcpy(node->type, "integer");
		cnt++;
		i++;
		while(ts[i] >= '0' && ts[i] <= '9'){
			node->token[cnt] = ts[i];
			cnt++;
			i++;
			count2++;

		}
		if(ts[i] == '.'){
				int count=1;
				int enter = 0;
				int enter2=0;
				strcpy(node->type, "float");
				node->token[cnt]= '.';
				cnt++;
				i++;
				count++;
				if(ts[i] >= '0' && ts[i] <= '9' && enter2==0){
					while((ts[i] >= '0' && ts[i] <= '9')){
						node->token[cnt] = ts[i];
						cnt++;
						i++;
						count++;
						enter =1;
					}
				}
				else{
					node->token[cnt-1] = '\0';
					strcpy(node->type, "integer");
					i--;
					return;
				}
				if(ts[i] == 'e' && enter == 1){
					node->token[cnt] = 'e';
					cnt++;
					i++;
					count++;
					enter2 = 1;
				}
				if(enter2==1){
					if(ts[i] >= '0' && ts[i] <= '9'){
						while((ts[i] >= '0' && ts[i] <= '9')){
							node->token[cnt] = ts[i];
							cnt++;
							i++;
						}
					}
					else{
						node->token[cnt-1] = '\0';
						i--;
						return;
					}
				}
		}
	node->token[cnt] = '\0';
	return;

}

void multiply(char*ts, TokenizerT* node){
	int cnt = 0;
	node->token[cnt] = '*';
	cnt++;
	i++;
	if(ts[i] == '='){
		node->token[cnt]= '=';
		strcpy(node->type, "multiply equals");
		cnt++;
		i++;
	}
	else{
		strcpy(node->type, "multiply");
	}
	node->token[cnt] = '\0';
return;
}



void divide(char *ts, TokenizerT* node){
	int cnt = 0;
	node->token[cnt] = '/';
	cnt++;
	i++;
	if(ts[i] == '='){
		node->token[cnt]= '=';
		strcpy(node->type, "divide equals");
		cnt++;
		i++;
	}
	else{
		strcpy(node->type, "divide");
	}
	node->token[cnt] = '\0';
return;
}

void or(char *ts, TokenizerT* node){
	int cnt=0;
	node->token[cnt]= '|';
	cnt++;
	i++;
	if(ts[i]== '|'){
		strcpy(node->type, "logical or");
		node->token[cnt] = '|';
		cnt++;
		i++;
	}
	else if(ts[i] == '='){
		strcpy(node->type, "or assignment");
		node->token[cnt] = '=';
		cnt++;
		i++;
	}
	else{
		strcpy(node->type, "bitwise or");
	}
	node->token[cnt]= '\0';
return;
}



void negate(char * ts, TokenizerT* node){
	int cnt = 0;
	node->token[cnt] = '!';
	cnt++;
	i++;
	if(ts[i] == '='){
		node->token[cnt]= '=';
		strcpy(node->type, "not equal");
		cnt++;
		i++;
	}
	else{
		strcpy(node->type, "negate");
	}
	node->token[cnt] = '\0';
return;
}


void mod(char * ts, TokenizerT* node){
	int cnt = 0;
	node->token[cnt] = '%';
	cnt++;
	i++;
	if(ts[i] == '='){
		node->token[cnt]= '=';
		strcpy(node->type, "modulus equal");
		cnt++;
		i++;
	}
	else{
		strcpy(node->type, "modulus");
	}
	node->token[cnt] = '\0';
return;
}




void plus(char *ts, TokenizerT* node){
	int cnt = 0;
	node->token[cnt] = '+';
	cnt++;
	i++;
	if(ts[i] == '='){
		node->token[cnt]= '=';
		strcpy(node->type, "plus equals");
		cnt++;
		i++;
	}
	else if(ts[i]=='+'){
		node->token[cnt]= '+';
		strcpy(node->type, "increment");
		cnt++;
		i++;
	}
	else{
		strcpy(node->type, "plus");
	}
	node->token[cnt] = '\0';
return;
}

void and(char * ts, TokenizerT* node){
	int cnt=0;
	node->token[cnt]= '&';
	cnt++;
	i++;
	if(ts[i]== '&'){
		strcpy(node->type, "logical and");
		node->token[cnt] = '&';
		cnt++;
		i++;
	}
	else if(ts[i] == '='){
		strcpy(node->type, "and assignment");
		node->token[cnt] = '=';
		cnt++;
		i++;
	}
	else{
		strcpy(node->type, "bitwise and");
	}
	node->token[cnt] = '\0';
return;
}


void word(char * ts, TokenizerT* node){
	int cnt = 0;
	strcpy(node->type, "word");
	while((ts[i] >= 'a' && ts[i] <= 'z') || (ts[i] >= 'A' && ts[i] <= 'Z')){
		node->token[cnt] = ts[i];
		cnt++;
		i++;
	}


	node->token[cnt] = '\0';
return;
}

void zero(char * ts, TokenizerT* node){

	int cnt=0;
	node->token[cnt]= '0';
	cnt++;
	i++;


	if(ts[i] == 'x'){
		strcpy(node->type, "hexidecimal");
		node->token[cnt]= 'x';
		cnt++;
		i++;
		if((ts[i]>= '0' && ts[i] <= '9') || (ts[i] >= 'a' && ts[i] <= 'f') || (ts[i] >= 'A' && ts[i] <= 'F')){
			while((ts[i] >= '0' && ts[i] <= '9') || (ts[i] >= 'a' && ts[i] <= 'f') || (ts[i] >= 'A' && ts[i] <= 'F')){
				node->token[cnt] = ts[i];
				cnt++;
				i++;
			}
		}
		else{
			node->token[cnt-1] = '\0';
			strcpy(node->type, "integer");
			i--;
			return;
		}
	}
	else if(ts[i] == '.'){
		int count=1;
		int enter = 0;
		int enter2=0;
		strcpy(node->type, "float");
		node->token[cnt]= '.';
		cnt++;
		i++;
		count++;
		if(ts[i] >= '0' && ts[i] <= '9' && enter2==0){
			while((ts[i] >= '0' && ts[i] <= '9')){
				node->token[cnt] = ts[i];
				cnt++;
				i++;
				count++;
				enter =1;
			}
		}
		else{
			node->token[cnt-1] = '\0';
			strcpy(node->type, "integer");
			i--;
			return;
		}
		if(ts[i] == 'e' && enter == 1){
			node->token[cnt] = 'e';
			cnt++;
			i++;
			count++;
			enter2 = 1;
		}
		if(enter2==1){
			if(ts[i] >= '0' && ts[i] <= '9'){
				while((ts[i] >= '0' && ts[i] <= '9')){
					node->token[cnt] = ts[i];
					cnt++;
					i++;
				}
			}
			else{
				node->token[cnt-1] = '\0';
				i--;
				return;
			}
		}


	}
	else if(ts[i] >= '0' && ts[i] <= '7'){
		strcpy(node->type, "octal");
		while(ts[i] >= '0' && ts[i] <= '7'){
			node->token[cnt] = ts[i];
			cnt++;
			i++;
		}



	}
	else{
		strcpy(node->type, "integer");
	}
	node->token[cnt] = '\0';

return;
}

void greaterthan(char* ts, TokenizerT* node){
	int cnt=0;
	node->token[cnt]= '>';
	cnt++;
	i++;
	if(ts[i] == '>'){
		node->token[cnt]= '>';
		cnt++;
		i++;
		if(ts[i]== '='){
			node->token[cnt]= '=';
			strcpy(node->type, "RS&assignment");
			cnt++;
			i++;
		}
		else{
			strcpy(node->type, "right shift");
		}
	}
	else if(ts[i] == '='){
		node->token[cnt]= '=';
		cnt++;
		i++;
		strcpy(node->type, "greaterthanequalto");
	}
	else{
		strcpy(node->type, "greater than");
	}
	node->token[cnt] = '\0';
return;
}

void lessthan(char* ts, TokenizerT* node){

	int cnt=0;
	node->token[cnt]= '<';
	cnt++;
	i++;
	if(ts[i] == '<'){
		node->token[cnt]= '<';
		cnt++;
		i++;
		if(ts[i]== '='){
			node->token[cnt]= '=';
			strcpy(node->type, "LS&assignment");
			cnt++;
			i++;
		}
		else{
			strcpy(node->type, "left shift");
		}
	}
	else if(ts[i] == '='){
		node->token[cnt]= '=';
		cnt++;
		i++;
		strcpy(node->type, "lessthanequalto");
	}
	else{
		strcpy(node->type, "less than");
	}
	node->token[cnt] = '\0';
return;
}


/*This method handles any other case that is inputed that doesnt meet the requirements of the other methods such as plus or minus*/
void other(char* ts, TokenizerT* node){
	int cnt = 0;


	if(ts[i] == '['){
		node->token[cnt] = '[';
		cnt++;
		i++;
		if(ts[i] == ']'){
			node->token[cnt] = ']';
			cnt++;
			i++;
			strcpy(node->type, "array element");

		}
		else{
			strcpy(node->type, "left bracket");
		}
	}
	else if(ts[i] == ']'){
		node->token[cnt] = ']';
		cnt++;
		i++;
		strcpy(node->type, "right bracket");
	}
	else if(ts[i] == '('){
		node->token[cnt] = '(';
		cnt++;
		i++;
		if(ts[i] == ')'){
			node->token[cnt] = ')';
			cnt++;
			i++;
			strcpy(node->type, "function");
		}
		else{
			strcpy(node->type, "left parenthesis");
		}
	}
	else if(ts[i] == '.'){
		node->token[cnt] = '.';
		cnt++;
		i++;
		strcpy(node->type, "period");
	}
	else if(ts[i] == ')'){
		node->token[cnt] = ')';
		cnt++;
		i++;
		strcpy(node->type, "right parenthesis");
	}
	else if(ts[i] == ','){
		node->token[cnt] = ',';
		cnt++;
		i++;
		strcpy(node->type, "comma");
	}
	else if(ts[i] == '?'){
		node->token[cnt] = '?';
		cnt++;
		i++;
		strcpy(node->type, "true");
	}
	else if(ts[i] == '{'){
		node->token[cnt] = '{';
		cnt++;
		i++;
		strcpy(node->type, "left brace");
	}
	else if(ts[i] == '}'){
		node->token[cnt] = '}';
		cnt++;
		i++;
		strcpy(node->type, "right brace");
	}
	else if(ts[i] == ':'){
		node->token[cnt] = ':';
		cnt++;
		i++;
		strcpy(node->type, "false");
	}
	else if(ts[i] == '~'){
		node->token[cnt] = '~';
		cnt++;
		i++;
		strcpy(node->type, "1's comps");
	}
	else if(ts[i] == '#'){
		node->token[cnt] = '#';
		cnt++;
		i++;
		strcpy(node->type, "pound");
	}
	else if(ts[i] == '^'){
		node->token[cnt] = '^';
		cnt++;
		i++;
		strcpy(node->type, "exclusive or");
	}
	else if(ts[i] == '@'){
		node->token[cnt] = '@';
		cnt++;
		i++;
		strcpy(node->type, "at");
	}
	else if(ts[i] == '\\'){
		node->token[cnt] = '\\';
		cnt++;
		i++;
		strcpy(node->type, "backslash");
	}
	else if(ts[i] == '/'){
		node->token[cnt] = '/';
		cnt++;
		i++;
		if(ts[i] == '/'){
			node->token[cnt] = '/';
			cnt++;
			i++;
			strcpy(node->type, "comment");
		}
		else{
			strcpy(node->type, "frontslash");
		}
	}
	else if(ts[i] == '"'){
		node->token[cnt] = '"';
		cnt++;
		i++;
		strcpy(node->type, "quote");
	}
	else if(ts[i] == '\''){
		node->token[cnt] = '\'';
		cnt++;
		i++;
		strcpy(node->type, "single quote");
	}
	else if(ts[i] == ';'){
		node->token[cnt] = ';';
		cnt++;
		i++;
		strcpy(node->type, "semi-colon");
	}
	else if(ts[i] == ':'){
		node->token[cnt] = ':';
		cnt++;
		i++;
		strcpy(node->type, "colon");
	}
	else if(ts[i] == '`'){
		node->token[cnt] = '`';
		cnt++;
		i++;
		strcpy(node->type, "accent mark");
	}
	else if(ts[i] == '='){
		node->token[cnt] = '=';
		cnt++;
		i++;
		strcpy(node->type, "assignment equals");
		if(ts[i] == '='){
			node->token[cnt] = '=';
                	cnt++;
    			i++;
             		strcpy(node->type, "comparative equals");
		}
	}
	else if(ts[i] == '_'){
		node->token[cnt] = '_';
		cnt++;
		i++;
		strcpy(node->type, "underscore");
	}
	else if(ts[i] == '$'){
	node->token[cnt] = '$';
	cnt++;
	i++;
	strcpy(node->type, "dollar sign");
	}
	else{
		fprintf(stderr, "Error With String Input\n");
	}
	node->token[cnt] = '\0';
	return;
}

void ckeywordcmp(TokenizerT* node){

	if(strcmp(node->token,"if") == 0){
		strcpy(node->type, "c- keyword");
	}
	else if (strcmp(node->token,"while") == 0){
		strcpy(node->type, "c- keyword");
	}
	else if (strcmp(node->token,"auto") == 0){
                strcpy(node->type, "c- keyword");
        }

	else if (strcmp(node->token,"break") == 0){
			strcpy(node->type, "c- keyword");
	}
	else if (strcmp(node->token,"return") == 0){
			strcpy(node->type, "c- keyword");
		}
	
	else if (strcmp(node->token,"case") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"char") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"const") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"continue") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"default") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"do") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"double") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"else") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"enum") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"extern") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"float") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"for") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"goto") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"int") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"long") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"register") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"short") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"signed") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"sizeof") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"static") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"struct") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"switch") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"typedef") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"union") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"unsigned") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"void") == 0){
                strcpy(node->type, "c- keyword");
        }
	else if (strcmp(node->token,"volatile") == 0){
                strcpy(node->type, "c- keyword");
        }




return;
}


void TKDestroy( TokenizerT * tk ) {
	free(tk);
	return;
}


void printtokens(TokenizerT* root){

	TokenizerT* node = root->next;
	TokenizerT* prev = root;
	while(node->next != NULL){
		printf("%s \"%s\"\n", node->type, TKGetNextToken(node));
		node = node->next;
		prev = prev->next;
		TKDestroy(prev);
	}


	return;
}

char *TKGetNextToken( TokenizerT * tk ) {
	return tk->token;
}
