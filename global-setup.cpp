
#include "./exp-parser/shunting-yard.h"

// Function for instantiating instances of a class.
const char* new_args[] = {"parent"};
packToken default_new(TokenMap scope) {
  TokenMap& parent = scope.find("parent")->asMap();
  TokenList list = scope.find("args")->asList();

  // Intantiate a new Map:
  packToken instance = TokenMap(&parent);

  // Call its init function if it exists:
  if (parent.map().count("__init__") && parent["__init__"]->type == FUNC) {
    Function* func = parent["__init__"].asFunc();
    Tuple tuple;
    // Copy the remaining arguments to this tuple:
    for (packToken& arg : list.list()) {
      tuple.tuple.push_back(arg->clone());
    }

    Function::call(instance, func, &tuple, scope);
  }

  // Return it:
  return instance;
}

const char* one_arg[] = {"value"};
packToken default_rpn(TokenMap scope) {
  std::string code = scope.find("value")->asString();

  calculator c(code.c_str(), scope);

  return c.str();
}

/* * * * * lazy built-in class * * * * */
const char* lazy_args[] = {"func"}; 
packToken lazy_class_init(TokenMap scope) {
  TokenMap _this = scope.find("this")->asMap();
  _this["func"] = packToken(scope.find("func")->asFunc()->clone());
  _this["args"] = scope.find("args")->asList();

  return packToken::None;
}

packToken lazy_class_exec(TokenMap scope) {
  TokenMap _this = scope.find("this")->asMap();
  Function* func = _this["func"].asFunc();
  TokenList args = _this["args"].asList();

  Tuple tuple;
  // Copy the tokens from this.args:
  for (packToken item : args.list()) {
    tuple.push_back(item);
  }

  // Copy the tokens received as exec() arguments:
  args = scope.find("args")->asList();
  for (packToken item : args.list()) {
    tuple.push_back(item);
  }

  return Function::call(_this, func, &tuple, *(scope.parent()));
}

/* * * * * Global Startup * * * * */

struct GlobalStartup {
  GlobalStartup() {
    TokenMap& global = TokenMap::default_global();
    global["global"] = global;
    global["new"] = CppFunction(&default_new, 1, new_args, "new");
    global["rpn"] = CppFunction(&default_rpn, 1, one_arg, "rpn");

    // Add a default class to global scope:
    global["Lazy"] = TokenMap();
    global["Lazy"]["__init__"] = CppFunction(&lazy_class_init, 1, lazy_args);
    global["Lazy"]["exec"] = CppFunction(&lazy_class_exec);
  }
} global_startup;
