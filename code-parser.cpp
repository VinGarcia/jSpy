#include <cstdlib>
#include <cstring> // For strchr

#include "code-parser.h"
#include "shunting-yard-exceptions.h"

/* * * * * Statement Class * * * * */

Statement::Statement(const char* code, const Scope& scope,
          const char* delim, const char** rest) {
  expr = new calculator(code, scope, delim, rest);
}

Statement::Statement(const Statement& other) {
  expr = new calculator(*(other.expr));
}

Statement::~Statement() { delete expr; }

Statement& Statement::operator=(const Statement& other) {
  delete expr; expr = new calculator(*(other.expr));
  return *this;
}

packToken Statement::exec(const Scope& scope) const {
  return expr->eval(scope);
}

/* * * * * CodeParser Class * * * * */

CodeParser::CodeParser(const char* code, const char** rest) {
  compile(code, rest);
}

CodeParser::CodeParser(const char* code, const Scope& parent_scope, const char** rest) {
  compile(code, parent_scope, rest);
}

void CodeParser::compile(const char* code, const char** rest) {
  compile(code, Scope::empty, rest);
}

void CodeParser::compile(const char* code, const Scope& parent_scope, const char** rest) {
  while (true) {

    // Ignore blank spaces:
    while (*code && isblank(*code)) ++code;

    if (*code == '\0' || *code == '}') {
      break;
    } else if (strchr(";\n", *code)) {
      ++code;
    } else {
      list.push_back(Statement(code, parent_scope, ";}\n", &code)); 

      if (*code == '\0' || *code == '}') break;
      ++code;
    }
  }

  if (rest) *rest = code;
}

TokenMap_t CodeParser::exec(const Scope& scope) {
  TokenMap_t local;
  scope.push(&local);
  for(const auto& stmt : list) {
    stmt.exec(scope);
  }
  scope.pop();
  // This copy of the local scope is mainly for debugging.
  return local;
}
