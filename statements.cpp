#include <cstdlib>
#include <cstring> // For strchr
#include <sstream>
#include <iostream>

#include "./statements.h"
#include "./range.h"
#include "./cparse/shunting-yard-exceptions.h"
#include "./matcher.h"

/* * * * * Utility functions: * * * * */

std::string parseName(const char** source, char end_char = '\0') {
  std::stringstream ss;
  const char* code = *source;

  // Parse the function name:
  if (isalpha(*code) || *code == '_') {
    ss << *code;
    ++code;

    while (isalnum(*code) || *code == '_') {
      ss << *code;
      ++code;
    }

    // Find the beggining of the non space character:
    while (isspace(*code) && *code != end_char) ++code;
  } else {
    throw syntax_error("Expected variable name!");
  }

  *source = code;

  return ss.str();
}

/* * * * * VarStatement Class * * * * */

void VarStatement::_compile(const char* code, const char** rest,
                            TokenMap parent_scope) {
  // Ignore white spaces:
  while (isspace(*code)) ++code;

  // For each declaration:
  while (1) {
    decl_t decl;
    // Extract the variable name:
    decl.name = parseName(&code, '\n');

    // Find the next character or delimiter in the line:
    while (isblank(*code)) ++code;

    if (*code && !strchr("=,};\n", *code)) {
      throw syntax_error("Unexpected character after variable declaration!");
    }

    // Check if there is an assignment or a colon:
    if (*code == '=') {
      ++code;

      // Ignore spaces before the expression,
      // so the user may add '\n's here.
      while (isspace(*code)) ++code;

      // Parse the expression:
      decl.expr.compile(code, parent_scope, ",};\n", &code);
    } else {
      decl.expr.compile("None");
    }

    declarations.push_back(decl);

    // Find the next one or the end:
    if (*code == ',') {
      ++code;
      while (isspace(*code)) ++code;
    } else {
      break;
    }
  }

  if (rest) *rest = code;
}

returnState VarStatement::_exec(TokenMap scope) const {
  for (const decl_t& decl : declarations) {
    scope[decl.name] = decl.expr.eval(scope);
  }
  return NORMAL;
}

/* * * * * ScopedStatement Class * * * * */

void ScopedStatement::_compile(const char* code, const char** rest,
                               TokenMap parent_scope) {
  this->code.compile(code, &code, parent_scope);
}

returnState ScopedStatement::_exec(TokenMap scope) const {
  code.exec(scope.getChild());
  return NORMAL;
}

/* * * * * IfStatement Class * * * * */

void IfStatement::_compile(const char* code, const char** rest,
                           TokenMap parent_scope) {

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

returnState IfStatement::_exec(TokenMap scope) const {
  if (cond.eval(scope).asBool()) {
    return _then.exec(scope);
  } else {
    return _else.exec(scope);
  }
}

/* * * * * ForStatement Class * * * * */

void ForStatement::_compile(const char* code, const char** rest,
                            TokenMap parent_scope) {
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

returnState ForStatement::_exec(TokenMap scope) const {
  Iterator* it;
  packToken p_it = it_expr.eval(scope);

  if (p_it->type == IT) {
    it = static_cast<Iterator*>(p_it->clone());
  } else if (p_it->type & IT) {
    it = static_cast<Iterable*>(p_it.token())->getIterator();
  } else {
    throw syntax_error("The evaluated object should be iterable!");
  }

  try {
    returnState rs;
    for (packToken* value = it->next(); value; value = it->next()) {
      scope[name] = *value;
      rs = body.exec(scope);

      if (rs.type == BREAK) break;
      if (rs.type != NORMAL && rs.type != CONTINUE) return rs;
    }
  } catch (...) {
    delete it;
    throw;
  }

  delete it;
  return NORMAL;
}

/* * * * * WhileStatement class * * * * */

void WhileStatement::_compile(const char* code, const char** rest,
                              TokenMap parent_scope) {
  std::stringstream ss;

  while (isspace(*code)) ++code;

  if (*code != '(') {
    throw syntax_error("Expected '(' after `while` statement!");
  }

  // Parse the condition:
  ++code;
  cond.compile(code, parent_scope, ")", &code);

  if (*code != ')') {
    throw syntax_error("Missing ')' after `while(` statement!");
  }

  ++code;
  body.compile(code, rest, parent_scope);
}

returnState WhileStatement::_exec(TokenMap scope) const {
  returnState rs;
  while (cond.eval(scope).asBool() == true) {
    rs = body.exec(scope);
    if (rs.type == BREAK) break;
    if (rs.type != NORMAL && rs.type != CONTINUE) return rs;
  }

  return NORMAL;
}

/* * * * * ExpStatement Class * * * * */

void ExpStatement::_compile(const char* code, const char** rest,
                            TokenMap parent_scope) {
  expr.compile(code, parent_scope, ";}\n", &code);

  // Skip the delimiter character:
  if (*code && *code != '}') ++code;

  if (rest) *rest = code;
}

returnState ExpStatement::_exec(TokenMap scope) const {
  return expr.eval(scope);
}

/* * * * * ReturnStatement Class * * * * */

void ReturnStatement::_compile(const char* code, const char** rest,
                               TokenMap parent_scope) {
  while (isspace(*code)) ++code;

  if (strchr(";}\n", *code)) {
    expr.compile("None");
  } else {
    expr.compile(code, parent_scope, ";}\n", &code);
    value_omitted = false;
  }

  if (*code && *code != '}') ++code;

  if (rest) *rest = code;
}

returnState ReturnStatement::_exec(TokenMap scope) const {
  return returnState(RETURN, expr.eval(scope), value_omitted);
}

/* * * * * FuncDeclaration Statement * * * * */

void FuncDeclaration::_compile(const char* code, const char** rest,
                               TokenMap parent_scope) {
  // Find the start of the name:
  while (isspace(*code)) ++code;

  // Parse the function name:
  try {
    name = parseName(&code);
  } catch(syntax_error e) {
    throw syntax_error("Missing name after `function` key-word!");
  }

  _compile(name, code, rest, parent_scope);

  // Ignore white spaces:
  while (isspace(*code)) ++code;
}

void FuncDeclaration::_compile(std::string name, const char* code,
                              const char** rest, TokenMap parent_scope) {
  // Make sure its empty:
  args.clear();

  if (*code != '(') {
    throw syntax_error("Expected argument list after `function` reserved word!");
  }

  // Find the next non-blank character:
  ++code;
  while (isspace(*code)) ++code;

  // Parse each argument of the block:
  while (*code && *code != ')') {

    if (*code == ',') {
      throw syntax_error("Empty item on argument list!");
    } else {
      
      try {
        // Parse the argument name:
        args.push_back(parseName(&code));
      } catch (syntax_error e) {
        throw syntax_error("Invalid argument name!");
      }

      if(*code == ',') {
        ++code;
      } else if (*code != ')') {
        if (isalpha(*code) || *code == '_') {
          syntax_error("Missing ',' in argument list!");
        } else {
          syntax_error("Invalid character in argument list!");
        }
      }
    }

    // Discard blank spaces:
    while (isspace(*code)) ++code;
  }

  if (*code != ')') {
    throw syntax_error("Expected function body before end of file!");
  } else {
    ++code;
  }

  body.compile(code, &code, parent_scope);

  if (rest) *rest = code;
}

returnState FuncDeclaration::_exec(TokenMap scope) const {
  scope[name] = this->asFunc();

  return NORMAL;
}

packToken FuncDeclaration::asFunc() const {
  return packToken(new UserFunction(args, body, name));
}

/* * * * * BlockStatement Class * * * * */

// Decide what type of statement to build:
Statement* buildStatement(const char** source, TokenMap scope) {
  const char* code = *source;
  const char* _template;
  uint i;

  switch (*code) {
  case '{':
    return new BlockStatement(code, source, scope);
  case 'b':
    _template = "break";
    for (i = 1; i < 5; ++i)
      if (code[i] != _template[i]) break;

    if (i == 5 && !(isalnum(code[i]) || code[i] == '_'))
      return new BreakStatement(code+5, source, scope);

    break;
  case 'c':
    _template = "continue";
    for (i = 1; i < 8; ++i)
      if (code[i] != _template[i]) break;

    if (i == 8 && !(isalnum(code[i]) || code[i] == '_'))
      return new ContinueStatement(code+8, source, scope);

    break;
  case 'i':
    if (code[1] == 'f' && !(isalnum(code[2]) || code[2] == '_'))
      return new IfStatement(code+2, source, scope);
    break;
  case 'm':
    _template = "matcher";
    for (i = 1; i < 7; ++i)
      if (code[i] != _template[i]) break;

    if (i == 7 && !(isalnum(code[i]) || code[i] == '_'))
      return new MatcherDeclaration(code+7, source, scope);
  case 'r':
    _template = "return";
    for (i = 1; i < 6; ++i)
      if (code[i] != _template[i]) break;

    if (i == 6 && !(isalnum(code[i]) || code[i] == '_'))
      return new ReturnStatement(code+6, source, scope);

    break;
  case 's':
    _template = "scoped";
    for (i = 1; i < 6; ++i)
      if (code[i] != _template[i]) break;

    if (i == 6 && !(isalnum(code[i]) || code[i] == '_'))
      return new ScopedStatement(code+6, source, scope);
    
    break;
  case 'v':
    if (code[1] == 'a' && code[2] == 'r' && !(isalnum(code[3]) || code[3] == '_'))
      return new VarStatement(code+3, source, scope);
    break;
  case 'w':
    _template = "while";
    for (i = 1; i < 5; ++i)
      if (code[i] != _template[i]) break;

    if (i == 5 && !(isalnum(code[i]) || code[i] == '_'))
      return new WhileStatement(code+5, source, scope);

    break;
  case 'y':
    _template = "yield";
    for (i = 1; i < 5; ++i)
      if (code[i] != _template[i]) break;

    if (i == 5 && !(isalnum(code[i]) || code[i] == '_'))
      return new YieldStatement(code+5, source, scope);

    break;
  case 'f':
    if (code[1] == 'o' && code[2] == 'r' && !(isalnum(code[3]) || code[3] == '_'))
      return new ForStatement(code+3, source, scope);

    _template = "function";
    for (i = 1; i < 8; ++i)
      if (code[i] != _template[i]) break;

    // This will work for 'func' or 'function'
    if ((i == 8 || i == 4) && !(isalnum(code[i]) || code[i] == '_'))
      return new FuncDeclaration(code+i, source, scope);

    _template = "finish";
    for (i = 1; i < 6; ++i)
      if (code[i] != _template[i]) break;

    if (i == 6 && !(isalnum(code[i]) || code[i] == '_'))
      return new FinishStatement(code+6, source, scope);
  }

  return new ExpStatement(code, source, scope);
}

void BlockStatement::cleanList(codeBlock_t* list) {
  for(auto stmt : *list) {
    delete stmt;
  }

  list->clear();
}

BlockStatement::BlockStatement(const BlockStatement& other) {
  for(const Statement* stmt : other.list) {
    list.push_back(stmt->clone());
  }
}

BlockStatement& BlockStatement::operator=(const BlockStatement& other) {
  cleanList(&list);
  for(const Statement* stmt : other.list) {
    list.push_back(stmt->clone());
  }
  return *this;
}

BlockStatement::~BlockStatement() {
  cleanList(&list);
}

void BlockStatement::_compile(const char* code, const char** rest,
                              TokenMap parent_scope) {
  // Make sure the list is empty:
  cleanList(&list);

  while (isspace(*code)) ++code;

  if (*code == '{') {

    // Find the next non-blank character:
    ++code;
    while (isspace(*code)) ++code;

    // Parse each statement of the block:
    while (*code && *code != '}') {
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

returnState BlockStatement::_exec(TokenMap scope) const {
  returnState rs;
  for(const auto stmt : list) {
    rs = stmt->exec(scope);
    if (rs.type != NORMAL) return rs;
  }

  return rs;
}
