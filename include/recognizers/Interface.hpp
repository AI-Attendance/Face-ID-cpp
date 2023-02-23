#pragma once

#include <vector>

namespace recognizers {
template <typename FloatType> class Recognizer {
public:
  virtual ~Recognizer() = default;
  // TODO: add face parameter
  virtual std::vector<FloatType> Recognize() = 0;
};
} // namespace recognizers
