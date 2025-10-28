#include <_Parser.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <exception>
#include <Declaration.h>
#include <_error.h>


Parser::Parser(std::vector<Token> _tokens):tokens(_tokens)
{}

std::shared_ptr<Expression> Parser::expression() {
    return assignment();
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
    return call();
}

std::shared_ptr<Expression> Parser::call() {
    std::shared_ptr<Expression> expr = primary();
    
    while(true){
        if(match({LEFT_PAREN})){
            expr = finishCall(expr);
        }else break;
    }
    return expr;
}

std::shared_ptr<Expression> Parser::finishCall(std::shared_ptr<Expression> expr){
    std::vector<std::shared_ptr<Expression>> args;
    if(!isAtEnd() && peek().tokenType !=RIGHT_PAREN){
        do
        {
            if(args.size() >= 255) throw ParseError(peek()._line,peek().lexeme,"Can't have more than 255 arguments.");
            args.push_back(expression());
        } while (match({COMMA}));
    }
    if(isAtEnd() || peek().tokenType != RIGHT_PAREN) throw ParseError(peek()._line,peek().lexeme,"Expect ')' after 'arguments'.");
    advance();
    Token paren = previous();
    return std::make_shared<Expression>(CallExpression{expr,paren,args});
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
    if(match({IDENTIFIER})){
        return std::make_shared<Expression>(VariableExpression{previous()});
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
std::vector<std::shared_ptr<Statement>> Parser::parse(bool executing) {
    std::vector<std::shared_ptr<Statement>> statements;
    this->executing = executing;
    while(!isAtEnd()){
        statements.push_back(declaration());
    }

    return statements;
}

std::shared_ptr<Statement> Parser::function(std::string kind){
    if(isAtEnd() || peek().tokenType != IDENTIFIER) throw ParseError(peek()._line,peek().lexeme,"Expect " + kind + " after name.");
    advance();
    Token name = previous();
    if(isAtEnd() || peek().tokenType != LEFT_PAREN) throw ParseError(peek()._line,peek().lexeme,"Expect '(' after " + kind + " name.");
    advance();

    std::vector<Token> params;
    if(!isAtEnd() && peek().tokenType!=RIGHT_PAREN){
        do{
            if(params.size() >= 255) throw ParseError(peek()._line,peek().lexeme, "Can't have more than 255 parameters.");
            if(isAtEnd() || peek().tokenType != IDENTIFIER) throw RuntimeError(peek(),"Expect parameter name.");
            advance();
            params.push_back(previous());
            
        } while(match({COMMA}));
    }
    if(isAtEnd() || peek().tokenType != RIGHT_PAREN) throw ParseError(peek()._line,peek().lexeme,"Expect ')' after parameters.");
    advance();
    if(isAtEnd() || peek().tokenType != LEFT_BRACE) throw ParseError(peek()._line,peek().lexeme,"Expect '{' before "+ kind+ " body.");
    advance();
    std::shared_ptr<Statement> body = block();
    funDecl decl{name, params, body};
    return std::make_shared<Statement>(FuncStatement{decl});
}

std::shared_ptr<Statement> Parser::statement(){
    if(match({FUN})) return function("function");
    if(match({FOR})) return forStatement();
    if(match({WHILE})) return whileStatement();
    if(match({IF})) return ifStatement();
    if(match({PRINT})) return printStatement();
    if(match({RETURN})) return returnStatement();
    if(match({LEFT_BRACE})) return block();
    return expressionStatement();
}   

std::shared_ptr<Statement> Parser::returnStatement(){
    Token keyword = previous();
    std::shared_ptr<Expression> val;
    if(isAtEnd() || peek().tokenType != SEMICOLON){
        val = expression();
    }
    if(isAtEnd() || peek().tokenType != SEMICOLON) throw ParseError(peek()._line,peek().lexeme,"Expect ';' after loop condition.");
    advance();
    return std::make_shared<Statement>(ReturnStatement{keyword,val});

}

std::shared_ptr<Statement> Parser::forStatement(){
    if(isAtEnd() || peek().tokenType != LEFT_PAREN) throw ParseError(peek()._line,peek().lexeme,"Expect '(' after 'for'.");
    advance();

    std::shared_ptr<Statement> initializer;
    if(match({SEMICOLON})){
        initializer=nullptr;
    }else if(match({VAR})){
        initializer = varDeclaration();
    }else{
        initializer = expressionStatement();
    }
    std::shared_ptr<Expression> condition;
    if(!isAtEnd() && peek().tokenType !=SEMICOLON){
        condition = expression();
    }
    if(isAtEnd() || peek().tokenType != SEMICOLON) throw ParseError(peek()._line,peek().lexeme,"Expect ';' after loop condition.");
    advance();

    std::shared_ptr<Expression> increment;
    if(!isAtEnd() && peek().tokenType !=RIGHT_PAREN){
        increment = expression();
    }
    if(isAtEnd() || peek().tokenType != RIGHT_PAREN) throw ParseError(peek()._line,peek().lexeme,"Expect ';' after for clauses.");
    advance();
    std::shared_ptr<Statement> body = statement();

    if(increment){
        body = std::make_shared<Statement>(BlockStatement{
            std::vector<std::shared_ptr<Statement>>{
                body, 
                std::make_shared<Statement>(ExpressionStatement{increment})
            }}
        );
    }

    if(!condition){
        condition = std::make_shared<Expression>(LiteralExpression{"true",true});
    }
    body = std::make_shared<Statement>(WhileStatement{condition,body});
    if(initializer){
        body = std::make_shared<Statement>(BlockStatement{
            std::vector<std::shared_ptr<Statement>>{initializer,body}
        });
    }
    return body;
}

std::shared_ptr<Statement> Parser::whileStatement(){
    if(isAtEnd() && peek().tokenType != LEFT_PAREN) throw ParseError(peek()._line,peek().lexeme,"Expect '(' after 'while'.");
    advance();
    std::shared_ptr<Expression> expr = expression();
    if(isAtEnd() && peek().tokenType != RIGHT_PAREN) throw ParseError(peek()._line,peek().lexeme,"Expect ')' after 'while'.");
    advance();
    std::shared_ptr<Statement> stmt = statement();
    return std::make_shared<Statement>(WhileStatement{expr,stmt});

}

std::shared_ptr<Statement> Parser::block(){
    std::vector<std::shared_ptr<Statement>> statements;
    while(!isAtEnd() && peek().tokenType != RIGHT_BRACE){
        statements.push_back(declaration());
    }
    
    if (isAtEnd()) {
        throw ParseError(peek()._line, peek().lexeme, "Expect '}' after block.");
    }
    advance();
    return std::make_shared<Statement>(BlockStatement{statements});
}
    
std::shared_ptr<Statement> Parser::ifStatement(){
    if(isAtEnd() && peek().tokenType != LEFT_PAREN) throw ParseError(peek()._line,peek().lexeme,"Expect '(' after 'if'.");
    advance();
    std::shared_ptr<Expression> condition = expression();
    if(isAtEnd() && peek().tokenType != RIGHT_PAREN) throw ParseError(peek()._line,peek().lexeme,"Expect '(' after 'if'.");
    advance();

    std::shared_ptr<Statement> thenBranch = statement();
    std::shared_ptr<Statement> elseBranch;
    if(match({ELSE})) elseBranch = statement();

    return std::make_shared<Statement>(IfStatement{condition, thenBranch, elseBranch});

}

std::shared_ptr<Statement> Parser::declaration() {
    try {
        if (match({VAR})) return varDeclaration();

        return statement();
    } catch (ParseError error) {
        Token t = peek();
        sync();
        throw error ;
    }
}

std::shared_ptr<Statement> Parser::varDeclaration(){
    if(!isAtEnd() && peek().tokenType == IDENTIFIER){
        Token name = advance();
        std::shared_ptr<Expression> initializer = nullptr;
        if(match({EQUAL})){
            initializer = expression();
        }
        if(!isAtEnd() && peek().tokenType == SEMICOLON && executing){
            advance();
        }
        else{
            throw ParseError(peek()._line,peek().lexeme,"Expect ';' after variable declaration.");
        }
        varDecl decl{name.lexeme, initializer};
        VarStatement stmt{name.lexeme, initializer};
        return std::make_shared<Statement>(stmt);
    }
    else{
        Token token = peek();
        throw ParseError(token._line, token.lexeme, "Expect variable name.");
    }
}  


std::shared_ptr<Statement> Parser::expressionStatement(){
    std::shared_ptr<Expression> expr = expression();
    if(executing && !match({SEMICOLON})){
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

std::shared_ptr<Expression> Parser::orOperator(){
    std::shared_ptr<Expression> expr = andOperator();
    while(match({OR})){
        Token op = previous();
        std::shared_ptr<Expression> right = andOperator();
        expr = std::make_shared<Expression>(LogicalExpression{expr,op,right});
    }
    return expr;
}

std::shared_ptr<Expression> Parser::andOperator(){
    std::shared_ptr<Expression> expr = equality();
    while(match({AND})){
        Token op = previous();
        std::shared_ptr<Expression> right = andOperator();
        expr = std::make_shared<Expression>(LogicalExpression{expr,op,right});
    }
    return expr;
}

std::shared_ptr<Expression> Parser::assignment(){
    std::shared_ptr<Expression> expr = orOperator();
    if(match({EQUAL})){
        Token equals = previous();
        std::shared_ptr<Expression> value = assignment();
        
        if(std::holds_alternative<VariableExpression>(*expr)){
            Token name = std::get<VariableExpression>(*expr).name;
            return std::make_shared<Expression>(AssignmentExpression{name,value});
        }
        throw RuntimeError(equals,"Invalid assignmnet target.");
    }
    return expr;
}


void Parser::sync(){
    advance();

    while (!isAtEnd()) {
      if (previous().tokenType == SEMICOLON) return;

      switch (peek().tokenType) {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
          return;
      }

      advance();
    }
    
}