#ifndef CODECRAFTERS_INTERPRETER_SCANNER_H
#define CODECRAFTERS_INTERPRETER_SCANNER_H


#include <string>
#include <list>
#include "common.h"
#include <vector>

class Scanner
{
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    bool isAtEnd();


public:
    Scanner(const std::string &string);

    std::vector<Token> scanTokens();

    char advance();

    void addToken(TokenType tokenType, std::string value);
    void addToken(ErrorType errorType, std::string data);

    void scanToken();

private:
    bool match(char expected);
    char peek();
    void string();
};


#endif //CODECRAFTERS_INTERPRETER_SCANNER_H