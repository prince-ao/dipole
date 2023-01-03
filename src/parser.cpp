#include "../includes/includes.h"
#include "../includes/lexer.h"
#include "../includes/parser.h"

AstNode *Parser::parse_program(){
	AstNode *left = nullptr; 
	AstNode *root = nullptr; 

	while(1){
		Token *curr = l->next();
		switch(curr->token){
			case Token_::PRINT:
				root = expression();
				root = mkastunary(curr, root);
				break;
			case Token_::EOT:
				return left;
			default:
				fputs("expected print\n", stderr);
				exit(1);
		}

		if(root){
			if(left == nullptr){
				left = root;
			} else{
				left = mkastnode(new Token(Token_::ASTGLUE), left, root);
			}
		}
	}

	return left;
}

AstNode *Parser::expression(){
	AstNode *left, *right;


	left = unary();

	while(1){
		Token *tt = l->next();
		/*if(tt->token == Token_::SEMI)
			return left;*/

		/*if(tt->token != Token_::PLUS && tt->token != Token_::MINUS){
			fprintf(stderr, "invalid token, expected + or -, but got: \n");
			l->print_token(tt);
			exit(1);
		}*/

		if(!arithop(tt)){
			l->put_back();
			break;
		}

		right = unary();

		left = mkastnode(tt, left, right);
	}

	return left;
}

bool Parser::arithop(Token *tt){
	if(tt->token == Token_::PLUS || tt->token == Token_::MINUS){
		return true;
	}
	return false;
}

AstNode *Parser::unary() {
	AstNode *left;
	Token *t = l->next();

	if(t->token == Token_::MINUS){
		left = unary();
		left = mkastunary(t, left);
	}else if(t->token == Token_::INT){
		left = mkastleaf(t);
	}else {
		fputs("invalid token, expect unary\n", stderr);
		exit(1);
	}
	return left;
}

AstNode *Parser::alloc_ast_node() {
	AstNode *result = (AstNode *)malloc(sizeof(AstNode));

	result->data = nullptr;
	result->left = nullptr;
	result->right = nullptr;
	result->parent = nullptr;

	return result;
}

AstNode *Parser::mkastnode(Token *token, AstNode *left, AstNode *right){
	AstNode *n = alloc_ast_node();

	n->data = token;
	n->left = left;
	n->right = right;
	if(left) n->left->parent = n;
	if(right) n->right->parent = n;

	return n;
}

AstNode *Parser::mkastleaf(Token *token){
	return mkastnode(token, nullptr, nullptr);
}

AstNode *Parser::mkastunary(Token *token, AstNode *left){
	return mkastnode(token, left, nullptr);
}

void Parser::free_ast(AstNode *root){
	if(root == nullptr) return;
	AstNode *left = root->left;
	AstNode *right = root->right;

	free(root->data);
	free(root);

	free_ast(left);
	free_ast(right);
}

void Parser::print_ast(AstNode *root, int depth){
	if(root == nullptr) return;

	for(int i = 0; i < depth; i++)
		printf(" ");
	l->print_token(root->data);

	print_ast(root->left, depth+1);
	print_ast(root->right, depth+1);
}
