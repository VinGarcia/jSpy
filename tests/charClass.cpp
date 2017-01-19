#include "../padrao.hpp"
#include <iostream>
#include "number.h"

#include "catch.hpp"
#include "catch_lower.hpp"

using namespace std;

/* * * * * START TEST charClass * * * * */

TEST_CASE("charClass", "[charClass]") {
  // Teste da Construtora do charClass
  #if NUMBER==1 || NUMBER==charClass || NUMBER==ALL
  {   
    using namespace std;
    using namespace pMatch;
    
    cout << " * * * * * CharClass * * * * *\n\n";
    cout << "Testando casos de erro: \"[abc[\"" << endl;
    
    cout << "  Casos Simples:" << endl;
    
    cout << 1 << endl;
    cout << "teste Erro True\n";
    cout << "      Erro? ";
    try { charClass("[]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 2 << endl;
    cout << "teste Erro True\n";
    cout << "      Erro? ";
    try { charClass("[^]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 3 << endl;
    cout << "teste Erro True\n";
    cout << "      Erro? ";
    try { charClass("[abc["); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 4 << endl;
    cout << "teste Erro True\n";
    cout << "      Erro? ";
    try { charClass("]abc]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << "  Casos do tipo: \"[abc]def]\":" << endl<<endl;
    
    cout << 5.1 << endl;
    cout << "teste Erro True\n";
    cout << "      Erro? ";
    try { charClass("[abc]def]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 5.2 << endl;
    cout << "teste Erro False\n";
    cout << "      Erro? ";
    try { charClass("[]abcdef]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 5.3 << endl;
    cout << "teste Erro True\n";
    cout << "      Erro? ";
    try { charClass("[^abc]def]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 5.4 << endl;
    cout << "teste Erro False\n";
    cout << "      Erro? ";
    try { charClass("[^]abcdef]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 5.5 << endl;
    cout << "teste Erro True\n";
    cout << "      Erro? ";
    try { charClass("[a]]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << "  Casos do tipo: \"[a--b]\":" << endl<<endl;
    
    cout << 6.1 << endl;
    cout << "teste Erro True\n";
    cout << "      Erro? ";
    try { charClass("[a--b]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 6.2 << endl;
    cout << "teste Erro False\n";
    cout << "      Erro? ";
    try { charClass("[--b]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 6.3 << endl;
    cout << "teste Erro False\n";
    cout << "      Erro? ";
    try { charClass("[a--]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 6.4 << endl;
    cout << "teste Erro True\n";
    cout << "      Erro? ";
    try { charClass("[^a--b]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 6.5 << endl;
    cout << "teste Erro False\n";
    cout << "      Erro? ";
    try { charClass("[^--b]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 6.6 << endl;
    cout << "teste Erro False\n";
    cout << "      Erro? ";
    try { charClass("[^a--]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << "  Casos do tipo: \"[a-b-c]\":" << endl<<endl;
    
    cout << 7.1 << endl;
    cout << "teste Erro True\n";
    cout << "      Erro? ";
    try { charClass("[a-b-c]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 7.2 << endl;
    cout << "teste Erro False\n";
    cout << "      Erro? ";
    try { charClass("[-b-c]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 7.3 << endl;
    cout << "teste Erro False\n";
    cout << "      Erro? ";
    try { charClass("[a-b-]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 7.4 << endl;
    cout << "teste Erro False\n";
    cout << "      Erro? ";
    try { charClass("[-b-]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 7.5 << endl;
    cout << "teste Erro True\n";
    cout << "      Erro? ";
    try { charClass("[^a-b-c]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 7.6 << endl;
    cout << "teste Erro False\n";
    cout << "      Erro? ";
    try { charClass("[^-b-c]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 7.7 << endl;
    cout << "teste Erro False\n";
    cout << "      Erro? ";
    try { charClass("[^a-b-]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
    cout << 7.8 << endl;
    cout << "teste Erro False\n";
    cout << "      Erro? ";
    try { charClass("[^-b-]"); cout << "False" <<endl<<endl; }
    catch(...){ cout << "True" <<endl<<endl; }
    
  }
  #endif
    
  /*
   * Teste da função: pMatch::charClass::match
  */

  #if NUMBER==match || NUMBER==2 || NUMBER==ALL
  {  
    using namespace std;
    using namespace pMatch;
    
    bool resp;
    
    try
    {
      cout << "\n\nTestando Formatos com uma única Classe" << endl << endl;
      
      cout << 1.1 << endl;
      cout << "Teste " << "1" << endl;
      resp = charClass("a").match('a');
      cout << "      " << resp << endl << endl;
      
      cout << 1.2 << endl;
      cout << "Teste " << "1" << endl;
      resp = charClass("b").imatch('a');
      cout << "      " << resp << endl << endl;
      
      cout << 2.1 << endl;
      cout << "Teste " << "0" << endl;
      resp = charClass("b").match('a');
      cout << "      " << resp << endl << endl;
      
      cout << 2.2 << endl;
      cout << "Teste " << "0" << endl;
      resp = charClass("a").imatch('a');
      cout << "      " << resp << endl << endl;
      
      cout << 3 << endl;
      cout << "Teste " << "1" << endl;
      resp = charClass("[abc]").match('b');
      cout << "      " << resp << endl << endl;
      
      cout << 4 << endl;
      cout << "Teste " << "0" << endl;
      resp = charClass("[abc]").match('d');
      cout << "      " << resp << endl << endl;
      
      cout << 5 << endl;
      cout << "Teste " << "1" << endl;
      resp = charClass("\\[").match('[');
      cout << "      " << resp << endl << endl;
      
      cout << 6 << endl;
      cout << "Teste " << "1" << endl;
      resp = charClass("[-abc]").match('-');
      cout << "      " << resp << endl << endl;
      
      cout << 7 << endl;
      cout << "Teste " << "0" << endl;
      resp = charClass("[a-bc]").match('-');
      cout << "      " << resp << endl << endl;
      
      cout << 8 << endl;
      cout << "Teste " << "1" << endl;
      resp = charClass("[]abc]").match(']');
      cout << "      " << resp << endl << endl;
      
      cout << 9 << endl;
      cout << "Teste " << "1" << endl;
      resp = charClass("[a-c]").match('b');
      cout << "      " << resp << endl << endl;
      
      cout << 10 << endl;
      cout << "Teste " << "0" << endl;
      resp = charClass("[^a-c]").match('b');
      cout << "      " << resp << endl << endl;
      
      cout << 11 << endl;
      cout << "Teste " << "0" << endl;
      resp = charClass("[^abc]").match('b');
      cout << "      " << resp << endl << endl;
      
      cout << 12 << endl;
      cout << "Teste " << "1" << endl;
      resp = charClass("[^abc]").match('d');
      cout << "      " << resp << endl << endl;
      
      cout << 13 << endl;
      cout << "Teste " << "0" << endl;
      resp = charClass("[^-abc]").match('-');
      cout << "      " << resp << endl << endl;
      
      cout << 14 << endl;
      cout << "Teste " << "0" << endl;
      resp = charClass("[^]abc]").match(']');
      cout << "      " << resp << endl << endl;
      
    }
    catch(...){ cout << "Erro inesperado durante o teste!\n\n"; exit(1); }
  }  
  #endif

 /*
  * @nome: Bateria de testes da charClass::find().
  * 
  * Cada teste imprime na tela 3 linhas:
  *   - O número do teste
  *   - uma linha escrito "Teste " + [valores esperados no teste]
  *   - uma linha contendo os valores de retorno do teste.
  *
  *  O teste é considerado válido se as duas últimas linhas
  *  Tiverem valores compatíveis (iguais)
  *
  */
  #if NUMBER==find || NUMBER==3 || NUMBER==ALL
  {  
    using namespace pMatch;
    
    string s("testando");
    
    size_t pos;
    char c;
    
    cout << " * * * * * charClass::find() * * * * *\n\n";
    
    cout << "Testando os formatos: \"a\" ou \"[abc]\"" << endl;
    
    cout << 1.1 << endl;
    cout << "teste a-4\n";
    c = charClass("a").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;

    cout << 1.2 << endl;
    cout << "teste a-4\n";
    c = charClass("[tes]").ifind(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 2.1 << endl;
    cout << "teste n-5\n";
    c = charClass("n").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 2.2 << endl;
    cout << "teste n-5\n";
    c = charClass("[tesa]").ifind(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 3 << endl;
    cout << "teste a-4\n";
    c = charClass("[a]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 4 << endl;
    cout << "teste n-5\n";
    c = charClass("[n]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 5 << endl;
    cout << "teste a-4\n";
    c = charClass("[an]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 6 << endl;
    cout << "teste a-4\n";
    c = charClass("[na]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 7 << endl;
    cout << "teste  -8\n";
    c = charClass("[x]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 8 << endl;
    cout << "teste e-1\n";
    c = charClass("[a-e]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 9 << endl;
    cout << "teste e-1\n";
    c = charClass("[e-a]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 10 << endl;
    cout << "teste  -8\n";
    c = charClass("[x-z]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 11 << endl;
    cout << "teste  -8\n";
    c = charClass("[]x-z]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << "Testando classe inversa: \"[^abc]\"" << endl;
    
    cout << 12 << endl;
    cout << "teste e-1\n";
    c = charClass("[^t]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 13 << endl;
    cout << "teste s-2\n";
    c = charClass("[^te]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 14 << endl;
    cout << "teste a-4\n";
    c = charClass("[^tse]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 15 << endl;
    cout << "teste a-4\n";
    c = charClass("[^]tse]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 16 << endl;
    cout << "teste a-4\n";
    c = charClass("[^e-t]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 17 << endl;
    cout << "teste a-4\n";
    c = charClass("[^t-e]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 18 << endl;
    cout << "teste e-1\n";
    c = charClass("[^t-z]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 19 << endl;
    cout << "teste s-2\n";
    c = charClass("[^a-et]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 20 << endl;
    cout << "teste s-2\n";
    c = charClass("[^ta-e]").find(s, pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << "Testando o casamento de caracteres especiais: \"[]ab^c-]\"\n";
    
    cout << 21 << endl;
    cout << "teste ]-3\n";
    c = charClass("[]]").find("[[[]", pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 22 << endl;
    cout << "teste [-3\n";
    c = charClass("[^]]").find("]]][", pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 23 << endl;
    cout << "teste ^-3\n";
    c = charClass("[a^]").find("]]]^", pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 24 << endl;
    cout << "teste o-3\n";
    c = charClass("[^^]").find("^^^o", pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 25 << endl;
    cout << "teste --3\n";
    c = charClass("[abc-]").find("^^^-", pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 26 << endl;
    cout << "teste --3\n";
    c = charClass("[-abc]").find("^^^-", pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 27 << endl;
    cout << "teste o-3\n";
    c = charClass("[^-abc]").find("---o", pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 28 << endl;
    cout << "teste o-3\n";
    c = charClass("[^-abc]").find("---o", pos=0);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << "Testando com variação da variável pos\n" << endl;
    
    s = string("testando com afeto");
    
    cout << 29 << endl;
    cout << "teste t-3\n";
    c = charClass("[^es]").find(s, pos=1);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 30 << endl;
    cout << "teste a-4\n";
    c = charClass("[abc]").find(s, pos=4);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 31 << endl;
    cout << "teste c-9\n";
    c = charClass("[c]").find(s, pos=5);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 32 << endl;
    cout << "teste o-17\n";
    c = charClass("[r-o]").find(s, pos=11);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << 33 << endl;
    cout << "teste  -18\n";
    c = charClass("[-]").find(s, pos=11);
    cout << "char: " << c << " pos: " << pos << endl<<endl;
    
    cout << "Testando casos de erro: com pos<0 e pos>input.length\n" << endl;
    
    cout << 34 << endl;
    cout << "teste Erro True" << endl;
    cout << "      Erro? ";
    try
    {c = charClass("[abc]").find(s, pos=-1); cout << "False" << endl << endl;}
    catch(...){ cout << "True" << endl << endl; }
    
    cout << 35 << endl;
    cout << "teste Erro True" << endl;
    cout << "      Erro? ";
    try
    {c = charClass("[abc]").find(s, pos=19); cout << "False" << endl << endl;}
    catch(...){ cout << "True" << endl << endl; }
    
  }
  #endif
    
  /*
   * Teste da função: pMatch::charClass::getClass
   * 
  */

  #if NUMBER==getClass || NUMBER==4 || NUMBER==ALL
  {

    using namespace std;
    using pMatch::charClass;
    
    size_t pos;
    string resp;
    
    try
    {
      cout << "\n\nTeste pMatch::charClass::getClass()" << endl << endl;
      
      cout << "Testando strings com uma única Classe: \"a\"" << endl << endl;
      
      cout << 1 << endl;
      cout << "Teste " << "a - 1" << endl;
      resp = charClass::getClass("a", pos=0);
      cout << "      " << resp << " - " << pos << endl << endl;
      
      cout << 2 << endl;
      cout << "Teste " << "[ - 2" << endl;
      resp = charClass::getClass("\\[", pos=0);
      cout << "      " << resp << " - " << pos << endl << endl;
      
      cout << 3 << endl;
      cout << "Teste " << "a - 3" << endl;
      resp = charClass::getClass("[a]", pos=0);
      cout << "      " << resp << " - " << pos << endl << endl;
      
      cout << 4 << endl;
      cout << "Teste " << "]abc[-- - 9" << endl;
      resp = charClass::getClass("[]abc[--] ", pos=0);
      cout << "      " << resp << " - " << pos << endl << endl;
      
      cout << 5 << endl;
      cout << "Teste " << "]abc[-- - 10" << endl;
      resp = charClass::getClass("[^]abc[--] ", pos=0);
      cout << "      " << resp << " - " << pos << endl << endl;
      
      cout << 6 << endl;
      cout << "Teste " << "]abc[-- - 10" << endl;
      resp = charClass::getClass("[^]abc[--] ", pos=0);
      cout << "      " << resp << " - " << pos << endl << endl;
      
      cout << "  Testando strings com multiplas Classes: \"[a]bc\""<<endl<<endl;
      
      pos = 0;
      string s("[a]bc[--]");
      
      cout << 7.1 << endl;
      cout << "Teste " << "a - 3" << endl;
      resp = charClass::getClass(s, pos);
      cout << "      " << resp << " - " << pos << endl << endl;
      
      cout << 7.2 << endl;
      cout << "Teste " << "b - 4" << endl;
      resp = charClass::getClass(s, pos);
      cout << "      " << resp << " - " << pos << endl << endl;
      
      cout << 7.3 << endl;
      cout << "Teste " << "c - 5" << endl;
      resp = charClass::getClass(s, pos);
      cout << "      " << resp << " - " << pos << endl << endl;
      
      cout << 7.4 << endl;
      cout << "Teste " << "-- - 9" << endl;
      resp = charClass::getClass(s, pos);
      cout << "      " << resp << " - " << pos << endl << endl;
      
      cout << "  Testando casos de erro" << endl << endl;
      
      cout << 8.1 << endl;
      cout << "Teste " << "No error" << endl;
      try{ resp=charClass::getClass("abc", pos=0); cout << "      No error\n\n"; }
      catch(...){ cout << "      Error\n\n"; }
      
      cout << 8.2 << endl;
      cout << "Teste " << "Error" << endl;
      try{ resp=charClass::getClass("abc", pos=3); cout << "      No error\n\n"; }
      catch(...) { cout << "      Error\n\n"; }
      
      cout << 8.3 << endl;
      cout << "Teste " << "Error" << endl;
      try{ resp=charClass::getClass("abc", pos=-1); cout << "      No error\n\n"; }
      catch(...){ cout << "      Error\n\n"; }
      
      cout << 8.4 << endl;
      cout << "Teste " << "Error" << endl;
      try{ resp=charClass::getClass("abc", pos=4); cout << "      No error\n\n"; }
      catch(...){ cout << "      Error\n\n"; }
      
      cout << 8.5 << endl;
      cout << "Teste " << "Error" << endl;
      try{ resp=charClass::getClass("[abc", pos=0); cout << "      No error\n\n"; }
      catch(...){ cout << "      Error\n\n"; }

      cout << "  Testando função str()" << endl << endl;

      cout << 9 << endl;
      cout << "Teste " << "a" << endl;
      try{ cout << "      " << charClass("a").str() << endl << endl; }
      catch(...){ cout << "      Error\n\n"; }

      cout << 10 << endl;
      cout << "Teste " << "a" << endl;
      try{ cout << "      " << charClass("[a]").str() << endl << endl; }
      catch(...){ cout << "      Error\n\n"; }

      cout << 11 << endl;
      cout << "Teste " << "[^a]" << endl;
      try{ cout << "      " << charClass("[^a]").str() << endl << endl; }
      catch(...){ cout << "      Error\n\n"; }

      cout << 12 << endl;
      cout << "Teste " << "[abc]" << endl;
      try{ cout << "      " << charClass("[abc]").str() << endl << endl; }
      catch(...){ cout << "      Error\n\n"; }

      cout << 13 << endl;
      cout << "Teste " << "[^abc]" << endl;
      try{ cout << "      " << charClass("[^abc]").str() << endl << endl; }
      catch(...){ cout << "      Error\n\n"; }

      cout << 14 << endl;
      cout << "Teste " << "[^][^\\]" << endl;
      try{ cout << "      " << charClass("[^][^\\]").str() << endl << endl; }
      catch(const char* c) { cout << "      Error" << c << "\n\n"; }
    } catch(int e) {
      cout << "  Unexpected error during test \n  Error number: " << e << endl;
    }

  }
  #endif
    
  /* * * * * END TEST charClass * * * * */
}

