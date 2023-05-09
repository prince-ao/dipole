#include "../includes/lexer.h"
#include "stdio.h"

void Lexer::lex(char *filein){
	this->filein = filein;
	curr = 0;
	curr_char = filein[curr];
	if(isBlockCmt) block_comment();

	char *string;
	while(curr_char != '\0') {
		curr_char = ignore_whitespace();
		if(curr_char == '\0') break;
		// fprintf(stdout, "current_char = %c; prev_char = %c\n", curr_char, filein[curr-1]);

		switch(curr_char){
			case '\n':
				if(tkns.token_array[tkns.size-1]->token == Token_::NEW_LINE) break;
				push_back(Token_::NEW_LINE, 0);
				break;
			case '+':
				push_back(Token_::PLUS, 0);
				break;
			case '-':
				push_back(Token_::MINUS, 0);
				break;
			case '*':
				push_back(Token_::STAR, 0);
				break;
			case '/':
				if(filein[curr+1] == '/'){
					line_comment();
				}else if(filein[curr+1] == '*'){
					curr += 2;
					block_comment();
				}else{
					push_back(Token_::SLASH, 0);
				}
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
			case '{':
				push_back(Token_::LBRACE, 0);
				break;
			case '}':
				push_back(Token_::RBRACE, 0);
				break;
			case '<':
				if(filein[curr+1] == '='){
					push_back(Token_::LE, 0);
					curr++;
				}else{
					push_back(Token_::LT, 0);
				}
				break;
			case '&':
				if(filein[curr+1] == '&'){
					push_back(Token_::AND, 0);
					curr++;
				}
				break;
			case '|':
				if(filein[curr+1] == '|'){
					push_back(Token_::OR, 0);
					curr++;
				}
				break;
			case '>':
				if(filein[curr+1] == '='){
					push_back(Token_::GE, 0);
					curr++;
				}else{
					push_back(Token_::GT, 0);
				}
				break;
			case '=':
				if(filein[curr+1] == '='){
					push_back(Token_::EQ, 0);
					curr++;
				}else{
					push_back(Token_::ASSIGN, 0);
				}
				break;
			case '!':
				if(filein[curr+1] == '='){
					push_back(Token_::NE, 0);
					curr++;
				}else{
					push_back(Token_::NOT, 0);
				}
				break;
			case '"':
				string = get_string();
				push_back(Token_::STRING, string);
				break;
			default:
				if(is_int()){
					push_back(Token_::INT, get_int());
					break;
				}else if(isdigit(curr_char) && !isdigit(filein[curr+1])){
					push_back(Token_::INT, 0);
					break;
				}
				std::pair<bool, char *> word;
				if((word = keyword()).first) break;
				else push_back(Token_::IDENT, word.second);
				if(filein[curr] == '\n') curr--;
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
	else return false;
}

int Lexer::get_int() {
	int result = 0;
	while(curr_char != '\0'){
		if(isdigit(curr_char)){
			result = (result*10) + (curr_char - '0');
		}else if(isspace(curr_char) && curr_char != '\n') break;
		//else if(curr_char == ';') { curr--; break; }
		else {
			curr--;
			break;
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
		temp->value.intvalue = intvalue;
		tkns.token_array[tkns.size++] = temp;
	}else{
		Token *temp = (Token *)malloc(sizeof(Token));
		temp->token = token;
		temp->value.intvalue = intvalue;
		tkns.token_array[tkns.size++] = temp;
	}
}

void Lexer::push_back(Token_ token, char *stringvalue) {
	if(tkns.capacity == tkns.size){
		tkns.capacity *= 2;
		errno = 0;
		if((tkns.token_array = (Token **)realloc(tkns.token_array, tkns.capacity * sizeof(Token))) == NULL){
			perror("realloc, lexer.c 28.37");
			exit(1);
		}
		Token *temp = (Token *)malloc(sizeof(Token));
		temp->token = token;
		temp->value.stringvalue = stringvalue;
		tkns.token_array[tkns.size++] = temp;
	}else{
		Token *temp = (Token *)malloc(sizeof(Token));
		temp->token = token;
		temp->value.stringvalue = stringvalue;
		tkns.token_array[tkns.size++] = temp;
	}
}

char Lexer::ignore_whitespace() {
	char curr_char = filein[curr++];

	while(curr_char != '\0'){
		if(curr_char == '\n'){
			Line++;
			break;
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
			printf("[%s]\n", "PLUS");
			break;
		case Token_::MINUS:
			printf("[%s]\n", "MINUS");
			break;
		case Token_::STAR:
			printf("[%s]\n", "STAR");
			break;
		case Token_::SLASH:
			printf("[%s]\n", "SLASH");
			break;
		case Token_::INT:
			printf("[%s, %d]\n", "INT", token->value.intvalue);
			break;
		case Token_::LET:
			printf("[%s]\n", "LET");
			break;
		case Token_::IDENT:
			printf("[%s, %s]\n", "IDENT", token->value.stringvalue);
			break;
		case Token_::STRING:
			printf("[%s, %s]\n", "STRING", token->value.stringvalue);
			break;
		case Token_::EQ:
			printf("[%s]\n", "EQ");
			break;
		case Token_::NE:
			printf("[%s]\n", "NE");
			break;
		case Token_::GT:
			printf("[%s]\n", "GT");
			break;
		case Token_::LT:
			printf("[%s]\n", "LT");
			break;
		case Token_::GE:
			printf("[%s]\n", "GE");
			break;
		case Token_::LE:
			printf("[%s]\n", "LE");
			break;
		case Token_::AND:
			printf("[%s]\n", "AND");
			break;
		case Token_::OR:
			printf("[%s]\n", "OR");
			break;
		case Token_::TRUE:
			printf("[%s]\n", "TRUE");
			break;
		case Token_::FALSE:
			printf("[%s]\n", "FALSE");
			break;
		case Token_::ASSIGN:
			printf("[%s]\n", "ASSIGN");
			break;
		case Token_::NOT:
			printf("[%s]\n", "NOT");
			break;
		case Token_::NONE:
			printf("[%s]\n", "NONE");
			break;
		case Token_::PRINT:
			printf("[%s]\n", "PRINT");
			break;
		case Token_::IF:
			printf("[%s]\n", "IF");
			break;
		case Token_::WHILE:
			printf("[%s]\n", "WHILE");
			break;
		case Token_::FOR:
			printf("[%s]\n", "FOR");
			break;
		case Token_::ELSE:
			printf("[%s]\n", "ELSE");
			break;
		case Token_::SEMI:
			printf("[%s]\n", "SEMI");
			break;
		case Token_::EOT:
			printf("[%s]\n", "EOT");
			break;
		case Token_::LPARAN:
			printf("[%s]\n", "LPEREN");
			break;
		case Token_::RPARAN:
			printf("[%s]\n", "RPEREN");
			break;
		case Token_::LBRACE:
			printf("[%s]\n", "LBRACE");
			break;
		case Token_::RBRACE:
			printf("[%s]\n", "RBRACE");
			break;
		case Token_::ASTGLUE:
			printf("[%s]\n", "ASTGLUE");
			break;
		case Token_::NEW_LINE:
			printf("[%s]\n", "NEW_LINE");
			break;
		default:
			puts("unknown");
			break;
	}
}


char *Lexer::get_word(){
	char *text = (char *)malloc(65 * sizeof(char));
	int counter = 0;
	while(isalpha(curr_char)){
		text[counter] = curr_char;
		curr_char = filein[++curr];
		counter++;
	}
	text[counter] = 0;
	text = (char *)realloc(text, (counter + 1) * sizeof(char));
	return text;
}

std::pair<bool, char*> Lexer::keyword(){
	int old_curr = curr;
	int old_curr_char = filein[curr];
	char *word = get_word();
	switch(old_curr_char){
		case 't':
			if(!strcmp(word, "true")){
				push_back(Token_::TRUE, 0);
				curr--;
				return std::make_pair(true, word);
			}
			break;
		case 'f':
			if(!strcmp(word, "false")){
				push_back(Token_::FALSE, 0);
				curr--;
				return std::make_pair(true, word);
			}else if(!strcmp(word, "for")) {
				push_back(Token_::FOR, 0);
				curr--;
				return std::make_pair(true, word);
			}
			break;
		case 'p':
			if(!strcmp(word, "print")){
				push_back(Token_::PRINT, 0);
				curr--;
				return std::make_pair(true, word);
			}
			break;
		case 'n':
			if(!strcmp(word, "none")){
				push_back(Token_::NONE, 0);
				curr--;
				return std::make_pair(true, word);
			}
			break;
		case 'i':
			if(!strcmp(word, "if")){
				push_back(Token_::IF, 0);
				curr--;
				return std::make_pair(true, word);
			}
			break;
		case 'e':
			if(!strcmp(word, "else")){
				push_back(Token_::ELSE, 0);
				curr--;
				return std::make_pair(true, word);
			}
			break;
		case 'l':
			if(!strcmp(word, "let")){
				push_back(Token_::LET, 0);
				curr--;
				return std::make_pair(true, word);
			}
			break;
		case 'w':
			if(!strcmp(word, "while")){
				push_back(Token_::WHILE, 0);
				curr--;
				return std::make_pair(true, word);
			}
			break;
		default:
			curr--;
			return std::make_pair(false, word);
	}
	return std::make_pair(false, word);
}

char *Lexer::get_string(){
	curr_char = filein[++curr];
	int size = 0;
	int capacity = 2;
	char *word = (char *)malloc(2 * sizeof(char));
	while(curr_char != '\0' && curr_char != '\n' && curr_char != '"') {
		if(size == capacity){
			word = (char *)realloc(word, (capacity *= 2) * sizeof(char));
		}
		word[size++] = curr_char;
		curr_char = filein[++curr];
	}
	if(curr_char == '"'){
		if(capacity == size){
				word = (char *)realloc(word, (++capacity) * sizeof(char));
		}else if(capacity > size){
				word = (char *)realloc(word, (size+1) * sizeof(char));
		}
		word[size++] = '\0';
		return word;
	} else {
		fputs("string not closed on the line\n", stderr);
		exit(1);
	}
}


Token *Lexer::next(){
	return tkns.token_array[curr_token++];
}

Token *Lexer::current(){
	return tkns.token_array[curr_token-1];
}

void Lexer::line_comment() {
	curr_char = filein[(curr += 2)];
	while(curr_char != '\n' && curr_char != '\0'){
		curr_char = filein[++curr];
	}
}

void Lexer::block_comment(){
	while(curr_char != '\0'){
		if(curr_char == '*' && filein[curr+1] == '/') break;
		curr_char = filein[++curr];
	}
	if(curr_char == '\0') {
		isBlockCmt = true;
		curr--;
	}
	else{
		isBlockCmt = false;
		curr_char = filein[(curr += 3)];
	}
}

void Lexer::put_back(){
	--curr_token;
}

Token *Lexer::peek() {
	return tkns.token_array[curr_token];
}
