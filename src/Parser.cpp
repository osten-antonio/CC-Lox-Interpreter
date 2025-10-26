#include <_Parser.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <memory>
#include <exception>

#include <_error.h>

Parser::Parser(std::vector<Token> _tokens):tokens(_tokens)
{}

std::shared_ptr<Expression> Parser::expression() {
    return equality();
}

std::shared_ptr<Expression> Parser::equality() {
    std::shared_ptr<Expression> expr1 = comparison();
    while (match({BANG_EQUAL, EQUAL_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expression> expr2 = comparison();
        expr1 = std::make_shared<Expression>(BinaryExpression{expr1, op, expr2});
    }
    return expr1;
}

std::shared_ptr<Expression> Parser::comparison() {
    std::shared_ptr<Expression> expr1 = term();
    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        Token op = previous();
        std::shared_ptr<Expression> expr2 = term();
        expr1 = std::make_shared<Expression>(BinaryExpression{expr1, op, expr2});
    }
    return expr1;
}

std::shared_ptr<Expression> Parser::term() {
    std::shared_ptr<Expression> expr1 = factor();
    while (match({PLUS, MINUS})) {
        Token op = previous();
        std::shared_ptr<Expression> expr2 = factor();
        expr1 = std::make_shared<Expression>(BinaryExpression{expr1, op, expr2});
    }
    return expr1;
}

std::shared_ptr<Expression> Parser::factor() {
    std::shared_ptr<Expression> expr1 = unary();
    while (match({STAR, SLASH})) {
        Token op = previous();
        std::shared_ptr<Expression> expr2 = unary();
        expr1 = std::make_shared<Expression>(BinaryExpression{expr1, op, expr2});
    }
    return expr1;
}

std::shared_ptr<Expression> Parser::unary() {
    if (match({MINUS, BANG})) {
        Token op = previous();
        std::shared_ptr<Expression> expr = unary();
        return std::make_shared<Expression>(UnaryExpression{op, expr});
    }
    return primary();
}


bool Parser::isAtEnd(){
    return peek().tokenType == END_OF_FILE;
}

Token Parser::peek(){
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) {
        ++current;
    }
    return previous();
}

bool Parser::match(std::vector<TokenType> types) {
    for (const auto& type : types) {
        if (!isAtEnd() && peek().tokenType==type) {
            advance();
            return true;
        }
    }
    return false;
}

std::shared_ptr<Expression> Parser::primary(){
    if (match({FALSE})) {
        return std::make_shared<Expression>(LiteralExpression{"false", false});
    }
    if (match({TRUE})) {
        return std::make_shared<Expression>(LiteralExpression{"true", true});
    }
    if (match({NUMBER, STRING})) {
        return std::make_shared<Expression>(LiteralExpression{previous().lexeme, previous()._literal});
    }
    if(match({NIL})){
        return std::make_shared<Expression>(LiteralExpression{previous().lexeme, std::monostate{}});
    }
    if (match({LEFT_PAREN})) {
        std::shared_ptr<Expression> innerExpr = expression();
        if(match({RIGHT_PAREN})){
            return std::make_shared<Expression>(GroupingExpression{innerExpr});
        }        
    }
    Token token = peek();
    throw ParseError(token._line,token.lexeme, "Expected expression.");
}
std::vector<std::shared_ptr<Statement>> Parser::parse() {
    std::vector<std::shared_ptr<Statement>> statements;

    while(!isAtEnd()){
        statements.push_back(statement());
    }

    return statements;
}

std::shared_ptr<Statement> Parser::statement(){
    if(match({PRINT})) return printStatement();

    return expressionStatement();
}   

std::shared_ptr<Statement> Parser::expressionStatement(){
    std::shared_ptr<Expression> expr = expression();
    if(!match({SEMICOLON})){
        Token token = peek();
        throw ParseError(token._line,token.lexeme, "Expect ';' after value.");
    }
    return std::make_shared<Statement>(ExpressionStatement{expr});
}


std::shared_ptr<Statement> Parser::printStatement(){
    std::shared_ptr<Expression> expr = expression();
    if(!match({SEMICOLON})){
        Token token = peek();
        throw ParseError(token._line,token.lexeme, "Expect ';' after value.");
    }
    return std::make_shared<Statement>(PrintStatement{expr});
}
