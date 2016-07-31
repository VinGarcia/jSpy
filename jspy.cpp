#include <iostream>
#include <string>
#include <iostream>

#include "code-parser.h"

int main(int argc, char* argv[]) {

  uint pos;
  std::string line;
  const char* rest;
  BlockStatement code;
  GlobalScope map;

  std::cout << "\nJSpy 0.1a by Vinícius Garcia" << std::endl;

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

  return 0;
}
