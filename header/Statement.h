#ifndef CODECRAFTERS_INTERPRETER_STATEMENT_H
#define CODECRAFTERS_INTERPRETER_STATEMENT_H


#include <Expression.h>
#include <Variable.h>
#include <vector>

struct Statement;

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

struct BlockStatement{
    std::vector<std::shared_ptr<Statement>> Statements;
};

struct IfStatement{
    std::shared_ptr<Expression> condition;
    std::shared_ptr<Statement> thenBranch;
    std::shared_ptr<Statement> elseBranch;
};

struct WhileStatement{
    std::shared_ptr<Expression> condition;
    std::shared_ptr<Statement> body;
};

struct Statement {
    std::variant<ExpressionStatement, PrintStatement, VarStatement, BlockStatement, IfStatement, WhileStatement> statement;
};


#endif