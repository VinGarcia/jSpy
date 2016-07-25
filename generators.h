
#ifndef GENERATORS_H_

#include <iostream>

#include "./exp-parser/shunting-yard.h"
#include "./code-parser.h"

// These are token numbers used to extend
// The default Token types of the calculator class.
//
// They should all have the format 0x2#
// Where # is an optional number between 0 and f
//
// This way they won't colide with the original types.
enum newTokTypes { IT = 0x20 };

class Iterator : public TokenBase {
  struct Startup;

 public:
  virtual packToken* next() = 0;
  virtual void reset() = 0;
};

class UserFunction : public Function {
  argsList _args;
  BlockStatement body;

 public:
  UserFunction(argsList args, BlockStatement body, std::string name = "")
               : _args(args), body(body) { this->name = name; }

  virtual const argsList args() const { return _args; };
  virtual packToken exec(TokenMap* scope) const {
    body.exec(scope);
    return packToken::None;
  }

  virtual TokenBase* clone() const {
    return new UserFunction(static_cast<const UserFunction&>(*this));
  }
};


// class Generator : public Iterator {
//   TokenMap local;

// };

#endif  // GENERATORS_H_
