#include "../includes/interpreter.h"

void Interpreter::interpret(AstNode *root){
	if(root == nullptr) return;

	switch(root->data->token){
		case Token_::ASTGLUE:
			interpret(root->left);
			interpret(root->right);
			break;
		case Token_::PRINT:
			printf("%d\n", i_expression(root->left));
			break;
	}
}

int Interpreter::i_expression(AstNode *root){
	int lval, rval;

	if(root->left)
		lval = i_expression(root->left);
	if(root->right)
		rval = i_expression(root->right);

	switch(root->data->token){
		case Token_::PLUS:
			return lval + rval;
			break;
		case Token_::MINUS:
			return lval - rval;
			break;
		case Token_::INT:
			return root->data->intvalue;
		default:
			puts("Unexpected token, i_expression");
			return 0;
	}
}
