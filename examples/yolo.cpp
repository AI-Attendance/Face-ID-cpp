#include <cstddef>
#include <iostream>
#include <string>
// #include <iterator>
// #include <memory>
// #include <sstream>
// #include <string>
// #include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <openvino/openvino.hpp>

using namespace std;

void printInputAndOutputsInfo(const ov::Model &network) {
  cout << "model name: " << network.get_friendly_name() << endl;

  const std::vector<ov::Output<const ov::Node>> inputs = network.inputs();
  for (const ov::Output<const ov::Node> &input : inputs) {
    cout << "    inputs" << endl;

    const std::string name =
        input.get_names().empty() ? "NONE" : input.get_any_name();
    cout << "        input name: " << name << endl;

    const ov::element::Type type = input.get_element_type();
    cout << "        input type: " << type << endl;

    const ov::Shape shape = input.get_shape();
    cout << "        input shape: " << shape << endl;
  }
  const std::vector<ov::Output<const ov::Node>> outputs = network.outputs();
  for (const ov::Output<const ov::Node> &output : outputs) {
    cout << "    outputs" << endl;

    const std::string name =
        output.get_names().empty() ? "NONE" : output.get_any_name();
    cout << "        output name: " << name << endl;

    const ov::element::Type type = output.get_element_type();
    cout << "        output type: " << type << endl;

    const ov::Shape shape = output.get_shape();
    cout << "        output shape: " << shape << endl;
  }
}

#define NUM_KEYPOINTS 5

struct Point {
  float x;
  float y;
  float prob;
};

typedef struct Object {
  float x1;
  float y1;
  float x2;
  float y2;
  float score;
  int _class;
  Point landmark[NUM_KEYPOINTS];
	Object(const ov::Tensor &tensor, int begin){
		x1 = tensor.data<float>()[begin];
		y1 = tensor.data<float>()[begin + 1];
		x2 = tensor.data<float>()[begin + 1];
		y2 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
		x1 = tensor.data<float>()[begin + 1];
	}
} Object;

#include <algorithm>
#include <vector>

void nms(std::vector<Object> &input_boxes, float NMS_THRESH = 0.45f) {
  sort(input_boxes.begin(), input_boxes.end(),
       [](Object b1, Object b2) { return b1.score > b2.score; });
  std::vector<float> vArea(input_boxes.size());
  for (int i = 0; i < int(input_boxes.size()); ++i) {
    vArea[i] = (input_boxes.at(i).x2 - input_boxes.at(i).x1 + 1) *
               (input_boxes.at(i).y2 - input_boxes.at(i).y1 + 1);
  }
  for (int i = 0; i < int(input_boxes.size()); ++i) {
    for (int j = i + 1; j < int(input_boxes.size());) {
      float xx1 = std::max(input_boxes[i].x1, input_boxes[j].x1);
      float yy1 = std::max(input_boxes[i].y1, input_boxes[j].y1);
      float xx2 = std::min(input_boxes[i].x2, input_boxes[j].x2);
      float yy2 = std::min(input_boxes[i].y2, input_boxes[j].y2);
      float w = std::max(float(0), xx2 - xx1 + 1);
      float h = std::max(float(0), yy2 - yy1 + 1);
      float inter = w * h;
      float ovr = inter / (vArea[i] + vArea[j] - inter);
      if (ovr >= NMS_THRESH) {
        input_boxes.erase(input_boxes.begin() + j);
        vArea.erase(vArea.begin() + j);
      } else {
        j++;
      }
    }
  }
}

std::vector<Object> tensor_to_object_vector(const ov::Tensor &tensor) {
	const int boxes = 3024;
  std::vector<Object> object_vector(boxes);
	for(int i = 0; i < boxes; i++)
	return object_vector;
}

int main(int argc, char **argv) {
  try {
    // -------- Get OpenVINO runtime version --------
    cout << ov::get_openvino_version() << endl;

    // -------- Step 1. Initialize OpenVINO Runtime Core --------
    ov::Core core;

    // -------- Step 2. Read a model --------
    string model_path = "../Face-ID/Models/yolov7-lite-e/model.xml";
    std::shared_ptr<ov::Model> model = core.read_model(model_path);
    printInputAndOutputsInfo(*model);

    // OPENVINO_ASSERT(model->inputs().size() == 1,
    //                 "Sample supports models with 1 input only");
    //  OPENVINO_ASSERT(model->outputs().size() == 1,
    //                  "Sample supports models with 1 output only");

    //// -------- Step 3. Set up input
    string image_path = "../Face-ID/Faces Pictures/Ahmed Yasser/4885.jpg";
    auto image = cv::imread(image_path);
    //// Read input image to a tensor and set it to an infer request
    //// without resize and layout conversions
    // FormatReader::ReaderPtr reader(image_path.c_str());
    // if (reader.get() == nullptr) {
    //   std::stringstream ss;
    //   ss << "Image " + model_path + " cannot be read!";
    //   throw std::logic_error(ss.str());
    // }

    ov::element::Type input_type = ov::element::u8;
    ov::Shape input_shape = {1, (size_t)image.rows, (size_t)image.cols, 3};
    unsigned char *input_data = image.data;

    //// just wrap image data by ov::Tensor without allocating of new memory
    ov::Tensor input_tensor = ov::Tensor(input_type, input_shape, input_data);

    const ov::Layout tensor_layout{"NHWC"};

    //// -------- Step 4. Configure preprocessing --------

    ov::preprocess::PrePostProcessor ppp(model);

    //// 1) Set input tensor information:
    //// - input() provides information about a single model input
    //// - reuse precision and shape from already available `input_tensor`
    //// - layout of data is 'NHWC'
    ppp.input()
        .tensor()
        .set_shape(input_shape)
        .set_element_type(input_type)
        .set_layout(tensor_layout);
    //// 2) Adding explicit preprocessing steps:
    //// - convert layout to 'NCHW' (from 'NHWC' specified above at tensor
    /// layout) / - apply linear resize from tensor spatial dims to model
    /// spatial dims
    ppp.input().preprocess().resize(
        ov::preprocess::ResizeAlgorithm::RESIZE_LINEAR);
    //// 4) Here we suppose model has 'NCHW' layout for input
    ppp.input().model().set_layout("NCHW");
    //// 5) Set output tensor information:
    //// - precision of tensor is supposed to be 'f32'
    // ppp.output().tensor().set_element_type(ov::element::f32);

    //// 6) Apply preprocessing modifying the original 'model'
    model = ppp.build();

    //// -------- Step 5. Loading a model to the device --------
    ov::CompiledModel compiled_model = core.compile_model(model, "CPU");

    //// -------- Step 6. Create an infer request --------
    ov::InferRequest infer_request = compiled_model.create_infer_request();
    ////
    ///-----------------------------------------------------------------------------------------------------

    //// -------- Step 7. Prepare input --------
    infer_request.set_input_tensor(input_tensor);

    //// -------- Step 8. Do inference synchronously --------
    infer_request.infer();

    //// -------- Step 9. Process output
    // const ov::Tensor &output_tensor = infer_request.get_output_tensor();
    auto output = infer_request.get_tensor("2022");

    //// Print classification results
    // ClassificationResult classification_result(output_tensor,
    // {image_path}); classification_result.show();
    //  -----------------------------------------------------------------------------------------------------
  } catch (const std::exception &ex) {
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
