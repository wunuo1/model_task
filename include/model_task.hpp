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
#include "hlog/logging.h"
#include "hobot/dnn/hb_dnn.h"
#include "hobot/hb_ucp.h"
#include "hobot/hb_ucp_sys.h"
#include "opencv2/core/mat.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#define EMPTY ""


#define MOUDULE_NAME "DNN_BASIC_SAMPLE"
#define LOGD(err_msg, ...) HFLOGM_D(MOUDULE_NAME, err_msg, ##__VA_ARGS__)
#define LOGI(err_msg, ...) HFLOGM_I(MOUDULE_NAME, err_msg, ##__VA_ARGS__)
#define LOGE(err_msg, ...) HFLOGM_E(MOUDULE_NAME, err_msg, ##__VA_ARGS__)
#define LOGW(err_msg, ...) HFLOGM_W(MOUDULE_NAME, err_msg, ##__VA_ARGS__)

#define HB_CHECK_SUCCESS(value, errmsg)             \
  do {                                              \
    /*value can be call of function*/               \
    auto ret_code = value;                          \
    if (ret_code != 0) {                            \
      LOGE("{}, error code: {}", errmsg, ret_code); \
      return ret_code;                              \
    }                                               \
  } while (0);

typedef struct Classification {
  int id;
  float score;
  const char *class_name;

  Classification() : class_name(0), id(0), score(0.0) {}
  Classification(int id, float score, const char *class_name)
      : id(id), score(score), class_name(class_name) {}

  friend bool operator>(const Classification &lhs, const Classification &rhs) {
    return (lhs.score > rhs.score);
  }

  ~Classification() {}
} Classification;


class ModelTask{
public:
    ModelTask();
    ~ModelTask();
    bool ModelInit(std::string &model_path);
    std::vector<std::vector<float>> ModelInfer(std::vector<std::vector<float>> &input);
    // bool LoadFile(std::string &file_path);
    // bool LoadInput(std::vector<float> &input);
private:
  hbDNNPackedHandle_t packed_dnn_handle_;
  hbDNNHandle_t dnn_handle_;
  const char **model_name_list_;
  int model_count_ = 0;
  int input_count_ = 0;
  int output_count_ = 0;
};

#endif //MODEL_TASK_H_