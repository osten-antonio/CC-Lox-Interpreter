#ifndef CODECRAFTERS_INTERPRETER_INTERPRETER_H
#define CODECRAFTERS_INTERPRETER_INTERPRETER_H

#include <common.h>
#include <vector>
#include <Expression.h>
#include <Statement.h>
#include <Environment.h>
#include <LoxCallable.h>

class Interpreter{

public:
    Interpreter();
    void executeBlock(std::shared_ptr<Statement> body,std::shared_ptr<Environment> env);
    void interpretStatements(std::vector<std::shared_ptr<Statement>> stmts);
    std::variant<std::string,std::monostate> interpret(const Expression& expr);
    std::shared_ptr<Environment> globals = std::make_shared<Environment>();
    std::shared_ptr<Environment> environment = globals;
    std::variant<std::string,std::monostate> getVal(Literal val);
private:
    Literal evaluate(const Expression& expr);

};


#endif