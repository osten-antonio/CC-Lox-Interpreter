#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <Scanner.h>

std::string read_file_contents(const std::string& filename);
bool hadError = false;

void printError(int line, std::string c){
    std::cerr << "[line " << line << "] Error: Unexpected character: " << c <<'\n';
}

void printTokens(std::vector<Token> tokens){
    for(Token token:tokens){
        std::string token_str = token.toString();
        if(token_str=="UNKNOWN_TOKEN"){
            printError(token._line,token.lexeme);
            hadError=true;
        }else{
            std::cout << token_str << '\n';
        }
        
    }
}

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    std::vector<Token> tokens;
    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here!" << std::endl;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);
        
        // Uncomment this block to pass the first stage
        
        Scanner* scanner= new Scanner(file_contents);
        tokens = scanner->scanTokens();

        printTokens(tokens);
        
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }
    if (hadError) return 65;
    return 0;
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
