#include"../includes/includes.h"
#include "../includes/lexer.h"

void Lexer::lex(char *filein){
	this->filein = filein;
	curr = 0;
	curr_char = filein[curr];

	
	while(curr_char != '\0') {
		curr_char = ignore_whitespace();
		if(curr_char == '\0') break;
		//fprintf(stderr, "current_char = %c; prev_char = %c\n", curr_char, filein[curr-1]);

		switch(curr_char){
			case '+':
				push_back(Token_::PLUS, 0);
				break;
			case '-':
				push_back(Token_::MINUS, 0);
				break;
			case ';':
				push_back(Token_::SEMI, 0);
				break;
			case '(':
				push_back(Token_::LPARAN, 0);
				break;
			case ')':
				push_back(Token_::RPARAN, 0);
				break;
			case '/':
				if(filein[curr+1] == '/'){
					line_comment();
				}
				break;
			default:
				if(keyword()) break;
				if(is_int()){
					push_back(Token_::INT, get_int());
					break;
				}

				fprintf(stderr, "unknown character at line %d: %c\n", Line, curr_char);
				exit(1);
		}

		curr_char = filein[++curr];
	}
}

char Lexer::prev_char() {
	int temp_curr;
	if((temp_curr = curr - 1) < 0){
		fputs("error lexing, lexer.c 37.7", stderr);
	}
	return filein[temp_curr];
}

bool Lexer::is_int() {
	if(curr_char >= '1' && curr_char <= '9') return true;
	else if(curr_char == '0' && (prev_char() >= '1' && prev_char() <= '9')) return true;
	else return false;
}

int Lexer::get_int() {
	int result = 0;
	while(curr_char != '\0'){
		if(is_int()){
			result = (result*10) + (curr_char - '0');
		}else if(isspace(curr_char)) break;
		else if(curr_char == ';') { curr--; break; }
		else {
			fprintf(stderr, "invalid character at line %d: %c\n", Line, curr_char);
			exit(1);
		}
		curr_char = filein[++curr];
	}
	return result;
}

void Lexer::push_back(Token_ token, int intvalue) {
	if(tkns.capacity == tkns.size){
		tkns.capacity *= 2;
		errno = 0;
		if((tkns.token_array = (Token **)realloc(tkns.token_array, tkns.capacity * sizeof(Token))) == NULL){
			perror("realloc, lexer.c 28.37");
			exit(1);
		}
		Token *temp = (Token *)malloc(sizeof(Token));
		temp->token = token;
		temp->intvalue = intvalue;
		tkns.token_array[tkns.size++] = temp;
	}else{
		Token *temp = (Token *)malloc(sizeof(Token));
		temp->token = token;
		temp->intvalue = intvalue;
		tkns.token_array[tkns.size++] = temp;
	}
}

char Lexer::ignore_whitespace() {
	char curr_char = filein[curr++];

	while(curr_char != '\0'){
		if(curr_char == '\n'){
			Line++;
			curr_char = filein[curr++];
		}else if(isspace(curr_char)){
			curr_char = filein[curr++];
		}else break;
	}
	curr--;

	return filein[curr];
}

void Lexer::done(){
	push_back(Token_::EOT, 0);
}

void Lexer::print_tokens() {
	for(int i = 0; i < tkns.size; i++){
		print_token(tkns.token_array[i]);
	}
}


void Lexer::print_token(Token* token){
	switch(token->token){
		case Token_::PLUS:
			printf("<%s>\n", "PLUS");
			break;
		case Token_::MINUS:
			printf("<%s>\n", "MINUS");
			break;
		case Token_::INT:
			printf("<%s, %d>\n", "INT", token->intvalue);
			break;
		case Token_::PRINT:
			printf("<%s>\n", "PRINT");
			break;
		case Token_::SEMI:
			printf("<%s>\n", "SEMI");
			break;
		case Token_::EOT:
			printf("<%s>\n", "EOT");
			break;
		case Token_::LPARAN:
			printf("<%s>\n", "LPEREN");
			break;
		case Token_::RPARAN:
			printf("<%s>\n", "RPEREN");
			break;
		case Token_::ASTGLUE:
			printf("<%s>\n", "ASTGLUE");
			break;
		default:
			puts("unknown");
			break;
	}
}

bool Lexer::keyword(){
	switch(curr_char){
		case 'p':
			char *text = (char *)malloc(6 * sizeof(char));
			int counter = 0; int temp_curr = curr;
			while(isalpha(curr_char)){
				if(counter == 5) return false;
				text[counter] = curr_char;
				curr_char = filein[++curr];
				counter++;
			}
			text[5] = 0;
			if(!strcmp(text, "print")){
				push_back(Token_::PRINT, 0);
				return true;
			}
			else {
				curr = temp_curr;
				curr_char = filein[curr];
				return false;
			}
	}
	return false;
}


Token *Lexer::next(){
	return tkns.token_array[curr_token++];
}

void Lexer::line_comment() {
	curr_char = filein[(curr += 2)];
	while(curr_char != '\n'){
		curr_char = filein[++curr];
	}
}

void Lexer::put_back(){
	--curr_token;
}
