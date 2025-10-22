#include <Scanner.h>
#include <stdexcept>

Scanner::Scanner(const std::string &string)
{
    source = string;
}

std::vector<Token> Scanner::scanTokens(){
    while (!isAtEnd()) {
      // We are at the beginning of the next lexeme.
      start = current;
      scanToken();
    }

    tokens.push_back(Token(END_OF_FILE, "", line));
    return tokens;
}

char Scanner::advance(){
    return source[current++];
}


bool Scanner::isAtEnd(){
    return current >= source.size();
}

void Scanner::addToken(TokenType tokenType){
    std::string text=source.substr(start,current);
    tokens.push_back(Token(tokenType, text, line));
}

void Scanner::scanToken(){
    char c = advance();
    switch (c) {
      case '(': addToken(LEFT_PAREN); break;
      case ')': addToken(RIGHT_PAREN); break;
      case '{': addToken(LEFT_BRACE); break;
      case '}': addToken(RIGHT_BRACE); break;
      case ',': addToken(COMMA); break;
      case '.': addToken(DOT); break;
      case '-': addToken(MINUS); break;
      case '+': addToken(PLUS); break;
      case ';': addToken(SEMICOLON); break;
      case '*': addToken(STAR); break; 
    }
}