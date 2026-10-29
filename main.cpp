#include "src/parser.hpp"
#include <exception>
#include <fstream>
#include <iostream>

int main(int argc, const char **argv) {
  std::ifstream in(argv[1]);
  Parser parser(in);

  try {
    parser.parse();
  } catch (std::exception &ex) {
    std::cerr << ex.what() << '\n';
  }

  in.close();

  return 0;
}
