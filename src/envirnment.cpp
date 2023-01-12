#include "../includes/envirnment.h"
#include <utility>

void Envirnment::define(char *name, std::pair<Type, char *> value){
	env[name] = value;
}

std::pair<Type, char *> Envirnment::get(char *name){
	auto pos_val = env.find(name);
	if(pos_val == env.end()) {
		char *res = (char *)malloc(5 * sizeof(char));
		strcpy(res, "none");
		return std::make_pair(Type::NONE, res);
	}else{
		return pos_val->second;
	}
}

