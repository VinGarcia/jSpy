#include "./jspy-types.h"
#include "./cparse/shunting-yard-exceptions.h"

template <>
HashTag& packToken::as<HashTag>() const {
  if (base->type != TAG) {
    throw bad_cast(
        "The Token is not a HashTag!");
  }
  return *static_cast<HashTag*>(base);
}
