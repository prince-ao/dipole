#pragma once
#ifndef PARSER_H
#define PARSER_H

#include "includes.h"
#include "lexer.h"

struct AstNode {
	struct Token *data;
	
	AstNode *left;
	AstNode *mid;
	AstNode *right;
	AstNode *parent;
};

class Parser {
public:
	Parser(Lexer *l){
		this->l = l;
	}
	AstNode *parse_program();
	void free_ast(AstNode *root);
	void print_ast(AstNode *root, int depth);
private:
	AstNode *statement();
	AstNode *block();
	AstNode *declStmt(Token *let);
	AstNode *assignStmt(Token *key);
	AstNode *ifStmt(Token *ifhead);
	AstNode *printStmt(Token *curr);
	AstNode *expression();
	AstNode *equality();
	AstNode *comparison();
	AstNode *logic_op();
	AstNode *term();
	AstNode *factor();
	AstNode *unary();
	AstNode *primary(Token *t);
	bool match(Token *a, Token_ b);
	template<typename... Args>
	bool match(Token *a, Token_ b, Args... args);
	bool arithop(Token *tt);
	AstNode *mkastnode(Token *token, AstNode *left, AstNode *mid, AstNode *right);
	AstNode *mkastbinary(Token *token, AstNode *left, AstNode *right);
	AstNode *mkastleaf(Token *token);
	AstNode *mkastunary(Token *token, AstNode *left);
	AstNode *alloc_ast_node();

	Lexer *l = nullptr;
};

#endif
