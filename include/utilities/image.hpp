#pragma once
#include <string.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

namespace image_tools {
// TODO: add struct contents
	//typedef cv::Mat Image;
#define Image cv::Mat
	void resize(const Image& src, Image& dst, int hight, int width);
	void pad(const Image& src, Image& dst, int hight, int width);
	void read_from_cam(Image& dst, const std::string& ip_cam);
	void read_from_cam(Image& dst, const int ip_cam);
	char display(const Image& src, int wait_time, const std::string& winname);
	void read_from_disk(Image& dst, const std::string& dir);
	void save_to_disk(const Image& src, const std::string& dir);
	void distroyWin(const std::string& winname);
	bool test_face(const Image& src, const float black_thres=0.2, const float blur_thres=100); // before running face recognition
} // namespace image_tools
