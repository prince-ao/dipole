#pragma once
#ifndef PARSER_H
#define PARSER_H

#include "includes.h"
#include "lexer.h"


struct AstNode {
	struct Token *data;
	AstNode *left;
	AstNode *right;
};

class Parser {
public:
	Parser(Lexer *l){
		this->l = l;
	}
	AstNode *parse_program();
	void free_ast(AstNode *root);
	AstNode *alloc_ast_node();
	AstNode *expression();
	AstNode *int_();
	void print_ast(AstNode *root, int depth);
private:
	AstNode *mkastnode(Token *token, AstNode *left, AstNode *right);
	AstNode *mkastleaf(Token *token);
	AstNode *mkastunary(Token *token, AstNode *left);
	Lexer *l = nullptr;
};

#endif
