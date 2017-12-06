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

  // Ignore white spaces:
  while (isspace(*expr)) ++expr;
  const char* name_start = expr;

  // Parse the class reference as an expression:
  TokenQueue_t name_rpn = calculator::toRPN(expr, data->scope, "(", &expr);

  const char* name_end = expr;

  // Move `class_rpn` to the top of `data->rpn`:
  while (!name_rpn.empty()) {
    TokenBase* token = name_rpn.front();
    name_rpn.pop();
    data->handle_token(token);
  }

  // Add the new operator to the queue:
  data->handle_op("new_op");

  // Find the open bracket:
  while (*expr && *expr != '(') ++expr;

  if (*expr != '(') {
    // Expected '(', throw error:
    while (isspace(name_end[-1])) --name_end;
    std::string name(name_start, name_end - name_start);
    throw syntax_error("Expected '(' after `new " + name + "`");
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

  data->handle_token(new CompiledFunc(expr, &expr, data->scope));

  data->handle_op("()");

  data->handle_token(new Tuple());

  *rest = expr;
}

struct Startup {
  Startup() {
    parserMap_t& parserMap = calculator::Default().parserMap;
    parserMap.add("new", &new_parser);
    parserMap.add("function", &function_parser);

    OppMap_t& opp = calculator::Default().opPrecedence;
    opp.add("new_op", 14);

    opMap_t& opMap = calculator::Default().opMap;
    opMap.add({MAP, "new_op", TUPLE}, &new_operator);
  }
} Startup;

}  // namespace custom_features
