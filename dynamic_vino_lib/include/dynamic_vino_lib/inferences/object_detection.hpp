// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @brief A header file with declaration for ObjectDetection Class
 * @file object_detection.hpp
 */
#ifndef DYNAMIC_VINO_LIB__INFERENCES__OBJECT_DETECTION_HPP_
#define DYNAMIC_VINO_LIB__INFERENCES__OBJECT_DETECTION_HPP_
#include <object_msgs/msg/object.hpp>
#include <object_msgs/msg/object_in_box.hpp>
#include <object_msgs/msg/objects_in_boxes.hpp>
#include <object_msgs/srv/detect_object.hpp>
#include <rclcpp/rclcpp.hpp>
#include <memory>
#include <vector>
#include <string>
#include "dynamic_vino_lib/models/object_detection_model.hpp"
#include "dynamic_vino_lib/engines/engine.hpp"
#include "dynamic_vino_lib/inferences/base_inference.hpp"
#include "inference_engine.hpp"
#include "opencv2/opencv.hpp"
// namespace
namespace dynamic_vino_lib
{
/**
 * @class ObjectDetectionResult
 * @brief Class for storing and processing face detection result.
 */
class ObjectDetectionResult : public Result
{
public:
  friend class ObjectDetection;
  explicit ObjectDetectionResult(const cv::Rect & location);
  std::string getLabel() const
  {
    return label_;
  }
  /**
   * @brief Get the confidence that the detected area is a face.
   * @return The confidence value.
   */
  float getConfidence() const
  {
    return confidence_;
  }

private:
  std::string label_ = "";
  float confidence_ = -1;
};
/**
 * @class ObjectDetection
 * @brief Class to load face detection model and perform face detection.
 */
class ObjectDetection : public BaseInference
{
public:
  using Result = dynamic_vino_lib::ObjectDetectionResult;
  explicit ObjectDetection(double);
  ~ObjectDetection() override;
  /**
   * @brief Load the face detection model.
   */
  void loadNetwork(std::shared_ptr<Models::ObjectDetectionModel>);
  /**
   * @brief Enqueue a frame to this class.
   * The frame will be buffered but not infered yet.
   * @param[in] frame The frame to be enqueued.
   * @param[in] input_frame_loc The location of the enqueued frame with respect
   * to the frame generated by the input device.
   * @return Whether this operation is successful.
   */
  bool enqueue(const cv::Mat &, const cv::Rect &) override;
  /**
   * @brief Start inference for all buffered frames.
   * @return Whether this operation is successful.
   */
  bool submitRequest() override;
  /**
   * @brief This function will fetch the results of the previous inference and
   * stores the results in a result buffer array. All buffered frames will be
   * cleared.
   * @return Whether the Inference object fetches a result this time
   */
  bool fetchResults() override;
  /**
   * @brief Get the length of the buffer result array.
   * @return The length of the buffer result array.
   */
  const int getResultsLength() const override;
  /**
   * @brief Get the location of result with respect
   * to the frame generated by the input device.
   * @param[in] idx The index of the result.
   */
  const dynamic_vino_lib::Result * getLocationResult(int idx) const override;
  /**
   * @brief Show the observed detection result either through image window
     or ROS topic.
   */
  const void observeOutput(const std::shared_ptr<Outputs::BaseOutput> & output);
  /**
   * @brief Get the name of the Inference instance.
   * @return The name of the Inference instance.
   */
  const std::string getName() const override;

private:
  std::shared_ptr<Models::ObjectDetectionModel> valid_model_;
  std::vector<Result> results_;
  int width_ = 0;
  int height_ = 0;
  int max_proposal_count_;
  int object_size_;
  double show_output_thresh_ = 0;
};
}  // namespace dynamic_vino_lib
#endif  // DYNAMIC_VINO_LIB__INFERENCES__OBJECT_DETECTION_HPP_
