#ifndef CODECRAFTERS_INTERPRETER_STATEMENT_H
#define CODECRAFTERS_INTERPRETER_STATEMENT_H


#include <Expression.h>

struct ExpressionStatement {
    std::shared_ptr<Expression> expression;
};

struct PrintStatement {
    std::shared_ptr<Expression> expression;
};

struct Statement {
    std::variant<ExpressionStatement, PrintStatement> statement;
};


#endif