

#ifndef GENERATORS_H_

#include "./exp-parser/shunting-yard.h"

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

// class Generator : public Iterator {
//   TokenMap_t local;

// };

#endif  // GENERATORS_H_
