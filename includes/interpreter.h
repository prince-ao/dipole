#pragma once
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "lexer.h"
#include <utility>
#include <cstring>
#include "environment.h"

class Interpreter {
public:
	void interpret(AstNode *root, Environment *scope);
private:
	Lexer l;
	std::pair<char *, Type> *expression(AstNode *root);
	char *itoa(int n);
	bool match(Type n, Type m);
	template<typename... Args>
	bool match(Type n, Type m, Args... args);
	Environment *global;
};

#endif
