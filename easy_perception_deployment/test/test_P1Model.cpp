// Copyright 2020 Advanced Remanufacturing and Technology Centre
// Copyright 2020 ROS-Industrial Consortium Asia Pacific Team
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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "bits/stdc++.h"
#include "epd_utils_lib/epd_container.hpp"
#include "gtest/gtest.h"
// OpenCV LIB
#include "opencv2/opencv.hpp"

std::string PATH_TO_SESSION_CONFIG(PATH_TO_PACKAGE "/data/session_config.txt");
std::string PATH_TO_USECASE_CONFIG(PATH_TO_PACKAGE "/data/usecase_config.txt");
std::string PATH_TO_ONNX_MODEL(PATH_TO_PACKAGE "/data/model/squeezenet1.1-7.onnx");
std::string PATH_TO_LABEL_LIST(PATH_TO_PACKAGE "/data/label_list/imagenet_classes.txt");
std::string PATH_TO_TEST_IMAGE(PATH_TO_PACKAGE "/data/9544757988_991457c228_z.jpg");

bool is_file_exist(const char * fileName)
{
  std::ifstream infile(fileName);
  return infile.good();
}

TEST(EPD_TestSuite, Test_loadP1ONNXModel_EPDContainer)
{
  if (!is_file_exist(PATH_TO_SESSION_CONFIG.c_str())) {
    system(("touch " + PATH_TO_SESSION_CONFIG).c_str());
    system(("echo " + PATH_TO_ONNX_MODEL + " >> " + PATH_TO_SESSION_CONFIG).c_str());
    system(("echo " + PATH_TO_LABEL_LIST + " >> " + PATH_TO_SESSION_CONFIG).c_str());
    system(("echo visualize >> " + PATH_TO_SESSION_CONFIG).c_str());
  } else {
    system(("rm " + PATH_TO_SESSION_CONFIG).c_str());
    system(("touch " + PATH_TO_SESSION_CONFIG).c_str());
    system(("echo " + PATH_TO_ONNX_MODEL + " >> " + PATH_TO_SESSION_CONFIG).c_str());
    system(("echo " + PATH_TO_LABEL_LIST + " >> " + PATH_TO_SESSION_CONFIG).c_str());
    system(("echo visualize >> " + PATH_TO_SESSION_CONFIG).c_str());
  }

  if (!is_file_exist(PATH_TO_USECASE_CONFIG.c_str())) {
    system(("touch " + PATH_TO_USECASE_CONFIG).c_str());
    system(("echo 0 >> " + PATH_TO_USECASE_CONFIG).c_str());
  } else {
    system(("rm " + PATH_TO_USECASE_CONFIG).c_str());
    system(("touch " + PATH_TO_USECASE_CONFIG).c_str());
    system(("echo 0 >> " + PATH_TO_USECASE_CONFIG).c_str());
  }

  EPD::EPDContainer * ortAgent_;

  ortAgent_ = new EPD::EPDContainer();

  EXPECT_EQ(ortAgent_->precision_level, unsigned(1));
  EXPECT_EQ(ortAgent_->classNames.size(), unsigned(1000));

  ortAgent_->setFrameDimension(1920, 1080);
  ortAgent_->initORTSessionHandler();

  ASSERT_EQ(!ortAgent_->p1_ort_session, false);

  // Download and load test image
  if (!is_file_exist(PATH_TO_TEST_IMAGE.c_str())) {
    system("apt-get install -y wget");
    system(
      "wget https://farm8.staticflickr.com/7329/9544757988_991457c228_z.jpg"
      " --directory-prefix " PATH_TO_PACKAGE "/data/");
  }

  cv::Mat frame = cv::imread(
    PATH_TO_TEST_IMAGE,
    cv::IMREAD_COLOR);
  std::vector<std::string> topK_obj_identities = ortAgent_->p1_ort_session->infer(
    frame);

  ASSERT_EQ(topK_obj_identities[0], "Irish setter, red setter ");

  if (!is_file_exist(PATH_TO_SESSION_CONFIG.c_str())) {
    system(("touch " + PATH_TO_SESSION_CONFIG).c_str());
    system(("echo " + PATH_TO_ONNX_MODEL + " >> " + PATH_TO_SESSION_CONFIG).c_str());
    system(("echo " + PATH_TO_LABEL_LIST + " >> " + PATH_TO_SESSION_CONFIG).c_str());
    system(("echo visualize >> " + PATH_TO_SESSION_CONFIG).c_str());
  } else {
    system(("rm " + PATH_TO_SESSION_CONFIG).c_str());
    system(("touch " + PATH_TO_SESSION_CONFIG).c_str());
    system(("echo " + PATH_TO_ONNX_MODEL + " >> " + PATH_TO_SESSION_CONFIG).c_str());
    system(("echo " + PATH_TO_LABEL_LIST + " >> " + PATH_TO_SESSION_CONFIG).c_str());
    system(("echo robot >> " + PATH_TO_SESSION_CONFIG).c_str());
  }

  topK_obj_identities = ortAgent_->p1_ort_session->infer(frame);
  ASSERT_EQ(topK_obj_identities[0], "Irish setter, red setter ");

  delete ortAgent_;
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
