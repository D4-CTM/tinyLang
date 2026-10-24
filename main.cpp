#include "src/lexer.hpp"
#include <fstream>
#include <iostream>

int main(int argc, const char** argv) {
    std::ifstream in(argv[1]);
    Lexer lexer(in);

    Token tkn = lexer.nextToken();

    while (tkn.token != TokenId::END_OF_FILE) {
        std::cout << "Token: " << lexer.tokenToString(tkn.token) << " [" << tkn.data << "]\n";
        tkn = lexer.nextToken();
    }

    in.close();
    return 0;
}
