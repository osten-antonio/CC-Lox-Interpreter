#include <Interpreter.h>
#include <iostream>

struct InterpreterVisitor{
    bool isEqual(Literal a, Literal b){
        if(std::holds_alternative<std::monostate>(a) && std::holds_alternative<std::monostate>(b)) return true;
        if(std::holds_alternative<std::monostate>(a)) return false;
        if (std::holds_alternative<bool>(a) && std::holds_alternative<bool>(b))
            return std::get<bool>(a) == std::get<bool>(b);

        if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
            return std::get<double>(a) == std::get<double>(b);

        if (std::holds_alternative<std::string>(a) && std::holds_alternative<std::string>(b))
            return std::get<std::string>(a) == std::get<std::string>(b);

        return false;
    }
    bool isTruthy(Literal obj){
        if(std::holds_alternative<std::monostate>(obj)) return false;
        if(std::holds_alternative<bool>(obj)) return std::get<bool>(obj);
        if(std::holds_alternative<double>(obj) && std::get<double>(obj) == 0) return false;
        if(std::holds_alternative<std::string>(obj) && std::get<std::string>(obj).length() == 0) return false; 
        return true;
    }

    Literal operator()(const BinaryExpression& expr){
        Literal left = std::visit(*this,*expr.left);
        Token op = expr.op;
        Literal right = std::visit(*this,*expr.right);
        switch (op.tokenType)
        {
        case PLUS:
            if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right)){
                return std::get<double>(left)+std::get<double>(right);
            }
            if(std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)){
                return std::get<std::string>(left)+std::get<std::string>(right);
            }
            break;
        case MINUS:
            return std::get<double>(left)-std::get<double>(right);
            break;
        case STAR:
            return std::get<double>(left)*std::get<double>(right);
            break;
        case SLASH:
            return std::get<double>(left)/std::get<double>(right);
            break;
        case GREATER:
            return std::get<double>(left)>std::get<double>(right);
        case GREATER_EQUAL:
            return std::get<double>(left)>=std::get<double>(right);
        case LESS:
            return std::get<double>(left)<std::get<double>(right);
        case LESS_EQUAL:
            return std::get<double>(left)<=std::get<double>(right);
        case BANG_EQUAL:
            return !isEqual(left,right);
        case EQUAL_EQUAL:
            return isEqual(left,right);
        default:
            break;
        }
        return std::monostate{};
    }
    Literal operator()(const UnaryExpression& expr){
        Literal right = std::visit(*this, *expr.right);

        switch (expr.op.tokenType)
        {
        case MINUS:
            return -std::get<double>(right);
            break;
        case BANG:
            return !isTruthy(right);
        default:
            break;
        }
        return std::monostate{};
    }
    Literal operator()(const LiteralExpression& expr){
        return expr.value;
    }
    Literal operator()(const GroupingExpression& expr){
        return std::visit(*this,*expr.expression);
    }
};



void Interpreter::interpret(const Expression& expr){
    Literal value = evaluate(expr);
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            std::cout << "nil" << '\n';
        } else if constexpr (std::is_same_v<T, bool>) {
            std::cout << (arg ? "true" : "false") << '\n';
        } else {
            std::cout << arg << '\n';
        }
    }, value);
}

Literal Interpreter::evaluate(const Expression& expr){
    InterpreterVisitor visitor;
    return std::visit(visitor,expr);
}