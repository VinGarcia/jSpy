#include <iostream>
#include "catch.hpp"

#include "../cparse/shunting-yard.h"
#include "../cparse/shunting-yard-exceptions.h"
#include "../statements.h"
#include "../matcher.h"

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
    "  \"pattern\" if ( a == 3 ) { return 42 }\n"
    "  \"pattern\" if ( b == 3 ) { return 10 }\n"
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
    "  \"pattern\" if ( a == True ) { if (not_sure == 1) return 42; else finish 42}\n"
    "  \"pattern\" if ( b == True ) { return 10 }\n"
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
  code = "matcher M3 {\n  \"pattern\" { return 10 }\n}End()";
  REQUIRE_NOTHROW(m.compile(code+7, &code, vars));
  REQUIRE_NOTHROW(m.exec(vars));
  REQUIRE_NOTHROW(calculator::calculate("results = M3.exec('no_match')", vars));
  REQUIRE(calculator::calculate("results.len()", vars).asDouble() == 0);
  REQUIRE(vars["results"].str() == "[]");

  code = "matcher M3 {\n  \"pattern\" if (False) { return 10 }\n}End()";
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
    "    \"open (program)p;\" finish 'openning %s' % p\n"
    "  }"
    "}";
  BlockStatement b;

  REQUIRE_NOTHROW(b.compile(code, &code, vars));
  REQUIRE_NOTHROW(b.exec(vars));

  REQUIRE_NOTHROW(calculator::calculate("results = M1.exec('open firefox')", vars));
  REQUIRE(calculator::calculate("results.len()", vars).asDouble() == 1);
  REQUIRE(vars["results"].str() == "[ \"openning firefox\" ]");
}
