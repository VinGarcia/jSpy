#include "./matcher.h"
#include "./exp-parser/shunting-yard-exceptions.h"

/* * * * * HOOK type * * * * */

// This value is used as the type for Hook Tokens.
// It was chosen so it won't colide with the default
// types defined on: `exp-parser/shunting-yard.h`
#define HOOK 0x10

// Function used to extract a Hook from a packToken:
Hook asHook(packToken& p_hook) {
  TokenBase* b_hook = p_hook.token();
  if (b_hook->type == HOOK) {
    return static_cast<Token<Hook>*>(b_hook)->val;
  } else {
    throw bad_cast("The token is not of HOOK type!");
  }
}

/* * * * * Matcher built-in class * * * * */

// Its the super class of all matcher objects:
TokenMap& Matcher_super_class() {
  static TokenMap matcher_super;
  return matcher_super;
}

// The exec function of all matcher objects:
const char* matcher_args[] = {"text"};
packToken matcher_exec(TokenMap scope) {
  TokenList list;
  std::string text = scope.find("text")->asString();
  TokenMap _this = scope.find("this")->asMap();
  TokenList hooks = _this["hooks"].asList();

  for (packToken& p_hook : hooks.list()) {

    // Extract the hook from the packToken:
    Hook hook = asHook(p_hook);

    // Ignore patterns if the guard evaluates to 'false':
    if (!hook.cond.eval(scope).asBool()) continue;
    
    Iterator* it = hook.getIterator(text, scope);

    packToken* map = it->next();

    returnState rs;
    while (map) {
      rs = hook.body.exec(map->asMap());

      if (rs.type != NORMAL && rs.value->type != NONE) {
        list.list().push_back(rs.value);
      }

      if (rs.type == FINISH) {
        delete it;
        return list;
      }

      map = it->next();
    }

    delete it;
  }

  return list;
}

// Build the Matcher_super_class:
struct MatcherStartup {
  MatcherStartup() {
    TokenMap& global = TokenMap::default_global();
    global["Matcher"] = Matcher_super_class();
    TokenMap& matcher_super = Matcher_super_class();
    matcher_super["exec"] = CppFunction(&matcher_exec, 1, matcher_args);
  }
} matcher_startup;

/* * * * * MatcherDeclaration Statement * * * * */

// From statements.cpp:
std::string parseName(const char** source, char end_char = '\n');

void MatcherDeclaration::_compile(const char* code, const char** rest,
                                  TokenMap parent_scope) {
  hooks.list().clear();

  // Find the start of the name:
  while (isspace(*code)) ++code;

  try {
    name = parseName(&code);
  } catch (const syntax_error& e) {
    throw syntax_error("Missing name on matcher declaration!");
  }

  // Find the start of the definition:
  while (isspace(*code)) ++code;

  // If there is a single pattern on this matcher:
  if (*code == '"') {
    // Add a new hook:
    TokenBase* hook = new Token<Hook>(Hook(code, &code, parent_scope), HOOK);
    hooks.list().push_back(packToken(hook));

    if (rest) *rest = code;
    return;
  }

  if (*code != '{') {
    throw syntax_error("Missing '{' or '\"' after `matcher` statement!");
  }

  ++code;

  // Find the first hook statement
  while (isspace(*code)) ++code;
  
  while (*code && *code != '}') {

    if(*code != '"') {
      throw syntax_error("A matcher pattern should start with `\"`!");
    }

    // Add a new hook:
    TokenBase* hook = new Token<Hook>(Hook(code, &code, parent_scope), HOOK);
    hooks.list().push_back(packToken(hook));

    // Find the start of the next pattern:
    while (isspace(*code)) ++code;
  }

  if (*code != '}') {
    throw syntax_error("Missing '}' at end of `matcher { ...` statement!");
  }

  ++code;

  if (rest) *rest = code;
}

returnState MatcherDeclaration::_exec(TokenMap scope) const {

  // Built an instance of the built-in class matcher:
  TokenMap instance = TokenMap( &Matcher_super_class() ); 
  instance["hooks"] = hooks;

  // Add it to the scope:
  scope[name] = instance;

  // Also register the hooks on the objectClass label list:
  pMatch::objectClass::labels[name] = new Matcher(hooks);

  return NORMAL;
}

bool Matcher::match(std::string input, uint pos) {
  // Limpa a lista de interpretações:
  this->var.lInt.clear();
  this->match_word.clear();

  bool match;
  for(packToken& p_hook : hooks.list()) {
    Hook hook = asHook(p_hook);
    match = hook.expr.match(input, pos);

    if (match) {
      this->var.lInt.splice(this->var.lInt.end(), hook.expr.var.lInt);
      this->match_word.splice(match_word.end(), hook.expr.getMatch());
    }
  }

  if (this->var.lInt.empty()) {
    return false;
  } else {
    return true;
  }
}

/* * * * * MatchIterator * * * * */
// This Iterator is used only by the Hook Parser class

struct MatchIterator : public Iterator {
  pMatch::arrayClass expr;
  std::string text;
  TokenMap scope;

  packToken last;
  bool more, match;

  MatchIterator(pMatch::arrayClass expr, std::string text,
                TokenMap scope = &TokenMap::empty)
                : expr(expr), text(text), scope(scope) {
    reset();
  }

  packToken* next() {
    if (match && this->more) {
      bool more = false;
      last = traverse(expr.var, &more);
      this->more = more;
      return &last;
    } else {
      reset();
      return NULL;
    }
  }

  void reset() {
    this->more = true;
    match = expr.match(text, 0, true);
  }

  // The traverse will build an object with one of the
  // possible interpretations for the text.
  //
  // And then it will delete that interpretation in particular.
  // 
  // If there are more interpretations possible after it returned,
  // the `*p_more` argument will be set to `true`.
  TokenMap traverse(pMatch::cVar& var, bool* p_more) {
    using namespace pMatch;

    // Create a new empty object:
    TokenMap obj = scope.getChild();

    if (var.lInt.size() == 0) {
      return obj;
    }

    // Get the first interpretation:
    tInterpretacao tInt = var.lInt.front();
    // Prepare to drop it, if not stated otherwise:
    bool keep_tInt = false;

    // If there is more than one interpretation
    // Even if we drop the first one
    // there will be more interpretations so:
    if (var.lInt.size() > 1) *p_more = true;

    // Read the word this variable represents:
    obj["text"] = tInt.word;

    // For each variable in this interpretation:
    for (cVar sub_var : tInt.var) {
      // If it has a name:
      if (sub_var.nome != "") {

        // Make this name readable:
        std::string nome;
        if (sub_var.nome[0]=='[') {
          nome = sub_var.nome.substr(1, sub_var.nome.size()-1);
        } else {
          nome = sub_var.nome;
        }

        // Save it on `obj`:
        TokenMap sub_obj = traverse(sub_var, &keep_tInt);
        if (sub_obj.map().size() == 1) {
          // If its just a string:
          obj[nome] = sub_obj["text"];
        } else {
          obj[nome] = sub_obj;
        }
      }
    }

    // If none of the subvariables had more than one interpretation:
    if (keep_tInt == false) {
      if (var.lInt.size() > 1) {
        // Discard the first interpretation:
        var.lInt.pop_front();
      }
    } else {
      // Make sure to inform there are more interpretations:
      *p_more = true;
    }

    return obj;
  }

  TokenBase* clone() const {
    return new MatchIterator(*this);
  }
};

/* * * * * Hook Parser * * * * */

void Hook::compile(const char* code, const char** rest,
                   TokenMap parent_scope) {
  // Find the start of the expression:
  while (isspace(*code)) ++code;

  if (*code != '"') {
    throw syntax_error("Expected match expression after `matcher` reserved word!");
  }

  // Build the hook expression:
  uint pos = 0;
  expr = pMatch::arrayClass(std::string(code), &pos);

  // Update code pointer:
  code += pos;

  // Skip white spaces:
  while (isspace(*code)) ++code;

  // Check to see if there is an if() statement:
  if (*code == 'i' &&
      code[1] == 'f' &&
      !(isalnum(code[2]) || code[2] == '_')) {
    
    code += 2;
    while (isspace(*code)) ++code;

    if (*code != '(') {
      throw syntax_error("Expected '(' after `if` statement!");
    }

    ++code;
    cond.compile(code, parent_scope, ")", &code);

    if (*code != ')') {
      throw syntax_error("Missing ')' after `if` statement!");
    }

    ++code;
  } else {
    cond.compile("True");
  }

  while (*code && isspace(*code)) ++code;

  if (*code && *code != ';' && *code != '}') {
    body.compile(code, &code, parent_scope);
  } else {
    if(*code == ';') ++code;
  }

  if (rest) *rest = code;
}

Iterator* Hook::getIterator(std::string text, TokenMap scope) const {
  return new MatchIterator(expr, text, scope);
}

