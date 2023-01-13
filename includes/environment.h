#pragma once
#ifndef ENVIRNMENT_H
#define ENVIRNMENT_H

#include<unordered_map>
#include<cstdlib>
#include<cstring>
enum class Type {
	NONE, NUMBER, BOOLEAN
};

class Environment {
public:
	Environment(){
		e = nullptr;
	}
	Environment(Environment *e){
		this->e = e;
	}
	void assign(char *name, std::pair<char *, Type> value);
	void define(char *name, std::pair<char *, Type> value);
	std::pair<char *, Type> get(char *name);
private:
	std::unordered_map<char *, std::pair<char *, Type>> env;
	Environment *e;
};

#endif
