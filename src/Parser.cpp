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
                double value = std::stod(token.lexeme);

                // Convert to string using default formatting
                std::ostringstream oss;
                oss << value;
                std::string s = oss.str();

                // Ensure at least one decimal point
                if (s.find('.') == std::string::npos) {
                    s += ".0";
                }

                std::cout << s << '\n';
                break;
            }
            case STRING:
                std::cout << token.lexeme.substr(1,token.lexeme.length()-2) << '\n';
                break;
            default:
                std::cout << token.lexeme << '\n';

        }

    }
}