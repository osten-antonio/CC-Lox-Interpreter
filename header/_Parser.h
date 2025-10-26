#ifndef CODECRAFTERS_INTERPRETER_PARSER_H
#define CODECRAFTERS_INTERPRETER_PARSER_H


#include <string>
#include <vector>
#include <common.h>
#include <Expression.h>
#include <Statement.h>

class Parser
{

std::vector<Token> tokens;
public:
    Parser(std::vector<Token> _tokens);
    void print();
    std::vector<std::shared_ptr<Statement>> parse();
private:
    int current = 0;
    bool match(std::vector<TokenType> types);
    bool check(TokenType type);

    std::shared_ptr<Expression> expression();
    std::shared_ptr<Expression> equality();
    std::shared_ptr<Expression> comparison();
    std::shared_ptr<Expression> term();
    std::shared_ptr<Expression> factor();
    std::shared_ptr<Expression> unary();
    std::shared_ptr<Expression> primary();
    
    std::shared_ptr<Statement> statement();
    std::shared_ptr<Statement> printStatement();
    std::shared_ptr<Statement> expressionStatement();

    Token advance();
    Token peek();
    Token previous();
    bool isAtEnd();

};


#endif