#include <iostream>

#include "catch.hpp"
#include "../pattern.h"

using namespace std;
using namespace pattern;

/* * * * * START TEST arrayClass * * * * */

TEST_CASE("objectClass", "[objectClass]") {

  /*
   * Teste do objectClass:
   */

  GIVEN("That it should build ok") {

    // Teste do objectClass:
    using pattern::objectClass;
    
    objectClass oc;
    size_t pos;
    std::string expected;
    
    THEN("It should work with a single argument") {
    
      expected = "verbo";
      CHECK(objectClass("verbo").str() == expected);
      
      expected = "verbo";
      CHECK(objectClass("verbo      dois").str() == expected);
      
      expected = "verbo";
      CHECK(objectClass("    verbo      tres").str() == expected);
      
      expected = "ver";
      CHECK(objectClass("    ver(bo      quatro    ").str() == expected);

      expected = "ver";
      CHECK(objectClass("    ver,bo      quatro    ").str() == expected);
    }
    
    THEN("It should work as a slave parser") {
      expected = "verbo";
      CHECK(objectClass("verbo", pos=0).str() == expected);
      
      expected = "verbo";
      CHECK(objectClass("averbo", pos=1).str() == expected);
      
      expected = "verbo";
      CHECK(objectClass("  verbo  ", pos=1).str() == expected);
      
      expected = "verbo";
      CHECK(objectClass("verbo, verbula", pos=0).str() == expected);
      
      expected = "verbo";
      CHECK(objectClass("Verbo, verbo  ) amacada", pos).str() == expected);

      expected = "teste_com_espaco";
      CHECK(objectClass(" teste_com_espaco, teste_com_espaco_2 ", pos=0).str() == expected);
      CHECK(pos == 18);
      
      expected = "teste_com_espaco_2";
      CHECK(objectClass(" teste_com_espaco, teste_com_espaco_2 ", pos).str() == expected);
      CHECK_NOTHROW(objectClass(" teste_com_espaco, teste_com_espaco_2 ", pos=18));
      CHECK(pos == 38);
    }

    THEN("It should report errors correctly") {
      CHECK_NOTHROW(objectClass("Verb,o", pos=0));
      CHECK_NOTHROW(objectClass("Verb)o", pos=0));

      CHECK_THROWS(objectClass("Verb;o", pos=0));
      CHECK_THROWS(objectClass("Verb:o", pos=0));
      CHECK_THROWS(objectClass("Verb(o", pos=0));
      CHECK_THROWS(objectClass("Verb.!@#$%^&*-'\"\t\n o", pos=0));
    }
  }
    
  GIVEN("That it is a matcher") {
    
    using pattern::objectClass;

    objectClass oc("teste");
    
    THEN("It should match correctly") {

      REQUIRE_NOTHROW(oc.match("testando",0));
      CHECK(oc.getMatch().str() == "");
      
      objectClass::labels["teste"] = new strClass("testando");
      
      REQUIRE_NOTHROW(oc.match("testando",0));
      CHECK(oc.getMatch().str() == "testando");
      
      REQUIRE_NOTHROW(oc.match("testestando",3));
      CHECK(oc.getMatch().str() == "testando");
      
      REQUIRE_NOTHROW(oc.match("testestandoagora",3));
      CHECK(oc.getMatch().str() == "testando");
      
      std::cout << "  @objectClass - multiple adds to same label" << std::endl;
      std::cout << "    Currently it overwrites the older label" << std::endl;
      std::cout << "    In future it should just append to the label list"
        << std::endl;
      
      delete objectClass::labels["teste"];
      objectClass::labels["teste"] = new blockClass("(\"test\",\"testando\");");
      
      REQUIRE_NOTHROW(oc.match("testestandoagora",3));
      CHECK(oc.getMatch().str() == "testando|test");
    }
  }
}

/* * * * * END TEST objectClass * * * * */
