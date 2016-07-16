#include <iostream>
#include "catch.hpp"

#include "shunting-yard.h"
#include "code-parser.h"

void PREPARE_ENVIRONMENT() {

}

TEST_CASE("Build and evaluate block of code") {
  const char* rest = 0;
  const char* code_text = " a = 2; b = 3; c = a+b; }";

  CodeParser code(code_text, &rest);
  REQUIRE_NOTHROW(code.exec());
  REQUIRE(code.local["c"].asDouble() == 5);
  REQUIRE(rest != 0);
  REQUIRE(*rest == '}');
}

TEST_CASE("Test the compile function") {
  const char* rest = 0;
  const char* code_text = " a = 2; b = 3; c = a+b; }";
  CodeParser code;

  code.compile(code_text, &rest);
  REQUIRE_NOTHROW(code.exec());
  REQUIRE(code.local["c"].asDouble() == 5);
  REQUIRE(rest != 0);
  REQUIRE(*rest == '}');
}
