#include "../includes/interpreter.h"

void Interpreter::interpret(AstNode *root, Environment *scope){
	std::pair<char *, Type> *boolean_expr_ans;
	Environment *new_scope;
	if(root == nullptr) return;

	switch(root->data->token){
		case Token_::ASTGLUE:
			interpret(root->left, scope);
			interpret(root->right, scope);
			break;
		case Token_::PRINT:
			printf("%s\n", expression(root->left, scope)->first);
			break;
		case Token_::IF:
			new_scope = new Environment(scope);
			boolean_expr_ans = expression(root->left, scope);
			if(!strcmp("true", boolean_expr_ans->first)) interpret(root->mid, new_scope);
			else if(!strcmp("false", boolean_expr_ans->first) || !strcmp("none", boolean_expr_ans->first)) interpret(root->right, new_scope);
			else {
				fputs("expected boolean if statement\n", stderr);
				exit(1);
			}
			break;
		case Token_::LET:
			scope->define(root->left->left->data->value.stringvalue, 
					*expression(root->left->right, scope));
			break;
		case Token_::ASSIGN:
			scope->assign(root->left->data->value.stringvalue, *expression(root->right, scope));
			break;
	}
}

std::pair<char *, Type> *Interpreter::expression(AstNode *root, Environment *scope){
	std::pair<char *, Type> *lval, *rval;
	char *res;
	int result;
	bool b_result;


	if(root->left)
		lval = expression(root->left, scope);
	if(root->right)
		rval = expression(root->right, scope);


	switch(root->data->token){
		case Token_::PLUS:
			if( match(lval->second, Type::NONE, Type::BOOLEAN)
					|| match(rval->second, Type::NONE, Type::BOOLEAN)){
				free(lval);
				free(rval);
				char *res = (char *)malloc(5 * sizeof(char));
				strcpy(res, "none");
				return new std::pair<char *, Type>{res, Type::NONE};
			}
			else if(match(lval->second, Type::STRING) && match(rval->second, Type::STRING)){
				char *new_string = (char *)malloc((strlen(lval->first) + strlen(rval->first) + 1)* sizeof(char));
				strcpy(new_string, lval->first);
				strcat(new_string, rval->first);
				return new std::pair<char *, Type>{new_string, Type::STRING};
			}
			result = atoi(lval->first) + atoi(rval->first);
			free(lval);
			free(rval);
			return new std::pair<char *, Type>{itoa(result), Type::NUMBER};
		case Token_::MINUS:
			if( match(lval->second, Type::NONE, Type::BOOLEAN)
					|| match(rval->second, Type::NONE, Type::BOOLEAN)){
				if(lval->second == Type::NUMBER){
					result = -atoi(lval->first);
					free(lval);
					return new std::pair<char *, Type>{itoa(result), Type::NUMBER};
				}
				free(lval);
				free(rval);
				char *res = (char *)malloc(5 * sizeof(char));
				strcpy(res, "none");
				return new std::pair<char *, Type>{res, Type::NONE};
			}
			result = atoi(lval->first) - atoi(rval->first);
			free(lval);
			free(rval);
			return new std::pair<char *, Type>{itoa(result), Type::NUMBER};
		case Token_::STAR:
			if( match(lval->second, Type::NONE, Type::BOOLEAN)
					|| match(rval->second, Type::NONE, Type::BOOLEAN)){
				free(lval);
				free(rval);
				char *res = (char *)malloc(5 * sizeof(char));
				strcpy(res, "none");
				return new std::pair<char *, Type>{res, Type::NONE};
			}
			result = atoi(lval->first) * atoi(rval->first);
			free(lval);
			free(rval);
			return new std::pair<char *, Type>{itoa(result), Type::NUMBER};
		case Token_::SLASH:
			if( match(lval->second, Type::NONE, Type::BOOLEAN)
					|| match(rval->second, Type::NONE, Type::BOOLEAN)){
				free(lval);
				free(rval);
				char *res = (char *)malloc(5 * sizeof(char));
				strcpy(res, "none");
				return new std::pair<char *, Type>{res, Type::NONE};
			}
			result = atoi(lval->first) / atoi(rval->first);
			free(lval);
			free(rval);
			return new std::pair<char *, Type>{itoa(result), Type::NUMBER};
		case Token_::GT:
			if( match(lval->second, Type::NONE, Type::BOOLEAN)
					|| match(rval->second, Type::NONE, Type::BOOLEAN)){
				free(lval);
				free(rval);
				char *res = (char *)malloc(5 * sizeof(char));
				strcpy(res, "none");
				return new std::pair<char *, Type>{res, Type::NONE};
			}
			else if(match(lval->second, Type::STRING) && match(rval->second, Type::STRING)){
				res = (char *)malloc(6 * sizeof(char));
				if(strcmp(lval->first, rval->first) > 0){
					strcpy(res, "true");
				}else{
					strcpy(res, "false");
				}
				free(lval);
				free(rval);
				return new std::pair<char *, Type>{res, Type::BOOLEAN};
			}
			res = (char *)malloc(6 * sizeof(char));
			if(atoi(lval->first) > atoi(rval->first))
				strcpy(res, "true");
			else
				strcpy(res, "false");
			free(lval);
			free(rval);
			return new std::pair<char *, Type>{res, Type::BOOLEAN};
		case Token_::LT:
			if( match(lval->second, Type::NONE, Type::BOOLEAN)
					|| match(rval->second, Type::NONE, Type::BOOLEAN)){
				free(lval);
				free(rval);
				char *res = (char *)malloc(5 * sizeof(char));
				strcpy(res, "none");
				return new std::pair<char *, Type>{res, Type::NONE};
			} else if(match(lval->second, Type::STRING) && match(rval->second, Type::STRING)){
				res = (char *)malloc(6 * sizeof(char));
				if(strcmp(lval->first, rval->first) < 0){
					strcpy(res, "true");
				}else{
					strcpy(res, "false");
				}
				free(lval);
				free(rval);
				return new std::pair<char *, Type>{res, Type::BOOLEAN};
			}
			res = (char *)malloc(6 * sizeof(char));
			if(atoi(lval->first) < atoi(rval->first))
				strcpy(res, "true");
			else
				strcpy(res, "false");
			free(lval);
			free(rval);
			return new std::pair<char *, Type>{res, Type::BOOLEAN};
		case Token_::GE:
			if( match(lval->second, Type::NONE, Type::BOOLEAN)
					|| match(rval->second, Type::NONE, Type::BOOLEAN)){
				free(lval);
				free(rval);
				char *res = (char *)malloc(5 * sizeof(char));
				strcpy(res, "none");
				return new std::pair<char *, Type>{res, Type::NONE};
			} else if(match(lval->second, Type::STRING) && match(rval->second, Type::STRING)){
				res = (char *)malloc(6 * sizeof(char));
				if(strcmp(lval->first, rval->first) >= 0){
					strcpy(res, "true");
				}else{
					strcpy(res, "false");
				}
				free(lval);
				free(rval);
				return new std::pair<char *, Type>{res, Type::BOOLEAN};
			}
			res = (char *)malloc(6 * sizeof(char));
			if(atoi(lval->first) >= atoi(rval->first))
				strcpy(res, "true");
			else
				strcpy(res, "false");
			free(lval);
			free(rval);
			return new std::pair<char *, Type>{res, Type::BOOLEAN};
		case Token_::LE:
			if( match(lval->second, Type::NONE, Type::BOOLEAN)
					|| match(rval->second, Type::NONE, Type::BOOLEAN)){
				free(lval);
				free(rval);
				char *res = (char *)malloc(5 * sizeof(char));
				strcpy(res, "none");
				return new std::pair<char *, Type>{res, Type::NONE};
			} else if(match(lval->second, Type::STRING) && match(rval->second, Type::STRING)){
				res = (char *)malloc(6 * sizeof(char));
				if(strcmp(lval->first, rval->first) <= 0){
					strcpy(res, "true");
				}else{
					strcpy(res, "false");
				}
				free(lval);
				free(rval);
				return new std::pair<char *, Type>{res, Type::BOOLEAN};
			}
			res = (char *)malloc(6 * sizeof(char));
			if(atoi(lval->first) <= atoi(rval->first))
				strcpy(res, "true");
			else
				strcpy(res, "false");
			free(lval);
			free(rval);
			return new std::pair<char *, Type>{res, Type::BOOLEAN};
		case Token_::EQ:
			if( match(lval->second, Type::NONE, Type::BOOLEAN)
					|| match(rval->second, Type::NONE, Type::BOOLEAN)){
				free(lval);
				free(rval);
				char *res = (char *)malloc(5 * sizeof(char));
				strcpy(res, "none");
				return new std::pair<char *, Type>{res, Type::NONE};
			} else if(match(lval->second, Type::STRING) && match(rval->second, Type::STRING)){
				res = (char *)malloc(6 * sizeof(char));
				if(strcmp(lval->first, rval->first) == 0){
					strcpy(res, "true");
				}else{
					strcpy(res, "false");
				}
				free(lval);
				free(rval);
				return new std::pair<char *, Type>{res, Type::BOOLEAN};
			}
			res = (char *)malloc(6 * sizeof(char));
			if(atoi(lval->first) == atoi(rval->first))
				strcpy(res, "true");
			else
				strcpy(res, "false");
			free(lval);
			free(rval);
			return new std::pair<char *, Type>{res, Type::BOOLEAN};
		case Token_::NE:
			if( match(lval->second, Type::NONE, Type::BOOLEAN)
					|| match(rval->second, Type::NONE, Type::BOOLEAN)){
				free(lval);
				free(rval);
				char *res = (char *)malloc(5 * sizeof(char));
				strcpy(res, "none");
				return new std::pair<char *, Type>{res, Type::NONE};
			} else if(match(lval->second, Type::STRING) && match(rval->second, Type::STRING)){
				res = (char *)malloc(6 * sizeof(char));
				if(strcmp(lval->first, rval->first) != 0){
					strcpy(res, "true");
				}else{
					strcpy(res, "false");
				}
				free(lval);
				free(rval);
				return new std::pair<char *, Type>{res, Type::BOOLEAN};
			}
			res = (char *)malloc(6 * sizeof(char));
			if(atoi(lval->first) != atoi(rval->first))
				strcpy(res, "true");
			else
				strcpy(res, "false");
			free(lval);
			free(rval);
			return new std::pair<char *, Type>{res, Type::BOOLEAN};
		case Token_::AND:
			if(!match(lval->second, Type::BOOLEAN)
					|| !match(rval->second, Type::BOOLEAN)){
				free(lval);
				free(rval);
				char *res = (char *)malloc(5 * sizeof(char));
				strcpy(res, "none");
				return new std::pair<char *, Type>{res, Type::NONE};
			}
			res = (char *)malloc(6 * sizeof(char));
			if(!strcmp(lval->first, "true") && !strcmp(rval->first, "true")){
				strcpy(res, "true");
			}else{
				strcpy(res, "false");
			}
			free(lval);
			free(rval);
			return new std::pair<char *, Type>{res, Type::BOOLEAN};
		case Token_::OR:
			if( match(lval->second, Type::BOOLEAN, Type::NONE)){
				if(lval->second == Type::BOOLEAN && !strcmp(lval->first, "false")){
					free(lval);
					return rval;
				}else if(!strcmp(lval->first, "none")){
					free(lval);
					return rval;
				}
			}
			free(rval);
			return lval;
		case Token_::NOT:
			if(match(lval->second, Type::NUMBER)){
				free(lval);
				char *res = (char *)malloc(5 * sizeof(char));
				strcpy(res, "none");
				return new std::pair<char *, Type>{res, Type::NONE};
			}
			else if(match(lval->second, Type::BOOLEAN)){
				if(!strcmp(lval->first, "false")){
					res = (char *)malloc(5 * sizeof(char));
					strcpy(res, "true");
					return new std::pair<char *, Type>{res, Type::BOOLEAN};
				}else{
					res = (char *)malloc(6 * sizeof(char));
					strcpy(res, "false");
					return new std::pair<char *, Type>{res, Type::BOOLEAN};
				}
			}
			else if(match(lval->second, Type::NONE)){
				res = (char *)malloc(6 * sizeof(char));
				strcpy(res, "false");
				return new std::pair<char *, Type>{res, Type::BOOLEAN};
			}
		case Token_::TRUE:
			res = (char *)malloc(5 * sizeof(char));
			strcpy(res, "true");
			return new std::pair<char *, Type>{res, Type::BOOLEAN};
		case Token_::FALSE:
			res = (char *)malloc(6 * sizeof(char));
			strcpy(res, "false");
			return new std::pair<char *, Type>{res, Type::BOOLEAN};
		case Token_::NONE:
			res = (char *)malloc(5 * sizeof(char));
			strcpy(res, "none");
			return new std::pair<char *, Type>{res, Type::NONE};
		case Token_::INT:
	return new std::pair<char *, Type>{itoa(root->data->value.intvalue), Type::NUMBER};
		case Token_::IDENT:
			return new std::pair<char *, Type>{scope->get(root->data->value.stringvalue)};
		case Token_::STRING:
			return new std::pair<char *, Type>{root->data->value.stringvalue, Type::STRING};
		default:
			res = (char *)malloc(5 * sizeof(char));
			strcpy(res, "none");
			return new std::pair<char *, Type>{res, Type::NONE};
	}
}

bool Interpreter::match(Type n, Type m){
	return n == m;
}

template<typename... Args>
bool Interpreter::match(Type n, Type m, Args... args){
	return n == m || match(n, args...);
}

char *Interpreter::itoa(int n){
	char *ans = (char *)malloc(2 * sizeof(char));

	if(n == 0){
		ans[0] = '0';
		ans[1] = 0;
		return ans;
	}

	int max_ans = 2;
	int len = 0;

	while(n > 0){
		if(max_ans == len+1){
			ans = (char *)realloc(ans, max_ans * 2 * sizeof(char));
			max_ans *= 2;
		}
		
		for(int i = len; i > 0; i--){
			ans[i] = ans[i-1];
		}

		ans[0] = (n%10)+'0';

		n/=10;

		len++;
	}

	if(max_ans == len+1){
		ans = (char *)realloc(ans, (max_ans + 1) * sizeof(char));
	}

	ans[len] = 0;
	return ans;
}
