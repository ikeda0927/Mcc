#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//values representing the type of token
enum{
	TK_NUM = 256,	//integer token
	TK_EOF,		//EOF token
};

//type of token
typedef struct{
	int ty;		//type of token
	int val;	//if "ty" is "TK_NUM", "val" mewns value
	char *input;	//this is used to signal an error	
} Token;

//stored user input
char *user_input;

//stored torkenized user input
Token tokens[100];

void error(char *fmt,...){ 		//function that take variable-length argument
	va_list ap;			//convert any number of arguments into one veriable
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1); 			//exit(0) means abnormal termination
}

//report the location if the error
void error_at(char *loc, char *msg){
	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s" ,pos,  "");
	fprintf(stderr, "^ %s\n" ,msg);
	exit(1);
}

//Tokenize the string stored in "inuser_input"
//and store it in "tokens"
void tokenize(){
	char *p = user_input;

	int i = 0;
	while(*p){
		if(isspace(*p)){
			p++;
			continue;
		}

		if(*p=='+' || *p=='-'){
			tokens[i].ty = *p;
			tokens[i].input = p;
			i++;
			p++;
			continue;
		}

		if(isdigit(*p)){
			tokens[i].ty = TK_NUM;
			tokens[i].input = p;
			tokens[i].val = strtol(p, &p, 10);
			i++;
			continue;
		}

		error_at(p, "error:can not tokenize");
	}

	tokens[i].ty = TK_EOF;
	tokens[i].input = p;
	
}

int main(int argc, char **argv){
	if(argc != 2){
		error("error:incorrect number of arguments");
		return 1;
	};

	user_input = argv[1];
	tokenize();

	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	if(tokens[0].ty != TK_NUM)
		error_at(tokens[0].input, "error:beginning of argument is not number");
	printf("	mov rax, %ld\n",tokens[0].val);

	int i = 1;
	while(tokens[i].ty != TK_EOF){
		if(tokens[i].ty == '+'){
			i++;
			if(tokens[i].ty != TK_NUM)
				error_at(tokens[i].input, "error:this is not number");
			printf("	add rax, %ld\n",tokens[i].val);
			i++;
			continue;
		}

		if(tokens[i].ty == '-'){
			i++;
			if(tokens[i].ty != TK_NUM)
				error_at(tokens[i].input, "error:this is not number");
			printf("	sub rax, %ld\n",tokens[i].val);
			i++;
			continue;
		}

		
		error_at(tokens[i].input ,"error:Unexpected token");
	}

	printf("	ret\n");
	return 0;
}


