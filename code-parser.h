#include <list>

#ifndef CODE_PARSER_H_
#define CODE_PARSER_H_

typedef unsigned uint;

#include "shunting-yard.h"

class Statement {
 protected:
  virtual void _compile(const char* code, const char** rest,
                        const Scope& parent_scope) = 0;
  virtual void _exec(const Scope& scope) const = 0;

 public:
  virtual ~Statement() {}
  void compile(const char* code, const char** rest = 0,
               const Scope& parent_scope = Scope::empty) {
    return _compile(code, rest, parent_scope);
  }

  void exec(const Scope& scope) const { _exec(scope); }

  virtual Statement* clone() const = 0;
};

class BlockStatement : public Statement {
  typedef std::list<Statement*> codeBlock_t;
  codeBlock_t list;

 private:
  void cleanList(codeBlock_t* list);

 private:
  void _compile(const char* code, const char** rest, const Scope& parent_scope);
  void _exec(const Scope& scope) const;

 public:
  BlockStatement() {}
  BlockStatement(const char* code, const char** rest = 0,
            const Scope& parent_scope = Scope::empty) {
    _compile(code, rest, parent_scope);
  }
  BlockStatement(const BlockStatement& other);
  ~BlockStatement();
  BlockStatement& operator=(const BlockStatement& other);
  uint size() { return list.size(); }

  virtual Statement* clone() const {
    return new BlockStatement(*this);
  }
};

class IfStatement : public Statement {
  calculator cond;
  BlockStatement _then;
  BlockStatement _else;

 private:
  void _compile(const char* code, const char** rest, const Scope& parent_scope);
  void _exec(const Scope& scope) const;

 public:
  IfStatement() {}
  IfStatement(const char* code, const char** rest = 0,
              const Scope& parent_scope = Scope::empty) {
    _compile(code, rest, parent_scope);
  }
  virtual Statement* clone() const {
    return new IfStatement(*this);
  }
};

class ForStatement : public Statement {
  std::string name;
  calculator it_expr;
  BlockStatement body;

 private:
  void _compile(const char* code, const char** rest, const Scope& parent_scope);
  void _exec(const Scope& scope) const;

 public:
  ForStatement() {}
  ForStatement(const char* code, const char** rest = 0,
               const Scope& parent_scope = Scope::empty) {
    _compile(code, rest, parent_scope);
  }
  virtual Statement* clone() const {
    return new ForStatement(*this);
  }
};

// class WhileStatement : public Statement {
//   calculator cond;
//   BlockStatement body;

//  private:
//   void _compile(const char* code, const char** rest, const Scope& parent_scope);
//   void _exec(const Scope& scope) const;

//  public:
//   WhileStatement() {}
//   WhileStatement(const char* code, const char** rest = 0,
//                const Scope& parent_scope = Scope::empty) {
//     _compile(code, rest, parent_scope);
//   }
// };

class ExpStatement : public Statement {
  calculator expr;

 private:
  void _compile(const char* code, const char** rest, const Scope& parent_scope);
  void _exec(const Scope& scope) const;

 public:
  ExpStatement() {}
  ExpStatement(const char* code, const char** rest = 0,
               const Scope& parent_scope = Scope::empty) {
    _compile(code, rest, parent_scope);
  }
  virtual Statement* clone() const {
    return new ExpStatement(*this);
  }
};

class FuncDeclaration : public Statement {
  typedef std::list<std::string> strList;
  strList args;
  BlockStatement body;
  std::string name;

 private:
  void _compile(const char* code, const char** rest, const Scope& parent_scope);
  void _exec(const Scope& scope) const;

 public:
  FuncDeclaration() {}
  FuncDeclaration(const char* code, const char** rest = 0,
               const Scope& parent_scope = Scope::empty) {
    _compile(code, rest, parent_scope);
  }
  virtual Statement* clone() const {
    return new FuncDeclaration(*this);
  }
};

#endif  // CODE_PARSER_H_
