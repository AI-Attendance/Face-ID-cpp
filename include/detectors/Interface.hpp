#pragma once

namespace detectors {
// TODO: add struct contents
struct FaceLandmarks {};
class Detector {
public:
  virtual ~Detector() = default;
  // TODO: add image parameter
  virtual FaceLandmarks Detect() = 0;
};
} // namespace detectors
