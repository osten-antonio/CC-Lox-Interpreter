#ifndef CODECRAFTERS_INTERPRETER_NATIVE_FUNC_H
#define CODECRAFTERS_INTERPRETER_NATIVE_FUNC_H

#include <LoxCallable.h>
#include <chrono>

struct ClockFunction : public LoxCallable{
    int arity() const override{
        return 0;
    }
    Literal call(Interpreter& interpreter, const std::vector<Literal>& args) override{
        using namespace std::chrono;
        double seconds = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()/1000;
        return seconds;
    }
};


#endif