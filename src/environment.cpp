#include "../includes/environment.h"
#include <utility>
#include <cstdio>

void Environment::define(char *name, std::pair<char *, Type> value){
	env[name] = value;
}

std::pair<char *, Type> Environment::get(char *name){
	for(auto &x : env){
		if(!strcmp(x.first, name)){
			return x.second;
		}
	}
	char *res = (char *)malloc(5 * sizeof(char));
	strcpy(res, "none");
	return std::make_pair(res, Type::NONE);
}
