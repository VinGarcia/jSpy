
#include <list>

#include "./statements.h"
#include "./pattern.h"

// This struct is used as sub-parser
// by the MatcherDeclaration
struct Hook {
  pMatch::arrayClass expr;
  calculator cond;
  BlockStatement body;

  void compile(const char* code, const char** rest, TokenMap parent_scope);
  Iterator* getIterator(std::string text, TokenMap = &TokenMap::empty) const;

 public:
  Hook() {}
  Hook(const char* code, const char** rest = 0,
       TokenMap parent_scope = &TokenMap::empty) {
    compile(code, rest, parent_scope);
  }
};

// Its equivalent to the class `banco`
struct MatcherDeclaration : public Statement {
  std::string name;
  TokenList hooks;

 private:
  void _compile(const char* code, const char** rest, TokenMap parent_scope);
  returnState _exec(TokenMap scope) const;

 public:
  MatcherDeclaration() {}
  MatcherDeclaration(const char* code, const char** rest = 0,
               TokenMap parent_scope = &TokenMap::empty) {
    _compile(code, rest, parent_scope);
  }
  virtual Statement* clone() const {
    return new MatcherDeclaration(*this);
  }
};

struct Matcher : public pMatch::matcher {
  TokenList hooks;
  Matcher() {}
  Matcher(TokenList hooks) : hooks(hooks) {}
  bool match(std::string input, uint pos);
  std::string str() { return ""; }
};

