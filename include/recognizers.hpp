#pragma once

#include "utilities/image.hpp"
#include <functional>
#include <vector>

namespace recognizers {
// TODO: add model struct
struct Model {};
// Function factory that facilitates the use of FaceNet model
template <typename T = double>
std::function<std::vector<T>(image_tools::Image)> facenet_factory(Model *model);
} // namespace recognizers
