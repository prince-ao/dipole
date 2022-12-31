#pragma once
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

class Interpreter {
public:
	void interpret(AstNode *root);
private:
	int i_expression(AstNode *root);
};

#endif
