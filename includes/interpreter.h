#pragma once
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include <utility>

enum class Type {
	NUMBER, BOOLEAN
};

class Interpreter {
public:
	void interpret(AstNode *root);
private:
	std::pair<char *, Type> *expression(AstNode *root);
};

#endif
