#include "recognizers.hpp"

template <typename T>
std::function<std::vector<T>(image_tools::Image)>
facenet_factory(recognizers::Model *model) {
  // do some preparations
  return [model](image_tools::Image image) -> std::vector<T> {
    // do other things to construct feature vector from an image
  };
}
