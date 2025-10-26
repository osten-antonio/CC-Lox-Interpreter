#ifndef CODECRAFTERS_INTERPRETER_ENVIRONMENT_H
#define CODECRAFTERS_INTERPRETER_ENVIRONMENT_H

#include <unordered_map>
#include <common.h>


class Environment{

public:
    Literal get(Token name);
    void define(std::string name, Literal value);
    void assign(Token name, Literal value);

private:
    std::unordered_map<std::string, Literal> values;

};

#endif