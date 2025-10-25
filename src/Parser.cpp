#include <_Parser.h>
#include <iostream>

Parser::Parser(std::vector<Token> _tokens):tokens(_tokens)
{}

void Parser::print(){
    for(const Token &token:tokens){
        switch(token.tokenType){
            case NIL:
                std::cout << "nil" << '\n';
                break;
            case TRUE:
                std::cout << "true" << '\n';
                break;
            case FALSE:
                std::cout <<"false" << '\n';
                break;
        }

    }
}