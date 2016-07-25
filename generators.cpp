#include <cstring>

#include "./generators.h"

class Range : public Iterator {
  long from, to, step, i;

 public:
  Range(long from, long to, long step) : from(from), to(to), step(step), i(from) {
    this->type = IT;
  }

  packToken* next() {
    long value = i;
    if ((step > 0 && value >= to) || (step < 0 && value <= to)) {
      i = from;
      return NULL;
    } else {
      i += step;
      return new packToken((double)value);
    }
  }

  void reset() {
    i=from;
  }

  virtual TokenBase* clone() const {
    return new Range(static_cast<const Range&>(*this));
  }
};

const char* range_args[] = {"from", "to", "step"};
packToken default_range(TokenMap* scope) {
  long to, step, from;

  packToken* p_from = scope->find("from");
  packToken* p_to = scope->find("to");
  packToken* p_step = scope->find("step");

  if ((*p_from)->type == NUM) {
    from = p_from->asDouble();
  } else if ((*p_from)->type == NONE) {
    throw std::invalid_argument("range() expects at least the first argument!");
  } else {
    throw std::invalid_argument("range() expects only numbers!");
  }

  if ((*p_to)->type == NONE) {
    to = from;
    from = 0;
    step = 1;
  } else if ((*p_to)->type == NUM) {
    to = p_to->asDouble();

    if ((*p_step)->type == NUM) {
      step = p_step->asDouble();
    } else if ((*p_step)->type == NONE) {
      step = 1;
    } else {
      throw std::invalid_argument("range() expects only numbers!");
    }
  } else {
    throw std::invalid_argument("range() expects only numbers!");
  }

  return Range(from, to, step);
}

/* * * * * Iterator Startup class * * * * */

struct Iterator::Startup {
  Startup() {
    TokenMap& global = TokenMap::default_global();
    global["range"] = CppFunction(default_range, 3, range_args);
  }
} iterator_startup;
