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
  scope = parent_scope;
  scope.push(&local);

  while (true) {

    // Ignore blank spaces:
    while (*code && isblank(*code)) ++code;

    if (*code == '\0' || *code == '}') {
      break;
    } else if (strchr(";\n", *code)) {
      ++code;
    } else {
      list.push_back(Statement(code, scope, ";}\n", &code)); 

      if (*code == '\0' || *code == '}') break;
      ++code;
    }
  }

  if (*code == '\0') {
    throw syntax_error("Expected '}'");
  }

  if (rest) *rest = code;
}

void CodeParser::exec(const Scope& scope) {
  for(const auto& stmt : list) {
    stmt.exec(scope);
  }
}
