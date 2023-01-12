#pragma once
#ifndef ENVIRNMENT_H
#define ENVIRNMENT_H

#include<unordered_map>
#include "interpreter.h"


class Envirnment {
public:
	void define(char *name, std::pair<Type, char *> value);
	std::pair<Type, char *> get(char *name);
private:
	std::unordered_map<char *, std::pair<Type, char *>> env;
};

#endif
