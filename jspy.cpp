#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "statements.h"

int main(int argc, char* argv[]) {

  uint pos;
  std::string line;
  const char* rest;
  BlockStatement code;
  GlobalScope scope;

  if (argc == 1) {
    std::cout << "\nJSpy 0.2a by Vinícius Garcia" << std::endl;

    while (true) {
      std::cout << "> ";
      getline(std::cin, line);
      if (line == "exit") break;

      // Skip blank lines:
      pos = 0;
      while (isblank(line[pos])) ++pos;
      if (line[pos] == '\0') continue;

      try {
        code.compile(line.c_str(), &rest, scope);
        packToken result = code.exec(scope).value;
        if (result->type != NONE && result->type != VAR) {
          std::cout << result << std::endl;
        }
      } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
      } catch(const char* c) {
        std::cout << c << std::endl;
      }
    }
  } else if (argc == 2) {
    // Read the file:
    std::ifstream t(argv[1]);
    std::stringstream ss;
    ss << t.rdbuf();

    // Prepare as a code block:
    std::string code = "{" + ss.str() + "}";

    // Execute it:
    try {
      BlockStatement b(code.c_str(), scope);
      b.exec(scope);
    } catch(const std::exception& e) {
      std::cout << e.what() << std::endl;
    } catch(const char* c) {
      std::cout << c << std::endl;
    }
  } else {
    throw std::invalid_argument("Expected a single file name as argument to the interpreter!");
  }

  return 0;
}
