#include <Environment.h>
#include <_error.h>

Literal Environment::get(Token name){
    if(values.find(name.lexeme) != values.end()){
        return values[name.lexeme];
    }
    throw RuntimeError(name,"Undefined variable "+name.lexeme+".");
    
}
void Environment::define(std::string name, Literal value){
    values[name]=value;
}

void Environment::assign(Token name, Literal value){
    if(values.find(name.lexeme)!=values.end()){
        values[name.lexeme]=value;
        return;
    }
    throw RuntimeError(name,"Undefined variable "+name.lexeme+".");
}