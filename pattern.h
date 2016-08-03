#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <map>

#ifndef CASAMENTO_PADRAO
#define CASAMENTO_PADRAO

typedef unsigned uint;

namespace pMatch
{
  // Testado
  class charClass : public std::string
  {
    private:
    bool invert=false;

    bool _match(char input, bool invert);
    char _find(std::string input, uint& pos, bool invert);

    public:
    charClass(std::string format);
    charClass(const char* format);
    
    bool match(char input);
    bool match(std::string, uint pos);
    bool imatch(char input);
    bool imatch(std::string, uint pos);

    char find(std::string input, uint& pos);
    char ifind(std::string input, uint& pos);

    static charClass getClass(std::string format, uint& pos);
    
    std::string str();
  };
  
  // Not going to be tested explicitly
  // Considerado testado por uso extremo em outros modulos
  class tWord : public std::string
  {
    uint _start;
    uint _end;
    
    public:
    uint start(){return _start;}
    uint end(){return _end;}
    
    tWord() : std::string() {}
    
    tWord(const char* word, uint start=0) : tWord(std::string(word), start) {}
    
    tWord(std::string word, uint start=0) : std::string(word) {
      if (start<0) {
        throw "Parâmetro pos inválido em pMatch::tWord::tWord(string,int)";
      }
      this->_start = start;
      this->_end   = start+this->length();
    }
  };
  
  class lWord : public std::list<tWord> {
   public:
    lWord() : std::list<tWord>() {}
    std::string str() {
      std::string resp = std::string("");
      if (this->empty()) return resp; 
      
      std::list<tWord>::iterator it = this->begin();
      resp += (*it); it++;
      for ( ; it!=this->end(); it++)
        resp += "|" + (*it);
      return resp;
    }
  };
  
  class cVar;
  
  // Variables list:
  typedef std::list<cVar> lVar;
  
  // Considerado testado por uso extenso em outros modulos.
  struct tInterpretacao
  {
    // Cada interpretação contém variáveis locais com seus valores:
    lVar var;
    // Cada interpretação contém um intervalo de texto que foi interpretado.
    tWord word;
  };
  
  // Classe que representa uma variável.
  // Considerado testado por uso extenso em outros modulos.
  class cVar {
    public:
    // Nome da variável:
    std::string nome;
    
    // Lista de interpretações do texto lido por essa variável:
    std::list<tInterpretacao> lInt;
    
    public:
    cVar() {}
    // Considerado testado por uso extenso em outros modulos.
    cVar(std::string nome) { this->nome=nome; }
    // Considerado testado por uso extenso em outros modulos.
    cVar(std::string nome, std::list<tInterpretacao>::iterator it) {
      this->nome=nome;
      lInt.push_back(*it);
    }
    
    std::string str() {
      using namespace std;
      string words, resp;
      list<tInterpretacao>::iterator itI=lInt.begin();
      list<cVar>::iterator itV;
      if(itI != lInt.end()) {
        words += lInt.front().word;
        if(!itI->var.empty()) {
          words += "{";
          for(itV=itI->var.begin(); itV != itI->var.end(); itV++)
            words += itV->str() + "; ";
          words.pop_back();
          words += "}";
        }
        itI++;
      }
      
      for(; itI != lInt.end(); itI++) {
        words += "|" + itI->word;
        if(!itI->var.empty()) {
          words += "{";
          for(itV=itI->var.begin(); itV != itI->var.end(); itV++)
            words += itV->str() + "; ";
          words.pop_back();
          words += "}";
        }
      }
      
      if(this->nome == "") resp = "no_name: "; else resp = this->nome + ": ";
      resp += words;
      
      return resp;
    }
  };
  
  class matcher {
   public:
    // Scope variable:
    cVar var;
    lWord match_word;
    
   public:
    // Return the last match found by the match() method.
    lWord getMatch() { return this->match_word; }
    virtual std::string str()=0;
    
    virtual bool match(std::string input, uint pos)=0;
    virtual ~matcher() {}
  };
  
  // Testado
  class strClass : public std::list<charClass>, public matcher {
    public:
    strClass() {}
    strClass(const char* str);
    strClass(std::string str);
    
    bool match(std::string input, uint pos);
    tWord find(std::string input, uint pos);
    
    // A função getClass lê uma string até encontrar um '\0'
    // ou um '(' que não seja precedido por um '\\'.
    // Seu comportamento trata o '(' de forma especial.
    // As demais funções dessa classe são indiferentes ao '('.
    static strClass getClass(std::string format, uint& pos, std::string stop_on = "(");
    
    std::string str();
  };
  
  // Uma classe array contém uma sequencia de strClasse`s e blockClasse`s
  // match é obtido se o texto der match em todos da sequencia.
  // onde cada parte do texto deve dar match com um da sequencia.
  // Testado
  class arrayClass : public matcher {
    std::list<matcher*> lista;
    
    void build(std::string str);
    
    std::list<tInterpretacao>
    sub_match(std::string, uint pos, std::list<matcher*>::iterator it);
    
    public:
    arrayClass() {}
    arrayClass(const char* str);
    arrayClass(std::string str);
    arrayClass(const arrayClass& array) { (*this) = array; }
    
    // Decodifica uma string tranformando-a em um arrayClass.
    // A string recebida deve estar entre aspas.
    // pos deve apontar para a abertura da aspas antes da execução.
    // e apontará para o fechamento das aspas após a execução.
    arrayClass(std::string str, uint* pos);
    
    bool match(std::string input, uint pos);
    bool match(std::string input, uint pos, bool raiz);
    tWord find(std::string input, uint pos);
    
    std::string str();
  };
  
  // A block class is a disjunction of arrayClasses and/or objectClasses.
  // It means it will match if any of its submatchers matches.
  // It also means it can have multiple results causing ambiguity.
  class blockClass : public matcher {
    bool repeater=false;
    // The vector containing all the possible patterns
    // of the blockClass instance.
    std::vector<matcher*> lista;
    
    // Fill the `lista` array
    void leLista(std::string str);
    
    // Check the argument string for problems:
    void validate(std::string str, uint fim);
    
    void build(std::string str, uint fim);
    
    std::string block_name;
     
    // Recursive auxiliar function of blockClass::match()
    std::list<tInterpretacao> sub_match(std::string str, uint pos, uint rep_pos);

    public:
    std::vector<matcher*> getLista() { return this->lista; }
    std::string nome() { return block_name; }
    
    public:
    blockClass() {}
    blockClass(const char* str);
    blockClass(std::string str);
    blockClass(const blockClass& block) { (*this) = block; }
    
    // This functions is a parser for the blockClass type.
    // It expects the first character to be a '('.
    blockClass(std::string input, uint& pos);
    
    bool match(std::string input, uint pos);
    tWord find(std::string input, uint pos);
    
    std::string str();
  };
  
  // Not tested yet!
  class objectClass : public matcher {
   public:
    typedef std::map<std::string, matcher*> LabelMap_t;
    static LabelMap_t labels;

   private:
    std::string name;

   private:
    void build(std::string str, uint* pos = NULL);
    
   public:
    objectClass() {}
    objectClass(const char* name);
    objectClass(std::string name);
    
    objectClass(std::string str, uint& pos);
    
    bool match(std::string, uint pos);
    // TODO(VinGarcia): Implement the `find` code:
    tWord find(std::string, uint pos) { return tWord("",0); }
   
    std::string str() { return name; }
  };
}

#endif
