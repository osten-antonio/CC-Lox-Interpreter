#ifndef CODECRAFTERS_INTERPRETER_COMMON_H
#define CODECRAFTERS_INTERPRETER_COMMON_H

#include <list>
#include <string>


enum TokenType
{
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,


    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    IDENTIFIER,
    STRING,
    NUMBER,

    AND,
    OR,
    IF,
    ELSE,
    TRUE,
    FALSE,
    FUN,
    CLASS,
    NIL,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    VAR,
    WHILE,
    FOR,

    END_OF_FILE,
    ERR

};

class Token
{
public:
    enum TokenType tokenType;
    std::string lexeme;

    size_t _line;

    Token(TokenType type, const std::string &data, size_t line)
    {
        tokenType = type;
        lexeme = data;
        _line = line;
    }

    std::string toString() const
    {
        return {std::to_string(tokenType) + " " + lexeme};
    }


    static std::string tokenString(const Token &token)
    {
        switch (token.tokenType)
        {

            case LEFT_PAREN:
                return "LEFT_PAREN ( null";
                break;
            case RIGHT_PAREN:
                return "RIGHT_PAREN ) null";
                break;
            case LEFT_BRACE:
                return "LEFT_BRACE { null";
                break;
            case RIGHT_BRACE:
                return "RIGHT_BRACE } null";
                break;
            case COMMA:
                return "COMMA , null";
                break;
            case DOT:
                return "DOT . null";
                break;
            case MINUS:
                return "MINUS - null";
                break;
            case PLUS:
                return "PLUS + null";
                break;
            case SEMICOLON:
                return "SEMICOLON ; null";
                break;
            case SLASH:
                return "SLASH / null";
                break;
            case STAR:
                return "STAR * null";
                break;
            case BANG:
                return "BANG ! null";
                break;
            case BANG_EQUAL:
                return "BANG_EQUAL != null";
                break;
            case EQUAL:
                return "EQUAL = null";
                break;
            case EQUAL_EQUAL:
                return "EQUAL_EQUAL == null";
                break;
            case GREATER:
                return "GREATER > null";
                break;
            case GREATER_EQUAL:
                return "GREATER_EQUAL >= null";
                break;
            case LESS:
                return "LESS < null";
                break;
            case LESS_EQUAL:
                return "LESS_EQUAL <= null";
                break;
            case IDENTIFIER:
                return "IDENTIFIER";
                break;
            case STRING:
                return "STRING";
                break;
            case NUMBER:
                return "NUMBER";
                break;
            case AND:
                return "AND";
                break;
            case OR:
                return "OR";
                break;
            case IF:
                return "IF";
                break;
            case ELSE:
                return "ELSE";
                break;
            case TRUE:
                return "TRUE";
                break;
            case FALSE:
                return "FALSE";
                break;
            case FUN:
                return "FUN";
                break;
            case CLASS:
                return "CLASS";
                break;
            case NIL:
                return "NIL";
                break;
            case PRINT:
                return "PRINT";
                break;
            case RETURN:
                return "RETURN";
                break;
            case SUPER:
                return "SUPER";
                break;
            case THIS:
                return "THIS";
                break;
            case VAR:
                return "VAR";
                break;
            case WHILE:
                return "WHILE";
                break;
            case FOR:
                return "FOR";
                break;
            case END_OF_FILE:
                return "EOF  null";
                break;
            default:
                return "UNKNOWN_TOKEN";
                break;
        }
    }
};


#endif //CODECRAFTERS_INTERPRETER_COMMON_H