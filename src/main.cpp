#include <inference_engine.hpp>
#include <iostream>
#include <lite/ncnn/cv/ncnn_facenet.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <utilities/image.hpp>

using namespace std;
using namespace cv;
using namespace image_tools;
int main() {

  // auto face = ncnncv::NCNNFaceNet("test", "test");
  cout << "test cout \n";
  VideoCapture cam(0);
  if (!cam.isOpened()) {
    cout << "Not opened!" << endl;
    return 1;
  }
  Mat frame;
  cam >> frame;
  pad(frame, frame, 1000, 1000);
  cv::imshow("test", frame);
  cout << frame.size[0];

  while (true) {
    Mat frame;
    cam >> frame;
    if (frame.empty()) {
      cout << "Can't get frame!" << endl;
      continue;
    }
    imshow("Frame", frame);
    int key = waitKey(1);
    if (key == 27 /*ESC*/)
      break;
  }
  destroyAllWindows();
  return 0;
}
