
#include "./exp-parser/shunting-yard.h"

struct GlobalStartup {
  GlobalStartup() {
    TokenMap& global = TokenMap::default_global();
    global["global"] = packMap(&global);
  }
} global_startup;
