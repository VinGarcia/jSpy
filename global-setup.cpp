
#include "./exp-parser/shunting-yard.h"
#include <cstdio>
#include <sstream>

struct lineIterator : public Iterator {
  FILE* file;
  packToken last;

  lineIterator(FILE* fp) : file(fp) {}

  packToken* next() {
    char c = fgetc(file);
    std::stringstream ss;

    if (c == EOF || c == '\0') {
      reset();
      return 0;
    }

    // Read the line until it ends:
    while (c != '\0' && c != EOF && c != '\n' && c != '\r') {
      ss << c;
      c = fgetc(file);
    }

    // If the file is over:
    if (c == EOF || c == '\0') {
      // Put the last character back:
      ungetc(c, file);
    } else {
      char peek = fgetc(file);
      // If the next char is not part of the End Of Line:
      if ((peek != '\r' && peek != '\n') || c == peek) {
        // Put it back:
        ungetc(peek, file);
      }
    }

    last = ss.str();
    return &last;
  }

  void reset() { fseek(file, 0, SEEK_SET); }

  TokenBase* clone() const {
    return new lineIterator(*this);
  }
};

const char* write_args[] = {"text"};
packToken file_write(TokenMap scope) {
  TokenMap _this = scope.find("this")->asMap();
  std::string text = scope.find("text")->asString();

  FILE* file = reinterpret_cast<FILE*>(_this["fp"].asInt());

  fprintf(file, "%s", text.c_str());

  return packToken::None;
}

packToken file_read(TokenMap scope) {
  TokenMap _this = scope.find("this")->asMap();

  FILE* file = reinterpret_cast<FILE*>(_this["fp"].asInt());

  char c = fgetc(file);
  std::stringstream ss;
  while (c && c != EOF) {
    ss << c;
    c = fgetc(file);
  }

  return ss.str();
}

packToken file_readlines(TokenMap scope) {
  TokenMap _this = scope.find("this")->asMap();

  FILE* file = reinterpret_cast<FILE*>(_this["fp"].asInt());

  if (file == 0) {
    throw std::runtime_error("Cannot read from a closed file!");
  }

  return lineIterator(file);
}

packToken file_close(TokenMap scope) {
  TokenMap _this = scope.find("this")->asMap();

  FILE* file = reinterpret_cast<FILE*>(_this["fp"].asInt());
  fclose(file);
  _this["fp"] = 0;

  return packToken::None;
}

// The base class for the file objects.
TokenMap BASE_file;

const char* open_args[] = {"addr", "mode"};
packToken default_open(TokenMap scope) {
  std::string addr = scope.find("addr")->asString();
  packToken p_mode = *scope.find("mode");
  std::string mode;

  if (p_mode->type == NONE) {
    mode = "r";
  } else {
    mode = p_mode.asString();
  }

  FILE* file = fopen(addr.c_str(), mode.c_str());

  TokenMap obj_file = BASE_file.getChild();
  obj_file["fp"] = reinterpret_cast<int64_t>(file);

  return obj_file;
}

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
    global["open"] = CppFunction(&default_open, 2, open_args, "open");

    // Add a default class to global scope:
    global["Lazy"] = TokenMap();
    global["Lazy"]["__init__"] = CppFunction(&lazy_class_init, 1, lazy_args);
    global["Lazy"]["exec"] = CppFunction(&lazy_class_exec);

    // Startup the BASE_file prototype:
    BASE_file["readlines"] = CppFunction(file_readlines);
    BASE_file["read"] = CppFunction(file_read);
    BASE_file["write"] = CppFunction(file_write, 1, write_args);
    BASE_file["close"] = CppFunction(file_close);
  }
} global_startup;
