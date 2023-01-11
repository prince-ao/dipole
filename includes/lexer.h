#pragma once
#ifndef LEXER_H
#define LEXER_H

#include<cstdlib>
#include"../includes/includes.h"

enum class Token_ {
	PLUS, MINUS, STAR, SLASH, NOT,
	TRUE, FALSE, NONE,
	GT, GE, LT, LE, EQ, NE,
	INT, SEMI, PRINT, IF, ELSE,
	LET, IDENT,
	ASSIGN, LPARAN, RPARAN,
	LBRACE, RBRACE,
	ASTGLUE, EOT, NEW_LINE
};

struct Token {
	Token(Token_ type){
		token = type;
	}
	Token_ token;
	union{
		int intvalue;
		char *stringvalue;
	} value;
};

class Lexer {
public:
	Lexer(){
		tkns.token_array = (Token **)malloc(sizeof(Token *));
		curr_token = 0;
	}
	Token *next();
	Token *current();
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
	bool isBlockCmt = false;

	char ignore_whitespace();
	void push_back(Token_ token, int intvalue);
	void push_back(Token_ token, char *stringvalue);
	int get_int();
	bool is_int();
	char prev_char();
	std::pair<bool, char*> keyword();
	void line_comment();
	void block_comment();
	char *get_word();
};

#endif
