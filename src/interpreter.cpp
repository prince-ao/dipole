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

	//printf("i_expr, %d, %d\n", lval, rval);
	/*if(root->parent && 
			root->parent->data->token == Token_::MINUS){
		if(root->data->token == Token_::MINUS){
			root->data->token = Token_::PLUS;
		}else if(root->data->token == Token_::PLUS){
			root->data->token = Token_::MINUS;
		}
	}*/
	
	switch(root->data->token){
		case Token_::PLUS:
			return lval + rval;
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
