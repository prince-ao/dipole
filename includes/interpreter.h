#pragma once
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include <utility>
#include <cstring>

enum class Type {
	NONE, NUMBER, BOOLEAN
};

class Interpreter {
public:
	void interpret(AstNode *root);
private:
	std::pair<char *, Type> *expression(AstNode *root);
	char *itoa(int n);
	bool match(Type n, Type m);
	template<typename... Args>
	bool match(Type n, Type m, Args... args);
};

#endif
