#include "../includes/interpreter.h"

void Interpreter::interpret(AstNode *root){
	if(root == nullptr) return;

	switch(root->data->token){
		case Token_::ASTGLUE:
			interpret(root->left);
			interpret(root->right);
			break;
		case Token_::PRINT:
			printf("%s\n", expression(root->left)->first);
			break;
	}
}

std::pair<char *, Type> *Interpreter::expression(AstNode *root){
	std::pair<char *, Type> *lval, *rval;

	if(root->left)
		lval = expression(root->left);
	if(root->right)
		rval = expression(root->right);

	switch(root->data->token){
		case Token_::PLUS:

			break;
		case Token_::MINUS:
			return lval - rval;
			break;
		case Token_::STAR:
			return lval * rval;
		case Token_::SLASH:
			return lval / rval;
		case Token_::INT:
			return root->data->intvalue;
		default:
			puts("Unexpected token, i_expression");
			return 0;
	}
}
