// Copyright 2022 Advanced Remanufacturing and Technology Centre
// Copyright 2022 ROS-Industrial Consortium Asia Pacific Team
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

#include <jsoncpp/json/json.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include "gtest/gtest.h"
#include "bits/stdc++.h"
#include "epd_utils_lib/epd_container.hpp"

std::string PATH_TO_SESSION_CONFIG(PATH_TO_PACKAGE "/config/session_config.json");
std::string PATH_TO_USECASE_CONFIG(PATH_TO_PACKAGE "/config/usecase_config.json");
std::string PATH_TO_ONNX_MODEL(PATH_TO_PACKAGE "/data/model/squeezenet1.1-7.onnx");
std::string PATH_TO_LABEL_LIST(PATH_TO_PACKAGE "/data/label_list/imagenet_classes.txt");

TEST(EPD_TestSuite, Test_readSessionUseCaseConfigTextFile_EPDContainer)
{
  Json::StreamWriterBuilder builder;
  std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
  builder["commentStyle"] = "None";
  builder["indentation"] = "    ";

  // Reset session_config.json
  system(("rm -f " + PATH_TO_SESSION_CONFIG).c_str());
  system(("touch " + PATH_TO_SESSION_CONFIG).c_str());
  // Reset usecase_config.json
  system(("rm -f " + PATH_TO_USECASE_CONFIG).c_str());
  system(("touch " + PATH_TO_USECASE_CONFIG).c_str());

  Json::Value session_config_json;
  session_config_json["path_to_model"] = PATH_TO_ONNX_MODEL;
  session_config_json["path_to_label_list"] = PATH_TO_LABEL_LIST;
  session_config_json["visualizeFlag"] = "visualize";
  session_config_json["useCPU"] = "CPU";

  Json::Value usecase_config_json;
  usecase_config_json["usecase_mode"] = 0;

  std::ofstream outputFileStream1(PATH_TO_SESSION_CONFIG);
  writer->write(session_config_json, &outputFileStream1);
  outputFileStream1.close();

  std::ofstream outputFileStream2(PATH_TO_USECASE_CONFIG);
  writer->write(usecase_config_json, &outputFileStream2);
  outputFileStream2.close();

  EPD::EPDContainer * ortAgent_;

  ortAgent_ = new EPD::EPDContainer();

  EXPECT_EQ(
    ortAgent_->onnx_model_path,
    PATH_TO_PACKAGE "/data/model/squeezenet1.1-7.onnx");
  EXPECT_EQ(
    ortAgent_->class_label_path,
    PATH_TO_PACKAGE "/data/label_list/imagenet_classes.txt");
}

TEST(EPD_TestSuite, Test_setFrameDimension_EPDContainer)
{
  EPD::EPDContainer * ortAgent_;
  ortAgent_ = new EPD::EPDContainer();

  ortAgent_->setFrameDimension(1920, 1080);

  EXPECT_EQ(ortAgent_->getWidth(), 1920);
  EXPECT_EQ(ortAgent_->getHeight(), 1080);
}

TEST(EPD_TestSuite, Test_setInitBoolean_EPDContainer)
{
  EPD::EPDContainer * ortAgent_;
  ortAgent_ = new EPD::EPDContainer();

  EXPECT_EQ(ortAgent_->isInit(), false);
  ortAgent_->setInitBoolean(true);
  EXPECT_EQ(ortAgent_->isInit(), true);
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
