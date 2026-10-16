#include "src/lexer.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "Invalid arguments. Expected: <path to compiler> <path to source code file>" << '\n';
        return 1;
    }

    Lexer lexer(argv[1]);

    Token tkn = lexer.consume();
    while (tkn.type != TokenType::UNKNOWN) {
        std::cout << "Token data: " << tkn.data << '\n';
        std::cout << "Token: " << printTokenType(tkn.type) << '\n' << '\n' ;

        tkn = lexer.consume();
    }

    return 0;
}
