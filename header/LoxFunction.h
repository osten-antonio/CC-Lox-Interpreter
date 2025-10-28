#ifndef CODECRAFTERS_INTERPRETER_FUNCTION_H
#define CODECRAFTERS_INTERPRETER_FUNCTION_H

#include <LoxCallable.h>
#include <Statement.h>
#include <Environment.h>


class LoxFunction: public LoxCallable{
public:
    LoxFunction(std::shared_ptr<Statement> decl, std::shared_ptr<Environment> _closure);
    Literal call(Interpreter& interpreter, const std::vector<Literal>& args) override;
    int arity() const override;
    std::string toString() const override;
private:
    std::shared_ptr<Statement> declaration;
    std::shared_ptr<Environment> closure;
};

#endif