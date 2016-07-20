#include <cstdlib>
#include <cstring> // For strchr
#include <sstream>

#include "./code-parser.h"
#include "./generators.h"
#include "./exp-parser/shunting-yard-exceptions.h"

/* * * * * IfStatement Class * * * * */

void IfStatement::_compile(const char* code, const char** rest,
                           const Scope& parent_scope) {

  while (isspace(*code)) ++code;

  if (*code != '(') {
    throw syntax_error("Expected '(' after `if` statement!");
  }

  // Parse the condition:
  cond.compile(code+1, parent_scope, ")", &code);

  if (*code != ')') {
    throw syntax_error("Missing ')' after `if` statement!");
  }

  _then.compile(code+1, &code, parent_scope);

  while (isspace(*code)) ++code;

  // Check if the next word is else:
  static const char* str = "else";
  for (int i = 0; i < 4; ++i) {
    if (str[i] != code[i]) {
      if(rest) *rest = code;
      return;
    }
  }
  if (isalnum(code[4]) || code[4] == '_') {
    if(rest) *rest = code;
    return;
  }

  _else.compile(code+4, &code, parent_scope);

  if(rest) *rest = code;
}

void IfStatement::_exec(const Scope& scope) const {
  if (cond.eval(scope).asBool()) {
    _then.exec(scope);
  } else {
    _else.exec(scope);
  }
}

/* * * * * ExpStatement Class * * * * */

void ForStatement::_compile(const char* code, const char** rest,
                            const Scope& parent_scope) {
  std::stringstream ss;

  while (isspace(*code)) ++code;

  if (*code != '(') {
    throw syntax_error("Expected '(' after `for` statement!");
  }

  // Parse the condition:
  ++code;
  while (isspace(*code)) ++code;

  if (isalpha(*code) || *code == '_') {
    ss << *code;
    ++code;
    while (isalnum(*code) || *code == '_') {
      ss << *code;
      ++code;
    }

    name = ss.str();

    if (!isspace(*code)) {
      throw syntax_error("Required a blank space after `for(` variable name");
    }

    ++code;
    while (isspace(*code)) ++code;

    if (*code == 'i' && code[1] == 'n' && !(isalnum(code[2]) || code[2] == '_')) {
      it_expr.compile(code+2, parent_scope, ")", &code);
    } else {
      throw syntax_error("Missing `in` in `for(<var> in <exp>)` statement");
    }

    if (*code != ')') {
      throw syntax_error("Missing ')' after `for` statement!");
    }

    body.compile(code+1, &code, parent_scope);

    if(rest) *rest = code;
  } else {
    throw syntax_error("Expected variable name declaration after `for(`");
  }
}

void ForStatement::_exec(const Scope& scope) const {
  // Get the local token map:
  TokenMap_t* local = scope.scope.front();
  Iterator* it;

  packToken p_it = it_expr.eval(scope);

  if (p_it->type == IT) {
    it = static_cast<Iterator*>(p_it->clone());
  } else {
    throw syntax_error("The evaluated object should be an iterator!");
  }

  for (packToken* value = it->next(); value; value = it->next()) {
    (*local)[name] = *value;
    body.exec(scope);
    delete value;
  }

  delete it;
}

/* * * * * ExpStatement Class * * * * */

void ExpStatement::_compile(const char* code, const char** rest,
                            const Scope& parent_scope) {
  expr.compile(code, parent_scope, ";}\n", &code);

  // Skip the delimiter character:
  if (*code) ++code;

  if (rest) *rest = code;
}

void ExpStatement::_exec(const Scope& scope) const {
  expr.eval(scope);
}

/* * * * * BlockStatement Class * * * * */

// Decide what type of statement to build:
Statement* buildStatement(const char** source, const Scope& scope) {
  const char* code = *source;
  switch (*code) {
  case '{':
    return new BlockStatement(code, source, scope);
  case 'i':
    if (code[1] == 'f' && !(isalnum(code[2]) || code[2] == '_'))
      return new IfStatement(code+2, source, scope);
  case 'f':
    if (code[1] == 'o' && code[2] == 'r' && !(isalnum(code[3]) || code[3] == '_'))
      return new ForStatement(code+3, source, scope);
  default:
    return new ExpStatement(code, source, scope);
  }
}

void BlockStatement::cleanList(codeBlock_t* list) {
  for(auto stmt : *list) {
    delete stmt;
  }

  list->clear();
}

BlockStatement::~BlockStatement() {
  cleanList(&list);
}

void BlockStatement::_compile(const char* code, const char** rest,
                              const Scope& parent_scope) {
  // Make sure the list is empty:
  cleanList(&list);

  while (isspace(*code)) ++code;

  if (*code == '{') {

    // Find the next non-blank character:
    ++code;
    while (isspace(*code)) ++code;

    // Parse each statement of the block:
    while (*code != '\0' && *code != '}') {
      // Ignore empty statements:
      if (strchr(";\n", *code)) {
        ++code;
      } else {
        list.push_back(buildStatement(&code, parent_scope));
      }

      // Discard blank spaces:
      while (isspace(*code)) ++code;
    }

    if (*code == '}') {
      ++code;
    } else {
      throw syntax_error("Missing a '}' somewhere on the code!");
    }
  } else {
    list.push_back(buildStatement(&code, parent_scope));
  }

  if (rest) *rest = code;
}

void BlockStatement::_exec(const Scope& scope) const {
  for(const auto stmt : list) {
    stmt->exec(scope);
  }
}
