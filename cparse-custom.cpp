#include <sstream>

#include "./cparse/shunting-yard.h"
#include "./cparse/shunting-yard-exceptions.h"

#include "./statements.h"
#include "./range.h"

namespace custom_features {

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

packToken new_operator(const packToken& left, const packToken& right, evaluationData* data) {
  TokenMap& parent = left.asMap();
  Tuple args;
  
  if (right->type == TUPLE) {
    args = right.asTuple();
  } else {
    args = Tuple(right);
  }

  // Create a new Map:
  packToken child = TokenMap(&parent);

  // Call its init function if it exists:
  packToken* init = parent.find("__init__");
  if (init) {
    Function* func = init->asFunc();
    Function::call(child, func, &args, data->scope);
  }

  // Return it:
  return child;
}

void new_parser(const char* expr, const char** rest, rpnBuilder* data) {
  // Parse the class reference as an expression:
  TokenQueue_t name_rpn = calculator::toRPN(expr, data->scope, "(", &expr);

  // Move `class_rpn` to the top of `data->rpn`:
  while (!name_rpn.empty()) {
    TokenBase* token = name_rpn.front();
    name_rpn.pop();
    data->rpn.push(token);
  }

  data->lastTokenWasOp = false;

  // Add the new operator to the queue:
  data->handle_op("new_op");

  // Find the open bracket:
  while (*expr && *expr != '(') ++expr;

  if (*expr == '(') {
    data->open_bracket("(");
    ++expr;
  } else {
    std::string name = calculator::str(name_rpn);
    throw syntax_error("Expected '(' after 'new " + name + "'");
  }

  // Save the '(' position:
  *rest = expr;
}

class CompiledFunc : public Function {
  FuncDeclaration func;

 public:
  CompiledFunc() : Function() {}
  CompiledFunc(const char* code, const char** rest = 0,
               TokenMap parent_scope = &TokenMap::empty)
              : Function(), func("", code, rest, parent_scope) {}
  virtual ~CompiledFunc() {}

 public:
  const std::string name() const { return ""; }
  const args_t args() const { return {}; }
  packToken exec(TokenMap scope) const {
    return func.asFunc();
  }

 public:
  TokenBase* clone() const {
    return new CompiledFunc(*this);
  }
};

void function_parser(const char* expr, const char** rest, rpnBuilder* data) {
  // Ignore white spaces:
  while (isspace(*expr)) ++expr;

  if (isalpha(*expr) || *expr == '_') {
    // Ignore the name:
    while (isalnum(*expr) || *expr == '_') ++expr;
    // Ignore white spaces:
    while (isspace(*expr)) ++expr;
  }

  data->rpn.push(new CompiledFunc(expr, &expr, data->scope));
  data->lastTokenWasOp = false;

  data->handle_op("()");

  data->rpn.push(new Tuple());
  data->lastTokenWasOp = false;

  *rest = expr;
}

struct Startup {
  Startup() {
    rWordMap_t& rwMap = calculator::default_rWordMap();
    rwMap["new"] = &new_parser;
    rwMap["function"] = &function_parser;

    OppMap_t& opp = calculator::default_opPrecedence();
    opp.add("new_op", 14);

    opMap_t& opMap = calculator::default_opMap();
    opMap.add({MAP, "new_op", TUPLE}, &new_operator);
  }
} Startup;

}  // namespace custom_features
