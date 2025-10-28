#ifndef CODECRAFTERS_INTERPRETER_DECLARATION_H
#define CODECRAFTERS_INTERPRETER_DECLARATION_H

#include <vector>
#include <common.h>

struct Statement;

struct varDecl{
    std::string identifier;
    std::shared_ptr<Expression> expression;
};

struct funDecl{
    Token name;
    std::vector<Token> params;
    std::shared_ptr<Statement> body;
};

#endif