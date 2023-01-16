#include "../includes/environment.h"
#include <utility>
#include <cstdio>

void Environment::define(char *name, std::pair<char *, Type> value){
	env[name] = value;
}

void Environment::assign(char *name, std::pair<char *, Type> value){
	std::unordered_map<char *, std::pair<char *, Type>>::iterator st = env.end();

	 std::unordered_map<char *, std::pair<char *, Type>>::iterator it;
	 for(it = env.begin(); it != env.end(); ++it){
		 if(!strcmp(it->first, name)) st = it;
	 }
	if(st == env.end()){
		std::pair<char *, Type> nm = get(name);
		if(nm.second != Type::NONE){
			env[name] = value;
			return;
		}
		fprintf(stderr, "%s has not been defined\n", name);
		exit(1);
	}
	env.erase(st);
	env[name] = value;
}

std::pair<char *, Type> Environment::get(char *name) {
	for(auto &x : env){
		if(!strcmp(x.first, name)){
			return x.second;
		}
	}

	if(e){
		return e->get(name);
	}

	char *res = (char *)malloc(5 * sizeof(char));
	strcpy(res, "none");
	return std::make_pair(res, Type::NONE);
}
