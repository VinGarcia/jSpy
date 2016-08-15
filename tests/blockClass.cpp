#include <iostream>

#include "catch.hpp"
#include "../pattern.h"

using namespace std;

TEST_CASE("blockClass", "[blockClass]") {

  GIVEN("that it should build ok") {
    using namespace pMatch;
    blockClass bc;
    
    cout << " * * * * * TESTE blockClass(string) * * * * *\n\n";
    
    cout << "  @Testes com objetos:" << endl << endl;
    
    try{
    cout << 1.1 << endl;
    cout << "Teste (teste);" << endl;
    bc = blockClass("(teste);");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 1.2 << endl;
    cout << "Teste (teste)*" << endl;
    bc = blockClass("(teste)*");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 2.1 << endl;
    cout << "Teste (teste)batata;" << endl;
    bc = blockClass("(teste)batata;");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 2.2 << endl;
    cout << "Teste (teste)batata*" << endl;
    bc = blockClass("(teste)batata*");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 3 << endl;
    cout << "Teste (teste);" << endl;
    bc = blockClass("(   teste   );");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 4 << endl;
    cout << "Teste (teste1,teste2,teste1000)nome;" << endl;
    bc = blockClass("( teste1 , teste2 , teste1000 )nome;");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 5 << endl;
    cout << "Teste (teste com espaco,teste com espaco 2)n0m3;" << endl;
    bc = blockClass("( teste com espaco, teste com espaco 2 )n0m3;");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    cout << "  @Testes com array`s:" << endl << endl;
    
    try{
    cout << 6 << endl;
    cout << "Teste (\"teste simples\")nome;" << endl;
    bc = blockClass("(\"teste simples\")nome;");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 7.1 << endl;
    cout << "Teste (\"teste simples\",\"teste composto\")nome;" << endl;
    bc = blockClass("(   \"teste simples\"  ,   \"teste composto\")nome;");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 7.2 << endl;
    cout << "Teste (\"teste simples\",teste composto)nome;" << endl;
    bc = blockClass("(   \"teste simples\"  ,   teste composto  )nome;");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 7.3 << endl;
    cout << "Teste (teste simples,\"teste composto\")nome;" << endl;
    bc = blockClass("(   teste simples  ,   \"teste composto\"  )nome;");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 8 << endl;
    cout << "Teste (\"abra (objeto); simples\")nome;" << endl;
    bc = blockClass("(\"abra (objeto) simples\")nome;");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    cout << "  @Testes com recursão:" << endl << endl;
    
    try{
    cout << 9 << endl;
    cout << "Teste (\"abra (objeto); simples\")nome;" << endl;
    bc = blockClass("(\"abra (objeto) simples\")nome;");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 10 << endl;
    cout << "Teste (\"abra (\" um (objeto); bem\"); complexo\")nome;" << endl;
    bc = blockClass("(\"abra (\" um (objeto) bem\") complexo\")nome;");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 11 << endl;
    cout << "Teste (\"complexidade (\"esta (\"(ficando);\"); bem\"); doida\");\n";
    bc = blockClass("(\"complexidade (\"esta (\"(ficando)\") bem\") doida\");");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    cout << "  @Testes terminados em ' ' e '\\0':" << endl << endl;
    
    try{
    cout << 12 << endl;
    cout << "Teste (teste simples)nome;" << endl;
    bc = blockClass("(teste simples)nome ");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 13 << endl;
    cout << "Teste (teste simples)nome;" << endl;
    bc = blockClass("(teste simples)nome");
    cout << "      " << bc.str() << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    cout << "  @Testes com erros" << endl << endl;
    
    try{
    cout << 14 << endl;
    cout << "Teste false" << endl;
    bc = blockClass("(   \"test\"  ,   \"testa\"  )");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl; }
    
    try{
    cout << 15 << endl;
    cout << "Teste true" << endl;
    bc = blockClass(" (   \"test\"  ,   \"testa\"  )");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 16 << endl;
    cout << "Teste true" << endl;
    bc = blockClass(" (   \"test\"  ,   \"testa\"  ) ");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 17 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\"\"testa\")");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 18 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", :bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 19 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", (bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 20 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", )bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 21 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", ()bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 22 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", b()bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 23 << endl;
    cout << "Teste false" << endl;
    bc = blockClass("(\"test:\\(\\)\", bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 24.1 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\"e, bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 24.2 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\" e, bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 25.1 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\"\"e\", bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 25.2 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\" \"e\", bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 26.1 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\",, bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 26.2 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\",  , bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 27.1 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(,\"test\", bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 27.2 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(  ,\"test\", bla)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 28.1 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", bla,)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 28.2 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", bla, )");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 29 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", b:la)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 30 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", b\\)la)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 31 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", b\\(la)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 32.1 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", b\"la)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 32.2 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", b\"\"la)");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 33 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", bla)10");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
    
    try{
    cout << 34 << endl;
    cout << "Teste true" << endl;
    bc = blockClass("(\"test\", bla)!");
    cout << "      " << "false" << endl << endl;
    }catch(const char* c){ cout << "      " << "true" << endl << endl; }
  }

  GIVEN("that it should match ok") {
    using namespace pMatch;
    blockClass bc;
    bool bMatch;
    
    cout << " * * * * * TESTE blockClass::match() * * * * *\n\n";
    
    cout << "  @Testes com objetos:" << endl << endl;
    
    try{
    cout << 1 << endl;
    cout << "Teste 1 \"teste\"" << endl;
    bMatch = (bc=blockClass("(\"teste\");")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }
    
    try{
    cout << 2 << endl;
    cout << "Teste 1 \"teste\"" << endl;
    bMatch = (bc=blockClass("(\"teste\");")).match("testei",0);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 3 << endl;
    cout << "Teste 1 \"testa\"" << endl;
    bMatch = (bc=blockClass("(\"testa\");")).match("agora testando",6);
    cout << "      " << bMatch << " \"" << bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 4 << endl;
    cout << "Teste 1 \"test|testa\"" << endl;
    bMatch = (bc=blockClass("(\"testa\",\"test\");")).match("agora testando",6);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 5 << endl;
    cout << "Teste 1 \"testa|test\"" << endl;
    bMatch = (bc=blockClass("(\"test\",\"testa\");")).match("agora testando",6);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 6 << endl;
    cout << "Teste 1 \"te|testa|test\"" << endl;
    bMatch = (bc=blockClass("(\"test\", \"testa\" ,\"te\");")).match("agora testando",6);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    cout << "  @Testes sem match (total e parcial):" << endl << endl;
    
    try{
    cout << 7 << endl;
    cout << "Teste 0 \"\"" << endl;
    bMatch = (bc=blockClass("(\"test\", \"testa\" ,\"te\");")).match("agora testando",7);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 8 << endl;
    cout << "Teste 0 \"\"" << endl;
    bMatch = (bc=blockClass("(\"test\",\"testa\",\"te\");")).match("agora testando",5);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 9 << endl;
    cout << "Teste 1 \"te|test\"" << endl;
    bMatch = (bc=blockClass("(\"test\",\"testa\",\"te\");")).match("agora test",6);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 10 << endl;
    cout << "Teste 0 \"\"" << endl;
    bMatch = (bc=blockClass("(\"test\",\"testa\");")).match("agora tes",6);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    cout << "  @Testes com o repeater ('*'):" << endl << endl;
    
    try{
    cout << 11 << endl;
    cout << "Teste 1 \"|tes\"" << endl;
    bMatch = (bc=blockClass("(\"tes\")*")).match("tes",0);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 12 << endl;
    cout << "Teste 1 \"|tes\"" << endl;
    bMatch = (bc=blockClass("(\"tes\")*")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 13 << endl;
    cout << "Teste 1 \"|tes|testes\"" << endl;
    bMatch = (bc=blockClass("(\"tes\")*")).match("testes",0);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 14 << endl;
    cout << "Teste 1 \"|tes|testes|testestes\"" << endl;
    bMatch = (bc=blockClass("(\"tes\")*")).match("testesteste",0);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 15 << endl;
    cout << "Teste 1 \"|te|tes|teste\"" << endl;
    bMatch = (bc=blockClass("(\"tes\",\"te\")*")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 16 << endl;
    cout << "Teste 1 \"|te\"" << endl;
    bMatch = (bc=blockClass("(\"tes\",\"te\")*")).match("teste",3);
    cout <<"      "<<bMatch<<" \""<< bc.getMatch().str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    cout << "  @Testes com variaveis:" << endl << endl;
    
    try{
    cout << 17 << endl;
    cout << "Teste 1 \"var: teste\"" << endl;
    bMatch = (bc=blockClass("(\"teste\")var;")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< bc.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 17.1 << endl;
    cout << "Teste 1 \"var: teste{v: s;}\"" << endl;
    bMatch = (bc=blockClass("(\"te(\"s\")v;te\")var;")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< bc.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 17.2 << endl;
    cout << "Teste 1 \"var: teste{v1: est{v2: s;};}\"" << endl;
    bMatch =(bc=blockClass("(\"t(\"e(\"s\")v2;t\")v1;e\")var;")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< bc.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 18 << endl;
    cout << "Teste 1 \"var: |te{[0]: te;}|tes{[0]: tes;}|teste{[0]: tes; [1]: te;}\"" << endl;
    bMatch = (bc=blockClass("(\"tes\",\"te\")var*")).match("teste",0);
    cout <<"      "<<bMatch<<" \""<< bc.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

    try{
    cout << 18.1 << endl;
    cout << "Teste 1 \"var: |tas{[0]: tas{v: a;};}|tastes{[0]: tas{v: a;}; [1]: tes{v: e;};}|tastestis{[0]: tas{v: a;}; [1]: tes{v: e;}; [2]: tis{v: i;};}\"" << endl;
    bMatch = (bc=blockClass("(\"t(\"[aei]\")v;s\")var*")).match("tastestis",0);
    cout <<"      "<<bMatch<<" \""<< bc.var.str() << '"' << endl << endl;
    }catch(const char* c){ cout << string("error: ") + c << endl; }

  }
}

/* * * * * END TEST blockClass * * * * */

