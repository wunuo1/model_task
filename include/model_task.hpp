// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

// This is a simple program that describes how to run resnet50 classification
// on an image and get its top k results by predict score.
// Should be noted: Only resnet50 is supported here.

#ifndef MODEL_TASK_H_
#define MODEL_TASK_H_

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <utility>
#include <vector>

#include "gflags/gflags.h"
#include "dnn/hb_dnn.h"
#include "dnn/hb_dnn_ext.h"
#include "dnn/plugin/hb_dnn_layer.h"
#include "dnn/plugin/hb_dnn_plugin.h"
#include "dnn/hb_sys.h"
#define EMPTY ""


#define HB_CHECK_SUCCESS(value, errmsg)             \
  do {                                              \
    /*value can be call of function*/               \
    auto ret_code = value;                          \
    if (ret_code != 0) {                            \
      return ret_code;                              \
    }                                               \
  } while (0);


class ModelTask{
public:
    ModelTask();
    ~ModelTask();
    bool ModelInit(std::string &model_path);
    std::vector<float> ModelInfer(std::vector<float> &input);
    // bool LoadFile(std::string &file_path);
    // bool LoadInput(std::vector<float> &input);
private:
  hbDNNTaskHandle_t packed_dnn_handle_;
  hbDNNHandle_t dnn_handle_;
  const char **model_name_list_;
  int model_count_ = 0;
  int input_count_ = 0;
  int output_count_ = 0;
};

#endif //MODEL_TASK_H_