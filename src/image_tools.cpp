#include "opencv2/core.hpp"
#include "opencv2/core/base.hpp"
#include "opencv2/highgui.hpp"
#include <image_tools.hpp>
void image_tools::pad(const cv::Mat &src, cv::Mat &dst, int height, int width) {
  cv::copyMakeBorder(src, dst, height, height, width, width,
                     cv::BORDER_CONSTANT, 0);
  cv::imshow("test", dst);
}
