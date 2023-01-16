#include "../includes/includes.h"
#include "../includes/lexer.h"
#include "../includes/parser.h"

AstNode *Parser::parse_program(){
	AstNode *left = nullptr; 
	AstNode *root = nullptr; 
	Token	*curr = nullptr;

	while(1){
		if((root = statement()) == nullptr){
			return left;
		}

		if(root){
			if(left == nullptr){
				left = root;
			} else{
				left = mkastbinary(new Token(Token_::ASTGLUE), left, root);
			}
		}
	}

	return left;
}

AstNode *Parser::statement(){
	AstNode *result;
top:
	Token *curr = l->next();

	switch(curr->token){
		case Token_::PRINT:
			result = printStmt(curr);
			break;
		case Token_::IF:
			result = ifStmt(curr);
			break;
		case Token_::LET:
			result = declStmt(curr);
			break;
		case Token_::IDENT:
			result = assignStmt(curr);
			break;
		case Token_::EOT:
			result = nullptr;
			break;
		case Token_::RBRACE:
			result = nullptr;
			break;
		case Token_::NEW_LINE:
			goto top;
		default:
			fputs("syntax error, token\n", stderr);
			exit(1);
	}
	return result;
}

AstNode *Parser::block() {
	Token *curr = l->next();
	AstNode *left = nullptr; 
	AstNode *root = nullptr;
	
	if(!match(curr, Token_::LBRACE)){
		fputs("expected a block\n", stderr);
		exit(1);
	}

	while(1){
		if((root = statement()) == nullptr){
			l->put_back();
			break;
		}

		if(root){
			if(left == nullptr){
				left = root;
			} else{
				left = mkastbinary(new Token(Token_::ASTGLUE), left, root);
			}
		}
	}

	curr = l->next();
	if(!match(curr, Token_::RBRACE)){
		fputs("expected a block\n", stderr);
		exit(1);
	}

	return left;
}

AstNode *Parser::declStmt(Token *let) {
	Token *curr = l->next();
	AstNode *tree;

	if(!match(curr, Token_::IDENT)) {
		fputs("expected an identifier\n", stderr);
		exit(1);
	}

	tree = mkastleaf(curr);

	curr = l->next();

	if(match(curr, Token_::ASSIGN)) {
		tree = mkastbinary(curr, tree, expression());

		curr = l->next();

		if(!match(curr, Token_::NEW_LINE)){
			fputs("expected a new line\n", stderr);
			exit(1);
		}
		
		return mkastunary(let, tree);
	}

	curr = l->current();

	if(!match(curr, Token_::NEW_LINE)){
		fputs("expected a new line\n", stderr);
		exit(1);
	}

	tree = mkastunary(let, tree);

	return tree;
}

AstNode *Parser::assignStmt(Token *key){
	Token *curr = l->next(); 

	AstNode *tree;

	if(!match(curr, Token_::ASSIGN)){
		fputs("expected assignment\n", stderr);
		exit(1);
	}

	tree = mkastbinary(curr, mkastleaf(key), expression());

	curr = l->next();

	if(!match(curr, Token_::NEW_LINE)){
		fputs("expected a new line\n", stderr);
		exit(1);
	}

	return tree;
}

AstNode *Parser::ifStmt(Token *ifhead) {
	Token *curr = l->next();

	AstNode *boolean_expr, *true_expr, *false_expr = nullptr;

	if(!match(curr, Token_::LPARAN)){
		fputs("invalid syntax, expected left parenthesis\n", stderr);
		exit(1);
	}

	boolean_expr = expression();

	curr = l->next();

	if(!match(curr, Token_::RPARAN)){
		fputs("invalid syntax, expected right parenthesis\n", stderr);
		exit(1);
	}

	true_expr = block();

	curr = l->next();

	if(match(curr, Token_::ELSE)){
		false_expr = block();
	}

	if(match(curr, Token_::EOT)) {
		l->put_back();
	}

	return mkastnode(ifhead, boolean_expr, true_expr, false_expr);
}

AstNode *Parser::printStmt(Token *curr){
	AstNode *tree = nullptr;
	
	tree = expression();

	tree = mkastunary(curr, tree);

	curr = l->next();

	if(!match(curr, Token_::NEW_LINE, Token_::EOT, Token_::RBRACE)){
		fputs("expected new line after print\n", stderr);
		l->print_token(curr);
		exit(1);
	}
	if(match(curr, Token_::RBRACE, Token_::EOT)) l->put_back();
	
	return tree;
}


AstNode *Parser::expression(){
	return logic_op();
}

AstNode *Parser::logic_op(){
	AstNode *left, *right;


	left = equality();

	while(1){
		Token *tt = l->next();
		
		if(!match(tt, Token_::AND, Token_::OR)){
			l->put_back();
			break;
		}

		right = equality();

		left = mkastbinary(tt, left, right);
	}

	return left;
}

AstNode *Parser::equality(){
	AstNode *left, *right;


	left = comparison();

	while(1){
		Token *tt = l->next();
		
		if(!match(tt, Token_::NE, Token_::EQ)){
			l->put_back();
			break;
		}

		right = comparison();

		left = mkastbinary(tt, left, right);
	}

	return left;
}

AstNode *Parser::comparison(){
	AstNode *left, *right;


	left = term();

	while(1){
		Token *tt = l->next();
		
		if(!match(tt, Token_::GT, Token_::GE, Token_::LT, Token_::LE)){
			l->put_back();
			break;
		}

		right = term();

		left = mkastbinary(tt, left, right);
	}

	return left;
}

AstNode *Parser::term(){
	AstNode *left, *right;


	left = factor();

	while(1){
		Token *tt = l->next();
		
		if(!match(tt, Token_::PLUS, Token_::MINUS)){
			l->put_back();
			break;
		}

		right = factor();

		left = mkastbinary(tt, left, right);
	}

	return left;
}

AstNode *Parser::factor(){
	AstNode *left, *right;


	left = unary();

	while(1){
		Token *tt = l->next();

		if(!match(tt, Token_::STAR, Token_::SLASH)){
			l->put_back();
			break;
		}

		right = unary();

		left = mkastbinary(tt, left, right);
	}

	return left;
}

bool Parser::match(Token *a, Token_ b) {
  return a->token == b;
}

template<typename... Args>
bool Parser::match(Token *a, Token_ b, Args... args) {
  return a->token == b || match(a, args...);
}

AstNode *Parser::unary() {
	AstNode *left;
	Token *t = l->next();

	if(match(t, Token_::MINUS, Token_::NOT)){
		left = unary();
		left = mkastunary(t, left);
	}else {
		left = primary(t);
	}
	return left;
}

AstNode *Parser::primary(Token *t){
	AstNode *n;
	if(match(t, 
				Token_::INT, 
				Token_::TRUE, 
				Token_::FALSE, 
				Token_::NONE, 
				Token_::IDENT,
				Token_::STRING)){
		n = mkastleaf(t);
	}else if(t->token == Token_::LPARAN){
		 n = expression();
		 t = l->next();
		 if(!match(t, Token_::RPARAN)){
			 fputs("expected right paran", stderr);
			 exit(1);
		 }
	}
	return n;
}

AstNode *Parser::alloc_ast_node() {
	AstNode *result = (AstNode *)malloc(sizeof(AstNode));

	result->data = nullptr;
	result->left = nullptr;
	result->right = nullptr;
	result->parent = nullptr;

	return result;
}

AstNode *Parser::mkastnode(Token *token, AstNode *left, AstNode *mid, AstNode *right){
	AstNode *n = alloc_ast_node();

	n->data = token;
	n->left = left;
	n->mid = mid;
	n->right = right;
	if(left) n->left->parent = n;
	if(right) n->right->parent = n;
	if(mid) n->mid->parent = n;

	return n;
}

AstNode *Parser::mkastbinary(Token *token, AstNode *left, AstNode *right){
	return mkastnode(token, left, nullptr, right);
}

AstNode *Parser::mkastleaf(Token *token){
	return mkastbinary(token, nullptr, nullptr);
}

AstNode *Parser::mkastunary(Token *token, AstNode *left){
	return mkastbinary(token, left, nullptr);
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
		printf("*");
	l->print_token(root->data);

	print_ast(root->left, depth+1);
	print_ast(root->mid, depth+1);
	print_ast(root->right, depth+1);
}
