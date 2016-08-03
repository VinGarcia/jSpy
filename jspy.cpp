#include <iostream>
#include <string>
#include <iostream>

#include <fstream>
#include <streambuf>

#include "statements.h"

int main(int argc, char* argv[]) {

  uint pos;
  std::string line;
  const char* rest;
  BlockStatement code;
  GlobalScope map;

  if (argc == 1) {
    std::cout << "\nJSpy 0.2a by Vinícius Garcia" << std::endl;

    while (true) {
      getline(std::cin, line);
      if (line == "exit") break;

      // Skip blank lines:
      pos = 0;
      while (isblank(line[pos])) ++pos;
      if (line[pos] == '\0') continue;

      try {
        code.compile(line.c_str(), &rest, &map);
        code.exec(&map);
      } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
      }
    }
  } else if (argc == 2) {
    std::ifstream t(argv[1]);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    std::string code = "{" + str + "}";
    BlockStatement b(code.c_str(), &map);
    b.exec(&map);
  } else {
    throw std::invalid_argument("Expected a single file name as argument to the interpreter!");
  }

  return 0;
}
