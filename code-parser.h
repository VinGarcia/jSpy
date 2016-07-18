#include <list>

#ifndef CODE_PARSER_H_
#define CODE_PARSER_H_

typedef unsigned uint;

#include "shunting-yard.h"

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
  typedef std::list<Statement> codeBlock_t;
  codeBlock_t list;

  CodeParser() {}
  CodeParser(const char* code, const char** rest);
  CodeParser(const char* code, const Scope& parent_scope = Scope::empty, const char** rest = 0);

  void compile(const char* code, const char** rest);
  void compile(const char* code, const Scope& parent_scope = Scope::empty, const char** rest = 0);

  uint size() { return list.size(); }

  TokenMap_t exec(const Scope& scope);
};

#endif  // CODE_PARSER_H_







