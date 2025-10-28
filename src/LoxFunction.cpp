#include <LoxFunction.h>
#include <Environment.h>
#include <Interpreter.h>
#include <Return.h>

LoxFunction::LoxFunction(std::shared_ptr<Statement> decl,std::shared_ptr<Environment> _closure):
declaration(decl), closure(_closure){};

Literal LoxFunction::call(Interpreter& interpreter, const std::vector<Literal>& args){
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(Environment{closure});
    std::vector<Token> params = std::get<FuncStatement>(declaration->statement).func.params;
    for(int i=0;i<LoxFunction::arity();i++){
        environment->define(params[i].lexeme, args[i]);
    }
    try{
        std::shared_ptr<Statement> body = std::get<FuncStatement>(declaration->statement).func.body;
        interpreter.executeBlock(body, environment);
    } catch(Return returnVal){
        return returnVal.val;
    }
    return std::monostate{};
}

int LoxFunction::arity() const{
    return std::get<FuncStatement>(declaration->statement).func.params.size();
}

std::string LoxFunction::toString() const{
    return "<fn "+ std::get<FuncStatement>(declaration->statement).func.name.lexeme + ">";
}