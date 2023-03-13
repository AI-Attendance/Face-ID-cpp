#pragma once

#include "utilities/image/image.hpp"
#include <functional>
#include <vector>

namespace detectors {
// TODO: add struct contents
struct FaceLandmarks {};
// TODO: add model struct
struct Model {};
// Function factory that facilitates the use of Yolo model
std::function<std::vector<FaceLandmarks>(image_tools::Image)>
yolo_factory(Model *model);
} // namespace detectors
