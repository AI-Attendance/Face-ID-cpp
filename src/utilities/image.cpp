#include "utilities/image.hpp"

// TODO how to release camera
void image_tools::read_from_cam(Image& dst, const std::string& ip_cam)
{
	static auto cam = cv::VideoCapture(ip_cam);
	cam.read(dst);
}

// tested
void image_tools::read_from_cam(Image& dst, const int ip_cam)
{
	static auto cam = cv::VideoCapture(ip_cam);
	cam.read(dst);
}

//tested
char image_tools::display(const Image& src, int wait_time, const std::string& winname="frame")
{
	cv::imshow(winname, src);
	return cv::waitKey(wait_time);
}

// tested
void image_tools::distroyWin(const std::string& winname="frame")
{
	cv::destroyWindow(winname);
}

void image_tools::read_from_disk(Image& dst, const std::string& dir)
{
	dst = cv::imread(dir);
	if(dst.data == NULL)
		std::cout << "Error happen when reading image!\n";
}

void image_tools::save_to_disk(const Image& src, const std::string& dir)
{
	cv::imwrite(dir, src);
}

// tested
void image_tools::resize(const Image& src, Image& dst, int height, int width)
{
	float f = src.size().aspectRatio() * ((float)height / width);
	if(src.size().aspectRatio() > (float(width) / height))
		height = int(height / f);
	else
		width = int(width * f);
	cv::resize(src, dst, cv::Size(width, height));
}

//tested
void image_tools::pad(const Image &src, Image &dst, int height, int width)
{
	if (src.size().height > height || src.size().width > width)
		return;

	if (src.size[0] & 1 || src.size[1] & 1)
		cv::resize(src, src, cv::Size(2 * src.size[0] / 2, 2 * src.size[0] / 2));

	int top = (height - src.size().height) / 2;
	int left = (width - src.size().width) / 2;
	cv::copyMakeBorder(src, dst, top, top, left, left, cv::BORDER_CONSTANT);
}

#include <iostream>

bool image_tools::test_face(const Image& src, const float black_thres, const float blur_thres)
{
	// test 1
	// if the image has too much black padding
	Image inverted, gray;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	float presentage_of_black = 1 - cv::countNonZero(gray) /
										(float)(gray.size().width * gray.size().height);
	if(presentage_of_black > black_thres)
		return false;

	// test 2
	// if the image is blurry
	// gray mat from above code
	// depend on the size of the frame
	Image laplacian;
	cv::Scalar mean, stddev;
    Laplacian(gray, laplacian, CV_64F);
	cv::meanStdDev(laplacian, mean, stddev);
	float sdv = stddev.val[0] * stddev.val[0];
	std::cout << "SDV: " << sdv << std::endl;
	if (sdv < blur_thres)
		return false;

	return true;
}
