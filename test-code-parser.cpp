#include <iostream>
#include "catch.hpp"

#include "shunting-yard.h"
#include "statements.h"
#include "matcher.h"

void PREPARE_ENVIRONMENT() {

}

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
  REQUIRE_NOTHROW(code.exec(map));

  REQUIRE(map["result"].asDouble() == 120);
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
}

// The toRPN function is to be used as a debug tool.
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
  "  lazy = new(Lazy, F, 'v1', 'v2');"
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
}

TEST_CASE("Test usage of the `new` function") {
  GlobalScope vars;
  const char* code =
    "{"
    "  a = map();"
    "  function init(a) {"
    "    this.value = a;"
    "  }"
    "  a.__init__ = init;"
    "  "
    "  b = new(a,10);"
    "}";
  BlockStatement b;
  REQUIRE_NOTHROW(b.compile(code));
  REQUIRE_NOTHROW(b.exec(vars));
  REQUIRE(vars["b"]["value"] == 10);
}

TEST_CASE("Test Hook parser class") {
  GlobalScope vars;
  vars["a"] = 2;
  vars["resp"] = packToken::None;
  const char* code = "  \"pattern\" if ( a == 3 ) { resp = 42; }End(); ";
  Hook h;

  REQUIRE_NOTHROW(h.compile(code, &code, vars));
  REQUIRE(*code == 'E');

  REQUIRE(h.expr.str() == "\"pattern\"");
  REQUIRE(h.cond.eval(vars).asBool() == false);
  REQUIRE_NOTHROW(h.body.exec(vars));
  REQUIRE(vars["resp"].asDouble() == 42);

  code = "  \"pattern\"{ resp = 43; }End(); ";
  REQUIRE_NOTHROW(h.compile(code, &code, vars));
  REQUIRE(*code == 'E');

  REQUIRE(h.expr.str() == "\"pattern\"");
  REQUIRE(h.cond.eval(vars).asBool() == true);
  REQUIRE_NOTHROW(h.body.exec(vars));
  REQUIRE(vars["resp"].asDouble() == 43);

  code = "\"pattern\";End() ";
  REQUIRE_NOTHROW(h.compile(code, &code, vars));
  REQUIRE(*code == 'E');

  REQUIRE(h.expr.str() == "\"pattern\"");
  REQUIRE(h.cond.eval(vars).asBool() == true);
  REQUIRE_NOTHROW(h.body.exec(vars));
}

// TODO(VinGarcia): This test causes memory leak from problem in the pattern.cpp file.
//                  We must fix it.
TEST_CASE("Testing the getIterator and traverse function") {
  GlobalScope vars;
  vars["a"] = 2;
  vars["resp"] = packToken::None;
  const char* code = "  \"pattern\" if ( a == 3 ) { resp = 42; }End(); ";
  Hook h;
  Iterator* it;

  REQUIRE_NOTHROW(h.compile(code, &code, vars));
  REQUIRE_NOTHROW(it = h.getIterator("pattern"));
  REQUIRE(it->next()->str() == "{ \"text\": \"pattern\" }");
  REQUIRE(it->next() == NULL);

  code = "  \"my (\"foo\", \"foo\")b;\" if ( a == 3 ) { resp = 42; }End(); ";
  REQUIRE_NOTHROW(h.compile(code, &code, vars));
  REQUIRE_NOTHROW(it = h.getIterator("my foo"));
  // TODO(VinGarcia): The map is alphanumerically ordered as are all std::maps.
  // maybe we wanted to make it to print in insertion order as does Javascript?
  // It would require a new implementation of a map.
  REQUIRE(it->next()->str() == "{ \"b\": \"foo\", \"text\": \"my foo\" }");
  REQUIRE(it->next()->str() == "{ \"b\": \"foo\", \"text\": \"my foo\" }");
  REQUIRE(it->next() == NULL);
}

TEST_CASE("Test MatcherDeclaration class") {
  GlobalScope vars;
  const char* code = "matcher name \"pattern\" if ( a == 3 ) { resp = 42 }End();";
  MatcherDeclaration m;

  REQUIRE_NOTHROW(m.compile(code+7, &code, vars));
  REQUIRE(*code == 'E');
  REQUIRE(m.name == "name");
  REQUIRE(m.hooks.list().size() == 1);

  code =
    "matcher name2 {\n"
    "  \"pattern\" if ( a == 3 ) { resp = 42 }\n"
    "  \"p2\" { resp = 10 }\n"
    "}End()";

  REQUIRE_NOTHROW(m.compile(code+7, &code, vars));
  REQUIRE(*code == 'E');
  REQUIRE(m.name == "name2");
  REQUIRE(m.hooks.list().size() == 2);
}

TEST_CASE("Test MatcherDeclaration class execution") {
  GlobalScope vars;
  const char* code =
    "matcher name2 {\n"
    "  \"pattern\" if ( a == 3 ) { yield 42 }\n"
    "  \"pattern\" if ( b == 3 ) { yield 10 }\n"
    "}End()";
  MatcherDeclaration m;

  REQUIRE_NOTHROW(m.compile(code+7, &code, vars));
  REQUIRE(*code == 'E');

  REQUIRE_NOTHROW(m.exec(vars));
  REQUIRE(calculator::calculate("name2.instanceof(Matcher)", vars).asBool() == true);
  REQUIRE(calculator::calculate("name2.hooks.len()", vars).asDouble() == 2);
}

TEST_CASE("Test Matcher built-in class execution") {
  GlobalScope vars;
  const char* code =
    "matcher M1 {\n"
    "  \"pattern\" if ( a == True ) { if (not_sure == 1) yield 42; else return 42}\n"
    "  \"pattern\" if ( b == True ) { yield 10 }\n"
    "}End()";
  MatcherDeclaration m;

  REQUIRE_NOTHROW(m.compile(code+7, &code, vars));
  REQUIRE(*code == 'E');

  REQUIRE_NOTHROW(m.exec(vars));
  REQUIRE(calculator::calculate("M1.instanceof(Matcher)", vars).asBool() == true);
  REQUIRE(calculator::calculate("M1.hooks.len()", vars).asDouble() == 2);

  vars["a"] = true;
  vars["b"] = true;
  vars["not_sure"] = true;
  REQUIRE_NOTHROW(calculator::calculate("results = M1.exec('pattern')", vars));
  REQUIRE(calculator::calculate("results.len()", vars).asDouble() == 2);
  REQUIRE(vars["results"].str() == "[ 42, 10 ]");

  vars["a"] = true;
  vars["b"] = false;
  vars["not_sure"] = true;
  REQUIRE_NOTHROW(calculator::calculate("results = M1.exec('pattern')", vars));
  REQUIRE(calculator::calculate("results.len()", vars).asDouble() == 1);
  REQUIRE(vars["results"].str() == "[ 42 ]");

  vars["a"] = true;
  vars["b"] = true;
  vars["not_sure"] = false;
  REQUIRE_NOTHROW(calculator::calculate("results = M1.exec('pattern')", vars));
  REQUIRE(calculator::calculate("results.len()", vars).asDouble() == 1);
  REQUIRE(vars["results"].str() == "[ 42 ]");

  // Test what happens when no match is found:
  vars["a"] = true;
  code = "matcher M3 {\n  \"pattern\" { yield 10 }\n}End()";
  REQUIRE_NOTHROW(m.compile(code+7, &code, vars));
  REQUIRE_NOTHROW(m.exec(vars));
  REQUIRE_NOTHROW(calculator::calculate("results = M3.exec('no_match')", vars));
  REQUIRE(calculator::calculate("results.len()", vars).asDouble() == 0);
  REQUIRE(vars["results"].str() == "[]");

  code = "matcher M3 {\n  \"pattern\" if (False) { yield 10 }\n}End()";
  REQUIRE_NOTHROW(m.compile(code+7, &code, vars));
  REQUIRE_NOTHROW(m.exec(vars));
  REQUIRE_NOTHROW(calculator::calculate("results = M3.exec('pattern')", vars));
  REQUIRE(calculator::calculate("results.len()", vars).asDouble() == 0);
  REQUIRE(vars["results"].str() == "[]");
}

TEST_CASE("Test Matchers cross reference on patterns") {
  GlobalScope vars;
  const char* code =
    "{"
    "  matcher program {\n"
    "    \"firefox\";"
    "    \"chrome\";"
    "  }"
    "  "
    "  matcher M1 {\n"
    "    \"open (program)p;\" return 'openning %s' % p\n"
    "  }"
    "}";
  BlockStatement b;

  REQUIRE_NOTHROW(b.compile(code, &code, vars));
  REQUIRE_NOTHROW(b.exec(vars));

  REQUIRE_NOTHROW(calculator::calculate("results = M1.exec('open firefox')", vars));
  REQUIRE(calculator::calculate("results.len()", vars).asDouble() == 1);
  REQUIRE(vars["results"].str() == "[ \"openning firefox\" ]");
}
