#include "detectors/common.hpp"

std::function<std::vector<detectors::FaceLandmarks>(image_tools::Image)>
yolo_factory(detectors::Model *model) {
  // do some preparations
  return
      [model](
          image_tools::Image image) -> std::vector<detectors::FaceLandmarks> {
        // do other things to construct landmarks from an image
      };
}
