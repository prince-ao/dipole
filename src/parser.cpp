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
	AstNode *left = int_();
	Token *tt = l->next();
	if(tt->token == Token_::SEMI)
		return left;

	if(tt->token != Token_::PLUS && tt->token != Token_::MINUS){
		fprintf(stderr, "invalid token, expected + or -, but got: \n");
		l->print_token(tt);
		exit(1);
	}
	left = mkastnode(tt, left, expression());

	return left;
}

AstNode *Parser::int_(){
	Token *tt = l->next();
	if(tt->token == Token_::INT){
		return mkastleaf(tt);
	}
	fputs("invalid token, expect int\n", stderr);
	exit(1);
}

AstNode *Parser::alloc_ast_node() {
	AstNode *result = (AstNode *)malloc(sizeof(AstNode));

	result->data = nullptr;
	result->left = nullptr;
	result->right = nullptr;

	return result;
}

AstNode *Parser::mkastnode(Token *token, AstNode *left, AstNode *right){
	AstNode *n = alloc_ast_node();

	n->data = token;
	n->left = left;
	n->right = right;

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