#include <iostream>
#include "catch.hpp"

#include "shunting-yard.h"
#include "code-parser.h"

void PREPARE_ENVIRONMENT() {

}

TEST_CASE("Build and evaluate BlockStatements") {
  const char* rest = 0;
  const char* code_text = "{ a = 2; b = 3; c = a+b; }End";
  TokenMap map;

  BlockStatement code(code_text, &rest);
  REQUIRE_NOTHROW(code.exec(&map));
  REQUIRE(map["a"].asDouble() == 2);
  REQUIRE(map["b"].asDouble() == 3);
  REQUIRE(map["c"].asDouble() == 5);
  REQUIRE(rest != 0);
  REQUIRE(*rest == 'E');

  code_text = "  c = a*2 + 2\n  b = 4;";
  REQUIRE_NOTHROW(code.compile(code_text, &rest));
  REQUIRE_NOTHROW(code.exec(&map));
  REQUIRE(map["c"].asDouble() == 6);
  REQUIRE(map["b"].asDouble() == 3);
  REQUIRE(rest == &(code_text[14]));

  REQUIRE_NOTHROW(code.compile("{a=b}"));
}

TEST_CASE("Build and evaluate IfStatements") {
  const char* rest = 0;
  const char* code_text = "if \n ((a+b)*2 == 6) {\n  a = a + 1; c = True;\n} else \n c = False;End";
  TokenMap map;
  map["a"] = 1;
  map["b"] = 2;

  IfStatement code;
  REQUIRE_NOTHROW(code.compile(code_text+2, &rest));
  REQUIRE(rest != 0);
  REQUIRE(*rest == 'E');

  // Test if it will execute the `then` block:
  REQUIRE_NOTHROW(code.exec(&map));
  REQUIRE(map["c"].asBool() == true);
  REQUIRE(map["a"].asDouble() == 2);

  // Test if it will execute the `else` block:
  REQUIRE_NOTHROW(code.exec(&map));
  REQUIRE(map["c"].asBool() == false);

  code_text = "if((a+b)*2 == 6) c = True";
  REQUIRE_NOTHROW(code.compile(code_text+2, &rest));
  REQUIRE(*rest == '\0');

  REQUIRE_NOTHROW(code.exec(&map));
  REQUIRE(map["c"].asBool() == false);

  map["a"] = 1;
  REQUIRE_NOTHROW(code.exec(&map));
  REQUIRE(map["c"].asBool() == true);
}

TEST_CASE("Build and evaluate ForStatements") {
  const char* rest = 0;
  const char* code_text = "for \n (n in range(3)) {\n b=a; a = a + 1; }End(); \n c = False;";
  TokenMap map;
  ForStatement code;
  map["a"] = 1;

  REQUIRE_NOTHROW(code.compile(code_text+3, &rest));
  REQUIRE(rest != 0);
  REQUIRE(*rest == 'E');

  REQUIRE_NOTHROW(code.exec(&map));
  REQUIRE(map["a"].asDouble() == 4);
  REQUIRE(map["b"].asDouble() == 3);
  REQUIRE(map["n"].asDouble() == 2);

  map["c"] = 0;
  code_text = "for(name in range(2,6,2)) c = c+name;E";
  REQUIRE_NOTHROW(code.compile(code_text+3, &rest));
  REQUIRE(*rest == 'E');

  REQUIRE_NOTHROW(code.exec(&map));
  REQUIRE(map["c"].asDouble() == 6);

  REQUIRE_NOTHROW(code.exec(&map));
  REQUIRE(map["c"].asDouble() == 12);
}

TEST_CASE("Build and evaluate WhileStatements") {
  const char* rest = 0;
  const char* code_text = "while \n (n < 10) {\n b=n; n = n + 1; }End(); \n c = False;";
  TokenMap map;
  WhileStatement code;
  map["n"] = 1;

  REQUIRE_NOTHROW(code.compile(code_text+5, &rest));
  REQUIRE(rest != 0);
  REQUIRE(*rest == 'E');
  
  REQUIRE_NOTHROW(code.exec(&map));
  REQUIRE(map["n"].asDouble() == 10);
  REQUIRE(map["b"].asDouble() == 9);
}

TEST_CASE("Code block with for and if statements", "[BlockStatement]") {
  const char* factorial_code =
    "{"
    "  n = 5;"
    "  total = 1;"
    "  for (i in range(1, n+1))"
    "    total = total*i;"
    "  if (total == 120)"
    "    result = total;"
    "  else"
    "    result = 'failed!';"
    "}";

  const char* rest = 0;
  TokenMap map;
  BlockStatement code;

  REQUIRE_NOTHROW(code.compile(factorial_code, &rest));
  REQUIRE_NOTHROW(code.exec(&map));

  REQUIRE(map["result"].asDouble() == 120);
}

TEST_CASE("Build and evaluate UserFunctions", "[UserFunctions][FuncDeclaration]") {
  const char* rest = 0;
  const char* code_text = "function my_sum (num1, num2) { n1 = num1; return num1 + num2 }End();";
  FuncDeclaration decl;
  TokenMap map;

  REQUIRE_NOTHROW(decl.compile(code_text+8, &rest, &map));
  REQUIRE_NOTHROW(decl.exec(&map));
  REQUIRE(map["my_sum"].str() == "[Function: my_sum]");
  REQUIRE(rest != 0);
  REQUIRE(*rest == 'E');

  map["n1"] = packToken::None;
  REQUIRE(calculator::calculate("my_sum(3,4) ", &map).asDouble() == 7);
  REQUIRE(map["n1"].asDouble() == 3);
  REQUIRE(calculator::calculate(" my_sum(1,1) ", &map).asDouble() == 2);
  REQUIRE(map["n1"].asDouble() == 1);
  REQUIRE(calculator::calculate(" my_sum(0,4) ", &map).asDouble() == 4);
  REQUIRE(map["n1"].asDouble() == 0);
}
