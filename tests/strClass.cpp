#include "../padrao.hpp"
#include <iostream>
#include "number.h"

#include "catch.hpp"
#include "catch_lower.hpp"

using namespace std;

/* * * * * START TEST strClass * * * * */

TEST_CASE("strClass", "[strClass]") {

  /*
   * Teste do strClass
   */

  #if NUMBER==strClass || NUMBER==1 || NUMBER==ALL
  {

    using namespace pMatch;
    
    string s("[a]bc[--]");
    
    strClass* sclass;
    strClass::iterator it;
    
    // Testes:
    
    cout << " * * * * * pMatch::strClass * * * * *\n\n";
    
    cout << "Testando o formato: \"[a]bc[--]\"" << endl << endl;
    
    cout << 1 << endl;
    cout << "teste abc[--]\n";
    sclass = new strClass("[a]bc[--]"); it = sclass->begin();
    cout << "      ";
    cout << it->str(); it++; cout << it->str(); it++; cout << it->str(); it++; cout << it->str();
    cout << endl << endl;
  
    cout << 2 << endl;
    cout << "teste abc[--]\n";
    sclass = new strClass(s); it = sclass->begin();
    cout << "      ";
    cout << it->str(); it++; cout << it->str(); it++; cout << it->str(); it++; cout << it->str();
    cout << endl << endl;
  
    cout << 3 << endl;
    cout << "teste abc[--]\n";
    strClass sclass1("[a]bc[--]"); it = sclass1.begin();
    cout << "      ";
    cout << it->str(); it++; cout << it->str(); it++; cout << it->str(); it++; cout << it->str();
    cout << endl << endl;
  
    cout << 4 << endl;
    cout << "teste abc[--]\n";
    strClass sclass2("[a]bc[--]"); it = sclass2.begin();
    cout << "      ";
    cout << it->str(); it++; cout << it->str(); it++; cout << it->str(); it++; cout << it->str();
    cout << endl << endl;
  
  }
  #endif
    
  /*
   * Teste do strClass::match()
   */

  #if NUMBER==match || NUMBER==2 || NUMBER==ALL
  {
    using namespace pMatch;
    
    string s("testando com afeto");
    
    uint pos;
    bool b;
    
    // Testes:
    
    cout << " * * * * * strClass::match * * * * *\n\n";
    
    cout << "Testando os formatos: \"a\" ou \"[abc]\"" << endl;
    
    cout << 1 << endl;
    cout << "teste 1\n";
    b = strClass("testa").match(s, pos=0);
    cout << "      " << b << endl<<endl;
    
    cout << 2 << endl;
    cout << "teste 0\n";
    b = strClass("esta").match(s, pos=0);
    cout << "      " << b << endl<<endl;
    
    cout << 3 << endl;
    cout << "teste 1\n";
    b = strClass("esta").match(s, pos=1);
    cout << "      " << b << endl<<endl;
    
    cout << 4 << endl;
    cout << "teste 1\n";
    b = strClass("[ t]a").match(s, pos=3);
    cout << "      " << b << endl<<endl;
    
    cout << 5 << endl;
    cout << "teste 1\n";
    b = strClass("[ t]a").match(s, pos=12);
    cout << "      " << b << endl<<endl;
    
    cout << 6 << endl;
    cout << "teste 1\n";
    b = strClass("[^t]a").match(s, pos=12);
    cout << "      " << b << endl<<endl;
    
  }
  #endif
   
  /*
   * Teste do strClass::find()
   *
   */
  #if NUMBER==find || NUMBER==3 || NUMBER==ALL
  {

    using namespace pMatch;
    
    string s("testando com carinho!");
    
    uint pos;
    tWord str;
    
    // Testes:
    
    cout << " * * * * * strClass::find() * * * * *\n\n";
    
    cout << 1 << endl;
    cout << "teste \"t\"-0\n";
    str = strClass("t").find(s, pos=0);
    cout << " str: \"" << str << "\"-" << str.start() << endl<<endl;
    
    cout << 2 << endl;
    cout << "teste \"ta\"-3\n";
    str = strClass("ta").find(s, pos=0);
    cout << " str: \"" << str << "\"-" << str.start()<< endl<<endl;
    
    cout << 3 << endl;
    cout << "teste \"nho!\"-17\n";
    str = strClass("nho!").find(s, pos=0);
    cout << " str: \"" << str << "\"-" << str.start()<< endl<<endl;
    
    cout << 4 << endl;
    cout << "teste \"com\"-9\n";
    str = strClass("[a-c][m-p][m-p]").find(s, pos=0);
    cout << " str: \"" << str << "\"-" << str.start()<< endl<<endl;
    
    cout << 5 << endl;
    cout << "teste \"\"-0\n";
    str = strClass("").find(s, pos=0);
    cout << " str: \"" << str << "\"-" << str.start()<< endl<<endl;
    
    cout << 6 << endl;
    cout << "teste \"com]\"-4\n";
    str = strClass("com]").find("0c2[com]", pos=0);
    cout << " str: \"" << str << "\"-" << str.start()<< endl<<endl;
    
    cout << 7 << endl;
    cout << "teste \"[com]\"-3\n";
    str = strClass("\\[com]").find("0c2[com]", pos=0);
    cout << " str: \"" << str << "\"-" << str.start()<< endl<<endl;
    
    cout << 8 << endl;
    cout << "teste \"2[com]\"-2\n";
    str = strClass("2\\[com]").find("0c2[com]", pos=0);
    cout << " str: \"" << str << "\"-" << str.start()<< endl<<endl;
    
    cout << "Testando inputs diferentes do param `pos`" << endl<<endl;
    
    cout << 9 << endl;
    cout << "teste \"t\"-3\n";
    str = strClass("t").find(s, pos=1);
    cout << " str: \"" << str << "\"-" << str.start()<< endl<<endl;
    
    cout << 10 << endl;
    cout << "teste \"\"-21\n";
    str = strClass("com").find(s, pos=14);
    cout << " str: \"" << str << "\"-" << str.start() << endl<<endl;
      
  }
  #endif
    
  /*
   * Teste do strClass::getClass
   */
  #if NUMBER==getClass || NUMBER==4 || NUMBER==ALL
  {
    using namespace pMatch;
    
    strClass s;
    uint start, pos;
    
    // Testes:
    
    cout << " * * * * * pMatch::strClass::getClass * * * * *\n\n";
    
    cout << 1 << endl;
    cout << "teste feijoada 0 8\n";
    s = strClass::getClass("feijoada", pos=start=0);
    cout << "      " << s.str() << " " << start << " " << pos <<endl<<endl;
    
    cout << 2 << endl;
    cout << "teste ijoada 2 8\n";
    s = strClass::getClass("feijoada", pos=start=2);
    cout << "      " << s.str() << " " << start << " " << pos <<endl<<endl;
    
    cout << 3 << endl;
    cout << "teste fei 0 3\n";
    s = strClass::getClass("fei(oada", pos=start=0);
    cout << "      " << s.str() << " " << start << " " << pos <<endl<<endl;
    
    cout << 4 << endl;
    cout << "teste fei(ada) 0 9\n";
    s = strClass::getClass("fei\\(ada)", pos=start=0);
    cout << "      " << s.str() << " " << start << " " << pos <<endl<<endl;
    
    cout << 5 << endl;
    cout << "teste  8 8\n";
    s = strClass::getClass("feijoada", pos=start=8);
    cout << "      " << s.str() << " " << start << " " << pos <<endl<<endl;
    
    cout << "* * * * * Iniciando testes de erros: * * * * * " << endl << endl;
    
    cout << 6 << endl;
    cout << "teste error\n";
    try{s = strClass::getClass("feijoada", pos=-1); cout<<"      ok";}
    catch(...){cout<<"      error";} cout<< endl<<endl;
    
    cout << 7 << endl;
    cout << "teste error\n";
    try{s = strClass::getClass("feijoada", pos=9); cout<<"      ok";}
    catch(...){cout<<"      error";} cout<< endl<<endl;
    
    cout << 8 << endl;
    cout << "teste ok\n";
    try{s = strClass::getClass("feijoada\\", pos=0); cout<<"      ok";}
    catch(...){cout<<"      error";} cout<< endl<<endl;

  }
  #endif
}
  
/* * * * * END TEST strClass * * * * */

