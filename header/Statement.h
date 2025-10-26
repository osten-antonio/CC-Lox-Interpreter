#ifndef CODECRAFTERS_INTERPRETER_STATEMENT_H
#define CODECRAFTERS_INTERPRETER_STATEMENT_H


#include <Expression.h>
#include <Variable.h>

struct ExpressionStatement {
    std::shared_ptr<Expression> expression;
};

struct PrintStatement {
    std::shared_ptr<Expression> expression;
};

struct VarStatement {
    std::string identifier;
    std::shared_ptr<Expression> initializer;
};


struct Statement {
    std::variant<ExpressionStatement, PrintStatement, VarStatement> statement;
};


#endif