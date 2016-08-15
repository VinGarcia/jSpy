#include <iostream>

#include "catch.hpp"
#include "../pattern.h"

using namespace std;
std::string expected;

TEST_CASE("blockClass", "[blockClass]") {

  GIVEN("that it should build ok") {
    using namespace pMatch;
    blockClass bc;

    THEN("It should work with references") {
      expected = "(teste);";
      REQUIRE(blockClass("(teste);").str() == expected);

      expected = "(teste)*";
      REQUIRE(blockClass("(teste)*").str() == expected);

      expected = "(teste)batata;";
      REQUIRE(blockClass("(teste)batata;").str() == expected);

      expected = "(teste)batata*";
      REQUIRE(blockClass("(teste)batata*").str() == expected);

      expected = "(teste);";
      REQUIRE(blockClass("(   teste   );").str() == expected);

      expected = "(teste1,teste2,teste1000)nome;";
      REQUIRE(blockClass("( teste1 , teste2 , teste1000 )nome;").str() == expected);
    }

    THEN("It should work with literal expressions") {
      expected = "(\"teste simples\")nome;";
      REQUIRE(blockClass("(\"teste simples\")nome;").str() == expected);

      expected = "(\"teste simples\",\"teste composto\")nome;";
      REQUIRE(blockClass("(   \"teste simples\"  ,   \"teste composto\")nome;").str() == expected);

      expected = "(\"teste simples\",teste_composto)nome;";
      REQUIRE(blockClass("(   \"teste simples\"  ,   teste_composto  )nome;").str() == expected);

      expected = "(teste_simples,\"teste composto\")nome;";
      REQUIRE(blockClass("(   teste_simples  ,   \"teste composto\"  )nome;").str() == expected);

      expected = "(\"abra (objeto); simples\")nome;";
      REQUIRE(blockClass("(\"abra (objeto) simples\")nome;").str() == expected);
    }

    THEN("it should work with nested expressions") {
      expected = "(\"abra (objeto); simples\")nome;";
      REQUIRE(blockClass("(\"abra (objeto) simples\")nome;").str() == expected);

      expected = "(\"abra (\" um (objeto); bem\"); complexo\")nome;";
      REQUIRE(blockClass("(\"abra (\" um (objeto) bem\") complexo\")nome;").str() == expected);

      expected = "(\"complexidade (\"esta (\"(ficando);\"); bem\"); doida\");";
      REQUIRE(blockClass("(\"complexidade (\"esta (\"(ficando)\") bem\") doida\");").str() == expected);
    }

    THEN("It should work with ' ' and '\\0' as delimiters") {
      expected = "(teste_simples)nome;";
      REQUIRE(blockClass("(teste_simples)nome ").str() == expected);

      expected = "(teste_simples)nome;";
      REQUIRE(blockClass("(teste_simples)nome").str() == expected);
    }

    THEN("It should report errors correctly") {

      REQUIRE_NOTHROW(blockClass("(   \"test\"  ,   \"testa\"  )"));

      REQUIRE_THROWS(blockClass(" (   \"test\"  ,   \"testa\"  )"));

      REQUIRE_THROWS(blockClass(" (   \"test\"  ,   \"testa\"  ) "));

      REQUIRE_THROWS(blockClass("(\"test\"\"testa\")"));

      REQUIRE_THROWS(blockClass("(\"test\", :bla)"));

      REQUIRE_THROWS(blockClass("(\"test\", (bla)"));

      REQUIRE_THROWS(blockClass("(\"test\", )bla)"));

      REQUIRE_THROWS(blockClass("(\"test\", ()bla)"));

      REQUIRE_THROWS(blockClass("(\"test\", b()bla)"));

      REQUIRE_NOTHROW(blockClass("(\"test:\\(\\)\", bla)"));

      REQUIRE_THROWS(blockClass("(\"test\"e, bla)"));

      REQUIRE_THROWS(blockClass("(\"test\" e, bla)"));

      REQUIRE_THROWS(blockClass("(\"test\"\"e\", bla)"));

      REQUIRE_THROWS(blockClass("(\"test\" \"e\", bla)"));

      REQUIRE_THROWS(blockClass("(\"test\",, bla)"));

      REQUIRE_THROWS(blockClass("(\"test\",  , bla)"));

      REQUIRE_THROWS(blockClass("(,\"test\", bla)"));

      REQUIRE_THROWS(blockClass("(  ,\"test\", bla)"));

      REQUIRE_THROWS(blockClass("(\"test\", bla,)"));

      REQUIRE_THROWS(blockClass("(\"test\", bla, )"));

      REQUIRE_THROWS(blockClass("(\"test\", b:la)"));

      REQUIRE_THROWS(blockClass("(\"test\", b\\)la)"));

      REQUIRE_THROWS(blockClass("(\"test\", b\\(la)"));

      REQUIRE_THROWS(blockClass("(\"test\", b\"la)"));

      REQUIRE_THROWS(blockClass("(\"test\", b\"\"la)"));

      REQUIRE_THROWS(blockClass("(\"test\", bla)10"));

      REQUIRE_THROWS(blockClass("(\"test\", bla)!"));
    }
  }

  GIVEN("that it should match ok") {
    using namespace pMatch;
    blockClass bc;
    bool bMatch;

    THEN("It should match disjunctions of literal patterns") {
      expected = "teste";
      REQUIRE_NOTHROW(bc = blockClass("(\"teste\");"));
      REQUIRE(bc.match("teste",0) == true);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "teste";
      REQUIRE_NOTHROW(bc = blockClass("(\"teste\");"));
      REQUIRE(bc.match("testei",0) == true);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "testa";
      REQUIRE_NOTHROW(bc = blockClass("(\"testa\");"));
      REQUIRE(bc.match("agora testando",6) == true);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "test|testa";
      REQUIRE_NOTHROW(bc = blockClass("(\"testa\",\"test\");"));
      REQUIRE(bc.match("agora testando",6) == true);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "testa|test";
      REQUIRE_NOTHROW(bc = blockClass("(\"test\",\"testa\");"));
      REQUIRE(bc.match("agora testando",6) == true);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "te|testa|test";
      REQUIRE_NOTHROW(bc = blockClass("(\"test\", \"testa\" ,\"te\");"));
      REQUIRE(bc.match("agora testando",6) == true);
      REQUIRE(bc.getMatch().str() == expected);
    }

    THEN("It should not match any text") {
      expected = "";
      REQUIRE_NOTHROW(bc = blockClass("(\"test\", \"testa\" ,\"te\");"));
      REQUIRE(bc.match("agora testando",7) == false);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "";
      REQUIRE_NOTHROW(bc = blockClass("(\"test\",\"testa\",\"te\");"));
      REQUIRE(bc.match("agora testando",5) == false);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "te|test";
      REQUIRE_NOTHROW(bc = blockClass("(\"test\",\"testa\",\"te\");"));
      REQUIRE(bc.match("agora test",6) == true);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "";
      REQUIRE_NOTHROW(bc = blockClass("(\"test\",\"testa\");"));
      REQUIRE(bc.match("agora tes",6) == false);
      REQUIRE(bc.getMatch().str() == expected);
    }

    THEN("It should match patterns with the kleene star '*'") {
      expected = "|tes";
      REQUIRE_NOTHROW(bc = blockClass("(\"tes\")*"));
      REQUIRE(bc.match("tes",0) == true);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "|tes";
      REQUIRE_NOTHROW(bc = blockClass("(\"tes\")*"));
      REQUIRE(bc.match("teste",0) == true);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "|tes|testes";
      REQUIRE_NOTHROW(bc = blockClass("(\"tes\")*"));
      REQUIRE(bc.match("testes",0) == true);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "|tes|testes|testestes";
      REQUIRE_NOTHROW(bc = blockClass("(\"tes\")*"));
      REQUIRE(bc.match("testesteste",0) == true);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "|te|tes|teste";
      REQUIRE_NOTHROW(bc = blockClass("(\"tes\",\"te\")*"));
      REQUIRE(bc.match("teste",0) == true);
      REQUIRE(bc.getMatch().str() == expected);

      expected = "|te";
      REQUIRE_NOTHROW(bc = blockClass("(\"tes\",\"te\")*"));
      REQUIRE(bc.match("teste",3) == true);
      REQUIRE(bc.getMatch().str() == expected);
    }

    THEN("It should save the matched text on variables correctly") {
      expected = "var: teste";
      REQUIRE_NOTHROW(bc = blockClass("(\"teste\")var;"));
      REQUIRE(bc.match("teste",0) == true);
      REQUIRE(bc.var.str() == expected);

      expected = "var: teste{v: s;}";
      REQUIRE_NOTHROW(bc = blockClass("(\"te(\"s\")v;te\")var;"));
      REQUIRE(bc.match("teste",0) == true);
      REQUIRE(bc.var.str() == expected);

      expected = "var: teste{v1: est{v2: s;};}";
      REQUIRE_NOTHROW(bc = blockClass("(\"t(\"e(\"s\")v2;t\")v1;e\")var;"));
      REQUIRE(bc.match("teste",0) == true);
      REQUIRE(bc.var.str() == expected);

      expected = "var: |te{[0]: te;}|tes{[0]: tes;}|teste{[0]: tes; [1]: te;}";
      REQUIRE_NOTHROW(bc = blockClass("(\"tes\",\"te\")var*"));
      REQUIRE(bc.match("teste",0) == true);
      REQUIRE(bc.var.str() == expected);

      expected = "var: |tas{[0]: tas{v: a;};}|tastes{[0]: tas{v: a;}; [1]: tes{v: e;};}|tastestis{[0]: tas{v: a;}; [1]: tes{v: e;}; [2]: tis{v: i;};}";
      REQUIRE_NOTHROW(bc = blockClass("(\"t(\"[aei]\")v;s\")var*"));
      REQUIRE(bc.match("tastestis",0) == true);
      REQUIRE(bc.var.str() == expected);
    }
  }
}
