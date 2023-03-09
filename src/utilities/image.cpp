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

void image_tools::save_to_disk(Image& src, const std::string& dir)
{
	cv::imwrite(dir, src);
}

// tested
void image_tools::resize(Image& src, Image& dst, int height, int width)
{
	float f = src.size().aspectRatio() * ((float)height / width);
	if(src.size().aspectRatio() > (float(width) / height))
		height = int(height / f);
	else
		width = int(width * f);
	cv::resize(src, dst, cv::Size(width, height));
}
