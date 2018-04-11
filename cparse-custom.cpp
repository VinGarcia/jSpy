#include <sstream>

#include "./cparse/shunting-yard.h"
#include "./cparse/shunting-yard-exceptions.h"

#include "./statements.h"
#include "./range.h"
#include "./jspy-types.h"

#include "./cparse/builtin-features/functions.inc"
#include "./cparse/builtin-features/operations.inc"
#include "./cparse-custom/reservedWords.inc"
#include "./cparse/builtin-features/typeSpecificFunctions.inc"

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

  if (*name_start == '(') {
    throw syntax_error("Expected a class name after `new` operator!");
  }

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

packToken in_operator(const packToken& left, const packToken& right, evaluationData* data) {
  Iterator* it;

  if (right->type == IT) {
    it = static_cast<Iterator*>(right->clone());
  } else if (right->type & IT) {
    it = static_cast<const Iterable*>(right.token())->getIterator();
  } else {
    throw syntax_error("Invalid use of `in` operator on non-iterable token!");
  }

  for (packToken* value = it->next(); value; value = it->next()) {
    if (left == *value) return true;
  }

  delete it;

  return false;
}

void in_parser(const char* expr, const char** rest, rpnBuilder* data) {
  data->handle_op("in_op");
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

void HashTag_parser(const char* expr, const char** rest, rpnBuilder* data) {
  if (!rpnBuilder::isvarchar(*expr)) {
    throw syntax_error(
        "Expected tag name after `#` character!");
  }

  // Get the HashTag name id:
  std::string tag_name = rpnBuilder::parseVar(expr, &expr);
  data->handle_token(new HashTag(tag_name));

  // Ignore white spaces:
  while (isspace(*expr) && *expr != '\n') ++expr;

  // If there is a map declaration just after the tag,
  // add the operation that will join them at execution time:
  if (*expr == '{') {
    data->handle_op("tag_map_op");
  }

  if (rest) *rest = expr;
}

packToken tag_map_operator(
    const packToken& left, const packToken& right,
    evaluationData* data) {
  left.as<HashTag>().data = right;
  return left;
}

packToken HashTag_equals(
    const packToken& left, const packToken& right,
    evaluationData* data) {
  return left.as<HashTag>().tag_id == right.as<HashTag>().tag_id;
}

packToken HashTag_str(TokenMap scope) {
  const HashTag& tag = scope["this"].as<HashTag>();
  std::stringstream ss;
  ss << "#" << tag.name();

  if (tag.data->type != NONE) {
    ss << " " << tag.data.str();
  }

  return ss.str();
}

packToken HashTagIndex(const packToken& p_left, const packToken& p_right, evaluationData* data) {
  HashTag& h_left = p_left.as<HashTag>();
  std::string& right = p_right.asString();

  if (h_left.data->type == MAP) {
    TokenMap& left = h_left.data.asMap();
    packToken* p_value = left.find(right);
    if (p_value) {
      return RefToken(right, *p_value, h_left);
    }
  }

  return RefToken(right, packToken::None(), h_left);
}

struct Startup {
  Startup() {
    parserMap_t& parser = calculator::Default().parserMap;
    parser.add("new", &new_parser);
    parser.add("function", &function_parser);
    parser.add("func", &function_parser);
    parser.add("in", &in_parser);
    parser.add("#", &HashTag_parser);

    OppMap_t& opp = calculator::Default().opPrecedence;
    opp.add("new_op", 14);
    opp.add("in_op", 8);
    opp.add("tag_map_op", 3);

    opMap_t& opMap = calculator::Default().opMap;
    opMap.add({MAP, "new_op", TUPLE}, &new_operator);
    opMap.add({ANY_TYPE, "in_op", IT}, &in_operator);
    opMap.add({TAG, "tag_map_op", MAP}, &tag_map_operator);
    opMap.add({TAG, "==", TAG}, &HashTag_equals);
    opMap.add({TAG, "[]", STR}, &HashTagIndex);
    opMap.add({TAG, ".", STR}, &HashTagIndex);

    TokenMap& base_tag = calculator::type_attribute_map()[TAG];
    base_tag["__str__"] = CppFunction(&HashTag_str, "__str__");
  }
} Startup;

}  // namespace custom_features
