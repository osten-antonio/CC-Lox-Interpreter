#include <_Parser.h>
#include <iostream>
#include <sstream>
#include <iomanip>

Parser::Parser(std::vector<Token> _tokens):tokens(_tokens)
{}


void Parser::print(){
    for(const Token &token:tokens){
        switch(token.tokenType){
            case NUMBER:
            {
                std::ostringstream oss;
                oss << std::setprecision(15);
                oss << token.lexeme;

                std::string s = oss.str();

                // Ensure at least 1 decimal point (eg 1 -> 1.0)
                if (s.find('.') == std::string::npos && s.find('e') == std::string::npos) {
                    s += ".0";
                }
                std::cout << s << '\n';
                break;
            }
            default:
                std::cout << token.lexeme << '\n';

        }

    }
}