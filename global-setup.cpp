
#include "./exp-parser/shunting-yard.h"

// Function for instantiating instances of a class.
const char* new_args[] = {"parent"};
packToken default_new(packMap scope) {
  packMap& parent = scope->find("parent")->asMap();
  packList list = scope->find("arglist")->asList();

  // Intantiate a new Map:
  packToken instance = packMap(TokenMap(parent));

  // Call its init function if it exists:
  if (parent->map.count("__init__") && (*parent)["__init__"]->type == FUNC) {
    Function* func = (*parent)["__init__"].asFunc();
    Tuple tuple;
    // Copy the remaining arguments to this tuple:
    for (packToken& arg : list->list) {
      tuple.tuple.push_back(arg->clone());
    }

    Function::call(instance, func, &tuple, scope);
  }

  // Return it:
  return instance;
}

struct GlobalStartup {
  GlobalStartup() {
    TokenMap& global = TokenMap::default_global();
    global["global"] = packMap(&global);
    global["new"] = CppFunction(&default_new, 1, new_args, "new");
  }
} global_startup;
