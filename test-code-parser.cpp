#include <iostream>
#include "catch.hpp"

#include "shunting-yard.h"
#include "code-parser.h"

void PREPARE_ENVIRONMENT() {

}

TEST_CASE("Build and evaluate block of code") {
  const char* rest = 0;
  const char* code_text = " a = 2; b = 3; c = a+b; }";
  TokenMap_t map, local;
  map["c"] = 0;

  CodeParser code(code_text, &rest);
  REQUIRE_NOTHROW(local = code.exec(&map));
  REQUIRE(local["a"].asDouble() == 2);
  REQUIRE(local["b"].asDouble() == 3);
  REQUIRE(map["c"].asDouble() == 5);
  REQUIRE(rest != 0);
  REQUIRE(*rest == '}');
}

TEST_CASE("Test the compile function") {
  const char* rest = 0;
  const char* code_text = " a = 2; b = 3; c = a+b; }";
  CodeParser code;
  TokenMap_t map, local;
  map["c"] = 0;

  code.compile(code_text, &rest);
  REQUIRE_NOTHROW(local = code.exec(&map));
  REQUIRE(local["a"].asDouble() == 2);
  REQUIRE(local["b"].asDouble() == 3);
  REQUIRE(map["c"].asDouble() == 5);
  REQUIRE(rest != 0);
  REQUIRE(*rest == '}');
}
