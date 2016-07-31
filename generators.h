
#ifndef GENERATORS_H_

#include <iostream>

#include "./exp-parser/shunting-yard.h"
#include "./code-parser.h"

class Range : public Iterable {
 private:
  struct Startup;
  struct RangeIterator;

 public:
  int64_t from, to, step;
  Range(int64_t f, int64_t t, int64_t s) : from(f), to(t), step(s) {}
  Iterator* getIterator();
};

class UserFunction : public Function {
  argsList _args;
  BlockStatement body;

 public:
  UserFunction(argsList args, BlockStatement body, std::string name = "")
               : _args(args), body(body) { this->name = name; }

  virtual const argsList args() const { return _args; }
  virtual packToken exec(packMap scope) const {
    returnState st;
    st = body.exec(scope);
    if (st.type == RETURN) {
      return st.value;
    } else {
      return packToken::None;
    }
  }

  virtual TokenBase* clone() const {
    return new UserFunction(static_cast<const UserFunction&>(*this));
  }
};

#endif  // GENERATORS_H_
