#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

using namespace std;
using namespace cv;

int main() {
  VideoCapture cam(0);
  if (!cam.isOpened()) {
    cout << "Not opened!" << endl;
    return 1;
  }
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
