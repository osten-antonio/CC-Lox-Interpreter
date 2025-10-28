#ifndef CODECRAFTERS_INTERPRETER_CALLABLE_H
#define CODECRAFTERS_INTERPRETER_CALLABLE_H

#include <vector>
#include <common.h>
struct Interpreter;

struct LoxCallable{
public:
    virtual Literal call(Interpreter& interpreter, const std::vector<Literal>& args) = 0;
    virtual int arity() const = 0;
    virtual std::string toString() const { return "<native fn>";}
};

#endif