#pragma once
#include <string.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

namespace image_tools {
// TODO: add struct contents
	#define Image cv::Mat
	void resize(Image& src, Image& dst, int hight, int width);
	void pad(Image& src, Image& dst, int hight, int width);
	void read_from_cam(Image& src, const std::string& ip_cam);
	void read_from_cam(Image& src, const int ip_cam);
	char display(const Image& src, int wait_time, const std::string& winname);
	void read_from_disk(Image& dst, const std::string& dir);
	void save_to_disk(Image& src, const std::string& dir);
	void distroyWin(const std::string& winname);
} // namespace image_tools
