#ifndef CODECRAFTERS_INTERPRETER_PARSER_H
#define CODECRAFTERS_INTERPRETER_PARSER_H


#include <string>
#include <vector>
#include <common.h>


class Parser
{

std::vector<Token> tokens;
public:
    Parser(std::vector<Token> _tokens);
    void print();
private:
    int current = 0;


};

#endif