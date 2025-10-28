#ifndef CODECRAFTERS_INTERPRETER_RETURN_H
#define CODECRAFTERS_INTERPRETER_RETURN_H

#include <exception>
#include <common.h>

class Return: public std::exception{
public:
    Literal val;
    Return(Literal _val):val(_val){}
};


#endif
