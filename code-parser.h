#include <list>
#include <cstdlib>
#include <cstring> // For strchr

#include "shunting-yard.h"
#include "shunting-yard-exceptions.h"

class Statement {
  calculator* expr;

 public:
  Statement(const char* code, const Scope& scope, const char* delim, const char** rest=0);
  Statement(const Statement& other);
  ~Statement();

  Statement& operator=(const Statement& other);

  packToken exec(const Scope& scope) const;
};

struct CodeParser {
  Scope scope;
  TokenMap_t local;

  typedef std::list<Statement> codeBlock_t;
  codeBlock_t list;

  CodeParser(const char* code, const char** rest) : CodeParser(code, Scope::empty, rest) {}
  CodeParser(const char* code, const Scope& parent_scope = Scope::empty, const char** rest=0);

  void exec() { exec(this->scope); }
  void exec(const Scope& scope);
};







