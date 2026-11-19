#include "src/parser.hpp"
#include "src/visitors/printVisitor.hpp"
#include <exception>
#include <fstream>
#include <iostream>

int main(int argc, const char **argv) {
  if (argc == 1) {
    std::cerr << "Expected path to 1 or more files" << '\n';
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    std::ifstream in(argv[i]);
    std::cout << "Reading file " << argv[i] << '\n';

    Lexer lexer(in);
    Parser parser(lexer);

    try {
      auto root = parser.parse();
      AstPrinter printer(std::cout);
      printer.visit(*root);
    } catch (std::exception &ex) {
      std::cerr << ex.what() << '\n';
    }
    std::cout << '\n';

    in.close();
  }

  return 0;
}
