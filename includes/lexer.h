#pragma once
#ifndef LEXER_H
#define LEXER_H

#include<cstdlib>

enum class Token_ {
	PLUS, MINUS, STAR, SLASH,
	INT, SEMI, PRINT, EOT, 
	LPARAN, RPARAN,
	ASTGLUE
};

struct Token {
	Token(Token_ type){
		token = type;
	}
	Token_ token;
	int intvalue = 0;
};

class Lexer {
public:
	Lexer(){
		tkns.token_array = (Token **)malloc(sizeof(Token *));
		curr_token = 0;
	}
	Token *next();
	void put_back();
	void lex(char *filein);
	void print_tokens();
	void print_token(Token* token);
	void done();
private:
	struct Token_Array {
		Token **token_array;
		int size = 0;
		int capacity = 1;
	};

	Token_Array tkns; // for the token array
	int curr_token; // for the token array

	char curr_char; // for the file
	int curr; // for the file
	char *filein;// for the file

	int Line = 1;

	char ignore_whitespace();
	void push_back(Token_ token, int intvalue);
	int get_int();
	bool is_int();
	char prev_char();
	bool keyword();
	void line_comment();
};

#endif
