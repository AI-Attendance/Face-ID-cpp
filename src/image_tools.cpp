#include "opencv2/core.hpp"
#include "opencv2/core/base.hpp"
#include "opencv2/highgui.hpp"
#include <image_tools.hpp>
void image_tools::pad(const cv::Mat &src, cv::Mat &dst, int height, int width) {
  if (src.size().height > height || src.size().width > width) {
    return;
  }
  if (src.size[0] & 1 || src.size[1] & 1) {
    cv::resize(src, src, cv::Size(2 * src.size[0] / 2, 2 * src.size[0] / 2));
  }
  int top = (height - src.size().height) / 2;
  int left = (width - src.size().width) / 2;
  cv::copyMakeBorder(src, dst, top, top, left, left, cv::BORDER_CONSTANT);
}
