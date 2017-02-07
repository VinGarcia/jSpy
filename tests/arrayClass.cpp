#include <iostream>

#include "catch.hpp"
#include "../pattern.h"

using namespace std;

/* * * * * START TEST arrayClass * * * * */

TEST_CASE("arrayClass", "[arrayClass]") {

  /*
   * Teste do  arrayClass:
   */
  std::string expected;

  GIVEN("that it should build ok") {
    
    using namespace pattern;
    arrayClass ac;
    
    THEN("it should build with a given string") {
      expected = "\"testando\"";
      CHECK( expected == arrayClass("testando").str() );

      expected = "\"[Tt]estando\"";
      CHECK( expected == arrayClass("[Tt]estando").str() );

      expected = "\"[T-t]estando\"";
      CHECK( expected == arrayClass("[T-t]estando").str() );

      expected = "\"  testando  \"";
      CHECK( expected == arrayClass("  testando  ").str() );
    }

    THEN("it should build with nested blockClasses") {
      
      expected = "\"(testando);\"";
      CHECK( expected == arrayClass("(testando)").str() );

      expected = "\"(testando)nome;\"";
      CHECK( expected == arrayClass("( testando )nome").str() );

      expected = "\"(testando)nome;\"";
      CHECK( expected == arrayClass("( testando )nome;").str() );

      expected = "\"(testando)nome; \"";
      CHECK( expected == arrayClass("( testando )nome ").str() );
    }

    THEN("it should build with nested block and array Classes") {
      
      expected = "\"abra o (program_name)p;\"";
      CHECK( expected == arrayClass("abra o (program_name)p;").str() );

      expected = "\"(verb)v; o (program_name)p;\"";
      CHECK( expected == arrayClass("(verb)v o (program_name)p").str() );

      expected = "\"(verb)v; o (program_name)p; agora!\"";
      CHECK( expected == arrayClass("(verb)v o (program_name)p agora!").str() );

      expected = "\" (ab); \"";
      CHECK( expected == arrayClass(" (ab) ").str() );
      
      expected = "\" (a); (b)c;agora!\"";
      CHECK( expected == arrayClass(" (a) (b)c;agora!").str() );
    }
    
    THEN("it should build even with uncommon characters") {
    
      expected = "\"\t)(;:'\",\"";
      CHECK( expected == arrayClass("\t)\\(;:'\",").str() );
    }
    
    THEN("it should detect erros correctly") {
      cout << "  @Tests with errors (arrayClass)" << endl;
      cout << "    NOT DONE YET" << endl << endl;
    }
  }

  //
  // Teste arrayClass::match()
  //
  GIVEN("that it is a matcher") {
    using namespace pattern;
    arrayClass ac;
    bool bMatch;
    
    THEN("it should match unambiguous blockClass patterns") {
    
      expected = "teste";
      CHECK( 1 == (ac=arrayClass("teste")).match("teste",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "teste";
      CHECK( 1 == (ac=arrayClass("(\"teste\");")).match("teste",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "teste agora";
      CHECK( 1 == (ac=arrayClass("teste (\"agora\");")).match("teste agora",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "agora teste";
      CHECK( 1 == (ac=arrayClass("(\"agora\"); teste")).match("agora teste",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "";
      CHECK( 0 == (ac=arrayClass("e agora acabou")).match("e agora",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "";
      CHECK( 0 == (ac=arrayClass("e (\"agora\"); acabou")).match("e agora",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "e agora acabou";
      CHECK( 1 == (ac=arrayClass("e (\"agora\"); acabou")).match("e agora acabou",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "e agora e foda";
      CHECK( 1 ==(ac=arrayClass("e (\"agora\") e (\"foda\")")).match("e agora e foda",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "e agora e foda";
      CHECK( 1 ==(ac=arrayClass("(\"e\") agora (\"e\") foda")).match("e agora e foda",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "e agora e foda";
      CHECK( 1 ==(ac=arrayClass("(\"e \");(\"agora \");(\"e \")(\"foda\")")).match("e agora e foda",0) );
      CHECK( expected == ac.getMatch().str() );
    }
    
    THEN("it should match ambiguous block patterns") {
      expected = "teste|teste";
      CHECK( 1 ==(ac=arrayClass("(\"teste\",\"teste\")")).match("teste",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "teste|teste";
      CHECK( 1 ==(ac=arrayClass("(\"tes\",\"te\")(\"te\",\"ste\")")).match("teste",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "teste agora|teste agora";
      CHECK( 1 ==(ac=arrayClass("teste (\"ago\",\"ag\")(\"ra\",\"ora\")")).match("teste agora",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "teste agora mesmo|teste agora mesmo";
      CHECK( 1 ==(ac=arrayClass("teste (\"ago\",\"ag\")(\"ra\",\"ora\") mesmo")).match("teste agora mesmo",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "teste";
      CHECK( 1 ==(ac=arrayClass("(\"tes\",\"te\")(\"te\")")).match("teste",0) );
      CHECK( expected == ac.getMatch().str() );

      expected = "teste";
      CHECK( 1 ==(ac=arrayClass("(\"te\",\"tes\")(\"te\")")).match("teste",0) );
      CHECK( expected == ac.getMatch().str() );
    }

    THEN("it should extract named variables from text") {
    
      expected = "no_name: teste{v1: teste;}";
      CHECK( 1 ==(ac=arrayClass("(\"teste\")v1")).match("teste agora mesmo",0) );
      CHECK( expected == ac.var.str() );
      
      expected = "no_name: teste{v1: tes; v2: te;}";
      CHECK( 1 ==(ac=arrayClass("(\"tes\")v1(\"te\")v2;")).match("teste",0) );
      CHECK( expected == ac.var.str() );
      
      expected = "no_name: teste{v1: te; v2: ste;}|teste{v1: tes; v2: te;}";
      CHECK( 1 ==(ac=arrayClass("(\"tes\",\"te\")v1(\"te\",\"ste\")v2;")).match("teste",0) );
      CHECK( expected == ac.var.str() );

      expected = "no_name: teste{V: teste{v1: teste;};}";
      CHECK( 1 ==(ac=arrayClass("(\"(\"teste\")v1;\")V;")).match("teste",0) );
      CHECK( expected == ac.var.str() );

      expected = "no_name: teste{V: teste{v1: te; v2: ste;};}|teste{V: teste{v1: tes; v2: te;};}";
      CHECK( 1 ==(ac=arrayClass("(\"(\"tes\",\"te\")v1(\"te\",\"ste\")v2\")V;")).match("teste",0) );
      CHECK( expected == ac.var.str() );

      expected = "no_name: teste{v1: te; v2: ; v3: ste;}|teste{v1: tes; v2: ; v3: te;}|teste{v1: te; v2: s; v3: te;}";
      CHECK( 1 ==(ac=arrayClass("(\"tes\",\"te\")v1;(\"s\",\"\")v2;(\"te\",\"ste\")v3")).match("teste",0) );
      CHECK( expected == ac.var.str() );
      
      expected = "no_name: teste{V: est;}";
      CHECK( 1 ==(ac=arrayClass("t(\"est\")V;e")).match("teste",0) );
      CHECK( expected == ac.var.str() );
    }
    
    THEN("it should match cases with kleene operator") {

      cout << "  @Cases with objects and repetitions (arrayClass)" << endl;
      cout << "    INCOMPLETE" << endl << endl;

      expected = "no_name: 100";
      CHECK( 1 ==(ac=arrayClass("[0-9]*")).match("100",0,true) );
      CHECK( expected == ac.var.str() );
      
      expected = "no_name: 100{V: 100{[0]: 1; [1]: 0; [2]: 0;};}";
      CHECK( 1 ==(ac=arrayClass("(\"[0-9]\")V*")).match("100",0,true) );
      CHECK( expected == ac.var.str() );
      
      expected = "no_name: 100{V: 100{[0]: 1{v: 1;}; [1]: 0{v: 0;}; [2]: 0{v: 0;};};}";
      CHECK( 1 ==(ac=arrayClass("(\"(\"[0-9]\")v\")V*")).match("100",0,true) );
      CHECK( expected == ac.var.str() );
      
      expected = "no_name: 09{V: 09{[0]: 09{v1: 0; v2: 9;};};}";
      CHECK( 1 ==(ac=arrayClass("(\"(\"[0-9]\")v1(\"[0-9]\")v2\")V*")).match("09",0,true) );
      CHECK( expected == ac.var.str() );

    }
  }
}

/* * * * * END TEST arrayClass * * * * */

