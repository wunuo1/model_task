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

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <utility>

#include "gflags/gflags.h"
#include "hlog/logging.h"
#include "hobot/dnn/hb_dnn.h"
#include "hobot/hb_ucp.h"
#include "hobot/hb_ucp_sys.h"
#define EMPTY ""

DEFINE_string(model_file, "/root/go2_parkour/memory_a_int16.hbm", "model file path");
DEFINE_int32(top_k, 5, "Top k classes, 5 by default");

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

int32_t read_image_2_tensor_as_nv12(hbDNNTensor *input_tensor) {
  // the struct of input shape is NHWC
    std::string observations_mem = "/root/go2_parkour/observations_mem.bin";
    std::ifstream file(observations_mem, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open the file!" << std::endl;
        return 1;
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
  
    // 假设文件中包含多个整数（int）
    std::vector<float> numbers(size / sizeof(float));
  
    // 读取文件中的数据到 vector
    file.read(reinterpret_cast<char*>(numbers.data()), size);
  
    // 检查读取是否成功
    if (!file) {
        std::cerr << "Failed to read the file!" << std::endl;
        return 1;
    }
  
    // for (auto num : numbers) {
    //     std::cout << num << " ";
    // }
  
    file.close();
    float *data_dst = reinterpret_cast<float *>(input_tensor[0].sysMem.virAddr);
    for(int i = 0; i < numbers.size(); i++){
      *data_dst++ = numbers[i];
    }
  
    hbUCPMemFlush(&input_tensor[0].sysMem, HB_SYS_MEM_CACHE_CLEAN);
  
    std::string hidden_state_mem = "/root/go2_parkour/hidden_state_mem.bin";
    std::ifstream file1(hidden_state_mem, std::ios::binary);
    if (!file1) {
        std::cerr << "Failed to open the file1!" << std::endl;
        return 1;
    }
    file1.seekg(0, std::ios::end);
    size_t size1 = file1.tellg();
    file1.seekg(0, std::ios::beg);
  
    // 假设文件中包含多个整数（int）
    std::vector<float> numbers1(size1 / sizeof(float));
  
    // 读取文件中的数据到 vector
    file1.read(reinterpret_cast<char*>(numbers1.data()), size1);
  
    // 检查读取是否成功
    if (!file1) {
        std::cerr << "Failed to read the file!" << std::endl;
        return 1;
    }
  
    // for (auto num : numbers) {
    //     std::cout << num << " ";
    // }
  
    file1.close();
    float *data_dst1 = reinterpret_cast<float *>(input_tensor[1].sysMem.virAddr);
    for(int i = 0; i < numbers1.size(); i++){
      *data_dst1++ = numbers1[i];
    }
  
    hbUCPMemFlush(&input_tensor[1].sysMem, HB_SYS_MEM_CACHE_CLEAN);
    return 0;
  }

int prepare_tensor(hbDNNTensor *input_tensor, hbDNNTensor *output_tensor,
                   hbDNNHandle_t dnn_handle);

/**
 * Step1: get model handle
 * Step2: prepare input and output tensor
 * Step3: set input data to input tensor
 * Step4: run inference
 * Step5: do postprocess with output data
 * Step6: release resources
 */
int main(int argc, char **argv) {
  // Parsing command line arguments
  gflags::SetUsageMessage(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::cout << gflags::GetArgv() << std::endl;

  // Init logging
  hobot::hlog::HobotLog::Instance()->SetLogLevel(
      "DNN_BASIC_SAMPLE", hobot::hlog::LogLevel::log_info);

  hbDNNPackedHandle_t packed_dnn_handle;
  hbDNNHandle_t dnn_handle;
  const char **model_name_list;
  auto modelFileName = FLAGS_model_file.c_str();
  int model_count = 0;
  // Step1: get model handle
  {
    HB_CHECK_SUCCESS(
        hbDNNInitializeFromFiles(&packed_dnn_handle, &modelFileName, 1),
        "hbDNNInitializeFromFiles failed");
    HB_CHECK_SUCCESS(hbDNNGetModelNameList(&model_name_list, &model_count,
                                           packed_dnn_handle),
                     "hbDNNGetModelNameList failed");
    HB_CHECK_SUCCESS(
        hbDNNGetModelHandle(&dnn_handle, packed_dnn_handle, model_name_list[0]),
        "hbDNNGetModelHandle failed");
  }

  std::vector<hbDNNTensor> input_tensors;
  std::vector<hbDNNTensor> output_tensors;
  int input_count = 0;
  int output_count = 0;
  // Step2: prepare input and output tensor
  {
    HB_CHECK_SUCCESS(hbDNNGetInputCount(&input_count, dnn_handle),
                     "hbDNNGetInputCount failed");
    HB_CHECK_SUCCESS(hbDNNGetOutputCount(&output_count, dnn_handle),
                     "hbDNNGetOutputCount failed");
    input_tensors.resize(input_count);
    output_tensors.resize(output_count);
    prepare_tensor(input_tensors.data(), output_tensors.data(), dnn_handle);
  }

  // Step3: set input data to input tensor
  {
    // read a single picture for input_tensor[0], for multi_input model, you
    // should set other input data according to model input properties.
    HB_CHECK_SUCCESS(
        read_image_2_tensor_as_nv12(input_tensors.data()),
        "read_image_2_tensor_as_nv12 failed");
    LOGI("read image to tensor as nv12 success");
  }

  hbUCPTaskHandle_t task_handle{nullptr};
  hbDNNTensor *output = output_tensors.data();
  // Step4: run inference
  {
    // generate task handle
    HB_CHECK_SUCCESS(
        hbDNNInferV2(&task_handle, output, input_tensors.data(), dnn_handle),
        "hbDNNInferV2 failed");

    // submit task
    hbUCPSchedParam ctrl_param;
    HB_UCP_INITIALIZE_SCHED_PARAM(&ctrl_param);
    ctrl_param.backend = HB_UCP_BPU_CORE_ANY;
    HB_CHECK_SUCCESS(hbUCPSubmitTask(task_handle, &ctrl_param),
                     "hbUCPSubmitTask failed");

    // wait task done
    HB_CHECK_SUCCESS(hbUCPWaitTaskDone(task_handle, 0),
                     "hbUCPWaitTaskDone failed");
  }

  // Step5: do postprocess with output data
  std::vector<Classification> top_k_cls;
  {
    // make sure CPU read data from DDR before using output tensor data
    for (int i = 0; i < output_count; i++) {
      hbUCPMemFlush(&output_tensors[i].sysMem, HB_SYS_MEM_CACHE_INVALIDATE);
    }
  }

  // Step6: release resources
  {
    // release task handle
    HB_CHECK_SUCCESS(hbUCPReleaseTask(task_handle), "hbUCPReleaseTask failed");
    // free input mem
    for (int i = 0; i < input_count; i++) {
      HB_CHECK_SUCCESS(hbUCPFree(&(input_tensors[i].sysMem)),
                       "hbUCPFree failed");
    }
    // free output mem
    for (int i = 0; i < output_count; i++) {
      HB_CHECK_SUCCESS(hbUCPFree(&(output_tensors[i].sysMem)),
                       "hbUCPFree failed");
    }
    // release model
    HB_CHECK_SUCCESS(hbDNNRelease(packed_dnn_handle), "hbDNNRelease failed");
  }

  return 0;
}

#define ALIGN(value, alignment) (((value) + ((alignment)-1)) & ~((alignment)-1))
#define ALIGN_32(value) ALIGN(value, 32)

int prepare_tensor(hbDNNTensor *input_tensor, hbDNNTensor *output_tensor,
                   hbDNNHandle_t dnn_handle) {
  int input_count = 0;
  int output_count = 0;
  hbDNNGetInputCount(&input_count, dnn_handle);
  hbDNNGetOutputCount(&output_count, dnn_handle);

  /** Tips:
   * For input memory size in most cases:
   * *   input_memSize = input[i].properties.alignedByteSize
   * but here for dynamic stride of y and uv，alignedByteSize is not fixed
   * For output memory size:
   * *   output_memSize = output[i].properties.alignedByteSize
   */
  hbDNNTensor *input = input_tensor;
  for (int i = 0; i < input_count; i++) {
    HB_CHECK_SUCCESS(
        hbDNNGetInputTensorProperties(&input[i].properties, dnn_handle, i),
        "hbDNNGetInputTensorProperties failed");

    /** Tips:
     * For input tensor, usually need to pad the input data according to stride obtained from properties.
     * but here for dynamic stride of y and uv，user needs to specify a value which should be 32 bytes aligned for the -1 position in stride.
     * */
    auto dim_len = input[i].properties.validShape.numDimensions;
    for (int32_t dim_i = dim_len - 1; dim_i >= 0; --dim_i) {
      if (input[i].properties.stride[dim_i] == -1) {
        auto cur_stride =
            input[i].properties.stride[dim_i + 1] *
            input[i].properties.validShape.dimensionSize[dim_i + 1];
        input[i].properties.stride[dim_i] = ALIGN_32(cur_stride);
      }
    }

    int input_memSize = input[i].properties.stride[0] *
                        input[i].properties.validShape.dimensionSize[0];
    HB_CHECK_SUCCESS(hbUCPMallocCached(&input[i].sysMem, input_memSize, 0),
                     "hbUCPMallocCached failed");

    // Show how to get input name
    const char *input_name;
    HB_CHECK_SUCCESS(hbDNNGetInputName(&input_name, dnn_handle, i),
                     "hbDNNGetInputName failed");
    LOGI("input[{}] name is {}", i, input_name);
  }

  hbDNNTensor *output = output_tensor;
  for (int i = 0; i < output_count; i++) {
    HB_CHECK_SUCCESS(
        hbDNNGetOutputTensorProperties(&output[i].properties, dnn_handle, i),
        "hbDNNGetOutputTensorProperties failed");
    int output_memSize = output[i].properties.alignedByteSize;
    HB_CHECK_SUCCESS(hbUCPMallocCached(&output[i].sysMem, output_memSize, 0),
                     "hbUCPMallocCached failed");

    // Show how to get output name
    const char *output_name;
    HB_CHECK_SUCCESS(hbDNNGetOutputName(&output_name, dnn_handle, i),
                     "hbDNNGetOutputName failed");
    LOGI("output[{}] name is {}", i, output_name);
  }
  return 0;
}

