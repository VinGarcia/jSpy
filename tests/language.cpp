#include <iostream>
#include "catch.hpp"

#include "../cparse/shunting-yard.h"
#include "../cparse/shunting-yard-exceptions.h"
#include "../statements.h"
#include "../matcher.h"

TEST_CASE("Build and evaluate BlockStatements") {
  const char* rest = 0;
  const char* code_text = "{ a = 2; b = 3; c = a+b; }End";
  TokenMap map;

  BlockStatement code(code_text, &rest);
  REQUIRE_NOTHROW(code.exec(map));
  REQUIRE(map["a"].asDouble() == 2);
  REQUIRE(map["b"].asDouble() == 3);
  REQUIRE(map["c"].asDouble() == 5);
  REQUIRE(rest != 0);
  REQUIRE(*rest == 'E');

  code_text = "  c = a*2 + 2\n  b = 4;";
  REQUIRE_NOTHROW(code.compile(code_text, &rest));
  REQUIRE_NOTHROW(code.exec(map));
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
  REQUIRE_NOTHROW(code.exec(map));
  REQUIRE(map["c"].asBool() == true);
  REQUIRE(map["a"].asDouble() == 2);

  // Test if it will execute the `else` block:
  REQUIRE_NOTHROW(code.exec(map));
  REQUIRE(map["c"].asBool() == false);

  code_text = "if((a+b)*2 == 6) c = True";
  REQUIRE_NOTHROW(code.compile(code_text+2, &rest));
  REQUIRE(*rest == '\0');

  REQUIRE_NOTHROW(code.exec(map));
  REQUIRE(map["c"].asBool() == false);

  map["a"] = 1;
  REQUIRE_NOTHROW(code.exec(map));
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

  REQUIRE_NOTHROW(code.exec(map));
  REQUIRE(map["a"].asDouble() == 4);
  REQUIRE(map["b"].asDouble() == 3);
  REQUIRE(map["n"].asDouble() == 2);

  map["c"] = 0;
  code_text = "for(name in range(2,6,2)) c = c+name;E";
  REQUIRE_NOTHROW(code.compile(code_text+3, &rest));
  REQUIRE(*rest == 'E');

  REQUIRE_NOTHROW(code.exec(map));
  REQUIRE(map["c"].asDouble() == 6);

  REQUIRE_NOTHROW(code.exec(map));
  REQUIRE(map["c"].asDouble() == 12);

  // Test the break statement:
  map["c"] = 0;
  code_text = "for(n in range(10)) { if(n >= 5) { break; print('here'); } c = c+1; }";
  REQUIRE_NOTHROW(code.compile(code_text+3, &rest));
  REQUIRE_NOTHROW(code.exec(map));

  REQUIRE(map["c"].asInt() == 5);

  // Test the continue statement:
  map["c"] = 0;
  code_text = "for(n in range(10)) { if(n % 2) continue; c = c+1; }";
  REQUIRE_NOTHROW(code.compile(code_text+3, &rest));
  REQUIRE_NOTHROW(code.exec(map));

  REQUIRE(map["c"].asInt() == 5);
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
  
  REQUIRE_NOTHROW(code.exec(map));
  REQUIRE(map["n"].asDouble() == 10);
  REQUIRE(map["b"].asDouble() == 9);
}

TEST_CASE("Code block with for and if statements", "[BlockStatement]") {
  const char* code =
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
  BlockStatement b;

  REQUIRE_NOTHROW(b.compile(code, &rest));
  REQUIRE_NOTHROW(b.exec(map));

  REQUIRE(map["result"].asDouble() == 120);
}

TEST_CASE("Build and evaluate VarStatements") {
  const char* rest = 0;
  const char* code =
    "var \n"
    "v1 = 1,"
    "\n"
    "v2 =\n"
    "2";

  TokenMap map;
  TokenMap child = map.getChild();
  VarStatement v;

  map["v1"] = 10;

  REQUIRE_NOTHROW(v.compile(code+3, &rest));
  REQUIRE_NOTHROW(v.exec(child));

  REQUIRE(map["v1"].asInt() == 10);
  REQUIRE(child["v1"].asInt() == 1);
  REQUIRE(child["v2"].asInt() == 2);

  rest = 0;
  code = "var v3";
  map["v3"] = 30;

  REQUIRE_NOTHROW(v.compile(code+3, &rest));
  REQUIRE_NOTHROW(v.exec(child));

  REQUIRE(*rest == '\0');

  // Test if the variable was declared on child scope:
  REQUIRE(child.map().count("v3") == 1);
}

TEST_CASE("Build and evaluate ScopedStatements") {
  const char* rest = 0;
  const char* code =
    "scoped {\n"
    "  var v1 = 1\n"
    "  var v2 = 2\n"
    "  v3 = v1 + v2\n"
    "}";

  TokenMap map;
  ScopedStatement s;

  map["v1"] = 10;
  map["v3"] = 30;

  REQUIRE_NOTHROW(s.compile(code+6, &rest));
  REQUIRE_NOTHROW(s.exec(map));

  REQUIRE(map["v1"].asInt() == 10);
  REQUIRE(map["v3"].asInt() == 3);
  REQUIRE(map.map().count("v2") == 0);
}

TEST_CASE("Build and evaluate UserFunctions", "[UserFunctions][FuncDeclaration]") {
  const char* rest = 0;
  const char* code_text = "function my_sum (num1, num2) { n1 = num1; return num1 + num2 }End();";
  FuncDeclaration decl;
  TokenMap map;

  REQUIRE_NOTHROW(decl.compile(code_text+8, &rest, map));
  REQUIRE_NOTHROW(decl.exec(map));
  REQUIRE(map["my_sum"].str() == "[Function: my_sum]");
  REQUIRE(rest != 0);
  REQUIRE(*rest == 'E');

  map["n1"] = packToken::None;
  REQUIRE(calculator::calculate("my_sum(3,4) ", map).asDouble() == 7);
  REQUIRE(map["n1"].asDouble() == 3);
  REQUIRE(calculator::calculate(" my_sum(1,1) ", map).asDouble() == 2);
  REQUIRE(map["n1"].asDouble() == 1);
  REQUIRE(calculator::calculate(" my_sum(0,4) ", map).asDouble() == 4);
  REQUIRE(map["n1"].asDouble() == 0);

  // Testing the return statement:
  code_text = "function f() { n1 = 10; return; n1 = 20 }End();";
  REQUIRE_NOTHROW(decl.compile(code_text+8, &rest, map));
  REQUIRE_NOTHROW(decl.exec(map));
  REQUIRE(calculator::calculate("f()", map).str() == "None");
  REQUIRE(map["n1"].asDouble() == 10);
}

TEST_CASE("Flow control statements") {
  const char* code =
    "{"
    "  outside = True;"
    "  function f() {"
    "    return True;"
    "    outside = False;"
    "  }"
    "  ret = f();"
    "  here = True;"
    "}";

  BlockStatement b;
  TokenMap vars;

  REQUIRE_NOTHROW(b.compile(code));
  REQUIRE_NOTHROW(b.exec(vars));
  REQUIRE(vars["outside"].asBool() == true);
  REQUIRE(vars["ret"].asBool() == true);
  REQUIRE(vars["here"].asBool() == true);

  code =
    "{"
    "  outside = True;"
    "  function f() {"
    "    for(n in range(1)) {"
    "      return True;"
    "    }"
    "    outside = False;"
    "  }"
    "  ret = f();"
    "  here = True;"
    "}";

  REQUIRE_NOTHROW(b.compile(code));
  REQUIRE_NOTHROW(b.exec(vars));
  REQUIRE(vars["outside"].asBool() == true);
  REQUIRE(vars["ret"].asBool() == true);
  REQUIRE(vars["here"].asBool() == true);

  code =
    "{"
    "  outside = True;"
    "  function f() {"
    // Finish should return from the function as would `return`
    "    for(n in range(1)) {"
    "      finish True;"
    "    }"
    "    outside = False;"
    "  }"
    "  ret = f();"
    "  here = True;"
    "}";

  REQUIRE_NOTHROW(b.compile(code));
  REQUIRE_NOTHROW(b.exec(vars));
  REQUIRE(vars["outside"].asBool() == true);
  REQUIRE(vars["ret"].asBool() == true);
  REQUIRE(vars["here"].asBool() == true);

  code =
    "{"
    "  outside = False;"
    "  function f() {"
    // Check that break and continue are not returning from the function:
    "    for(n in range(1)) {"
    "      break;"
    "    }"
    "    for(n in range(1)) {"
    "      continue;"
    "    }"
    "    outside = True;"
    "    return True;"
    "  }"
    "  ret = f();"
    "  here = True;"
    "}";

  REQUIRE_NOTHROW(b.compile(code));
  REQUIRE_NOTHROW(b.exec(vars));
  REQUIRE(vars["outside"].asBool() == true);
  REQUIRE(vars["ret"].asBool() == true);
  REQUIRE(vars["here"].asBool() == true);
}

// The `rpn()` function is to be used as a debug tool.
TEST_CASE("Test built-in functions and classes") {
  GlobalScope vars;
  REQUIRE_NOTHROW(calculator::calculate("str = rpn('a=2')", vars));
  // calculator c("str = rpn('a=2')", vars);

  REQUIRE(vars["str"].asString() == "calculator { RPN: [ a, 2, = ] }");

  const char* code =
  "{"
  "  L = list();"
  "  function F(val) {"
  "    for(arg in args)"
  "      L.push(arg);"
  "    return val;"
  "  }"
  "  lazy = new Lazy(F, 'v1', 'v2');"
  "}";

  BlockStatement b;
  REQUIRE_NOTHROW(b.compile(code));
  REQUIRE_NOTHROW(b.exec(vars));

  REQUIRE(calculator::calculate("lazy.exec()", vars).asString() == "v1");
  REQUIRE(vars["L"].asList().list().size() == 1);
  REQUIRE(vars["L"].str() == "[ \"v2\" ]");

  REQUIRE_NOTHROW(calculator::calculate("L = list()", vars));
  REQUIRE(calculator::calculate("lazy.exec('+', 2)", vars).asString() == "v1");
  REQUIRE(vars["L"].asList().list().size() == 3);
  REQUIRE(vars["L"].str() == "[ \"v2\", \"+\", 2 ]");

  code =
  "{"
  "  file = open('__test_file__.o', 'w');"
  "  file.write('hello\\nworld');"
  "  file.close();"
  "  "
  "  file = open('__test_file__.o');"
  "  lines = file.readlines();"
  "}";
  REQUIRE_NOTHROW(b.compile(code));
  REQUIRE_NOTHROW(b.exec(vars));

  REQUIRE(vars["lines"]->type == IT);
  Iterator* it = static_cast<Iterator*>(vars["lines"].token());
  packToken* p;
  REQUIRE_NOTHROW(p = it->next());
  REQUIRE(p != 0);
  REQUIRE(p->asString() == "hello");

  REQUIRE_NOTHROW(p = it->next());
  REQUIRE(p != 0);
  REQUIRE(p->asString() == "world");

  REQUIRE_NOTHROW(p = it->next());
  REQUIRE(p == 0);

  code = "L = list(list(1,2,3).reverse())";
  REQUIRE_NOTHROW(b.compile(code));
  REQUIRE_NOTHROW(b.exec(vars));
  REQUIRE(vars["L"].str() == "[ 3, 2, 1 ]");
}

TEST_CASE("Test usage of the `new` reserved word") {
  GlobalScope vars;
  const char* code =
    "{"
    "  a = map();"
    "  function init(a) {"
    "    this.value = a;"
    "  }"
    "  a.__init__ = init;"
    "  "
    "  b = new a(10);"
    "}";
  BlockStatement b;
  REQUIRE_NOTHROW(b.compile(code));
  REQUIRE_NOTHROW(b.exec(vars));
  REQUIRE(vars["b"]["value"] == 10);

  REQUIRE(calculator::calculate("c = new a()", vars));
  REQUIRE(vars["c"]["value"]->type == NONE);

  REQUIRE_THROWS(calculator::calculate("c = new a"));
  try {
    calculator::calculate("c = new a . b [' c ']");
  } catch (const syntax_error& e) {
    REQUIRE(e.what() == std::string("Expected '(' after `new a . b [' c ']`"));
  }
}

TEST_CASE("Test usage of the `function` reserved word") {
  GlobalScope vars;
  const char* code =
    "{"
    "  a = function() { return 'a'; }\n"
    "  b = function   (){ return 'b'; }\n"
    "  c = function c() { return 'c'; };"
    "  d = function d (){ return 'd'; }\n"
    // "  exe = function() { return 'run'; }();"
    "}";
  BlockStatement b;
  REQUIRE_NOTHROW(b.compile(code));
  REQUIRE_NOTHROW(b.exec(vars));
  REQUIRE(vars["a"]->type == FUNC);
  REQUIRE(vars["b"]->type == FUNC);
  REQUIRE(vars["c"]->type == FUNC);
  REQUIRE(vars["d"]->type == FUNC);
  // REQUIRE(vars["exe"].asString() == "run");
}
