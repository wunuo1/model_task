/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Copyright (c) 2024，WuChao D-Robotics.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// 注意: 此程序在RDK板端端运行
// Attention: This program runs on RDK board.

// D-Robotics *.bin 模型路径
// Path of D-Robotics *.bin model.
#define MODEL_PATH "/root/adaptation_module11.bin"

// 推理使用的测试图片路径
// Path of the test image used for inference.
#define TESR_IMG_PATH "../../rm_test3.jpg"
// #define TESR_IMG_PATH "../../../../../resource/assets/kite.jpg"

// 前处理方式选择, 0:Resize, 1:LetterBox
// Preprocessing method selection, 0: Resize, 1: LetterBox
#define RESIZE_TYPE 0
#define LETTERBOX_TYPE 1
#define PREPROCESS_TYPE LETTERBOX_TYPE

// 推理结果保存路径
// Path where the inference result will be saved
#define IMG_SAVE_PATH "cpp_result.jpg"

// 模型的类别数量, 默认1
// Number of classes in the model, default is 80
#define CLASSES_NUM 18

// NMS的阈值, 默认0.45
// Non-Maximum Suppression (NMS) threshold, default is 0.45
#define NMS_THRESHOLD 0.45

// 分数阈值, 默认0.25
// Score threshold, default is 0.25
#define SCORE_THRESHOLD 0.25

// kpt 分数阈值, 默认0.25
// kpt Score threshold, default is 0.25
#define KPT_SCORE_THRESHOLD 0.5

// kpt 的个数
// num of kpt
#define KPT_NUM 4

// kpt 的编码，2:x,y, 3:x,y,vis
// encode type of kpt
#define KPT_ENCODE 3

// NMS选取的前K个框数, 默认300
// Number of top-K boxes selected by NMS, default is 300
#define NMS_TOP_K 300

// 控制回归部分离散化程度的超参数, 默认16
// A hyperparameter that controls the discretization level of the regression part, default is 16
#define REG 16

// 绘制标签的字体尺寸, 默认1.0
// Font size for drawing labels, default is 1.0.
#define FONT_SIZE 1.0

// 绘制标签的字体粗细, 默认 1.0
// Font thickness for drawing labels, default is 1.0.
#define FONT_THICKNESS 1.0

// 绘制矩形框的线宽, 默认2.0
// Line width for drawing bounding boxes, default is 2.0.
#define LINE_SIZE 2.0

// C/C++ Standard Librarys
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>

// RDK BPU libDNN API
#include "dnn/hb_dnn.h"
#include "dnn/hb_dnn_ext.h"
#include "dnn/plugin/hb_dnn_layer.h"
#include "dnn/plugin/hb_dnn_plugin.h"
#include "dnn/hb_sys.h"

#define RDK_CHECK_SUCCESS(value, errmsg)                        \
    do                                                          \
    {                                                           \
        /*value can be call of function*/                       \
        auto ret_code = value;                                  \
        if (ret_code != 0)                                      \
        {                                                       \
            std::cout << errmsg << ", error code:" << ret_code; \
            return ret_code;                                    \
        }                                                       \
    } while (0);

// COCO Names
std::vector<std::string> object_names = {
    "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light", "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard", "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple", "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch", "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone", "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear", "hair drier", "toothbrush"};

int main()
{

    // 1. 加载bin模型
    auto begin_time = std::chrono::system_clock::now();
    hbPackedDNNHandle_t packed_dnn_handle;
    const char *model_file_name = MODEL_PATH;
    RDK_CHECK_SUCCESS(
        hbDNNInitializeFromFiles(&packed_dnn_handle, &model_file_name, 1),
        "hbDNNInitializeFromFiles failed");

    // 2. 打印模型信息
    // 2. Show model message

    // 2.1 模型名称
    // 2.1 Model names
    const char **model_name_list;
    int model_count = 0;
    RDK_CHECK_SUCCESS(
        hbDNNGetModelNameList(&model_name_list, &model_count, packed_dnn_handle),
        "hbDNNGetModelNameList failed");

    // 如果这个bin模型有多个打包，则只使用第一个，一般只有一个
    // If this bin model has multiple packages, only the first one is used, usually there is only one.
    if (model_count > 1)
    {
        std::cout << "This model file have more than 1 model, only use model 0.";
    }
    const char *model_name = model_name_list[0];
    std::cout << "[model name]: " << model_name << std::endl;

    // 2.2 获得Packed模型的第一个模型的handle
    // 2.2 Get the handle of the first model in the packed model
    hbDNNHandle_t dnn_handle;
    RDK_CHECK_SUCCESS(
        hbDNNGetModelHandle(&dnn_handle, packed_dnn_handle, model_name),
        "hbDNNGetModelHandle failed");

    // 2.3 模型输入检查
    // 2.3 Model input check
    int32_t input_count = 0;
    RDK_CHECK_SUCCESS(
        hbDNNGetInputCount(&input_count, dnn_handle),
        "hbDNNGetInputCount failed");

    hbDNNTensorProperties input_properties;
    RDK_CHECK_SUCCESS(
        hbDNNGetInputTensorProperties(&input_properties, dnn_handle, 0),
        "hbDNNGetInputTensorProperties failed");

    // 2.3.1 D-Robotics YOLO11-Detect *.bin 模型应该为单输入
    // 2.3.1 D-Robotics YOLO11-Detect *.bin model should have only one input
    if (input_count > 1)
    {
        std::cout << "Your Model have more than 1 input, please check!" << std::endl;
        return -1;
    }


    // 2.3.4 D-Robotics YOLO11-Detect *.bin 模型输入Tensor数据的valid shape应为(1,3,H,W)
    // valid shape: (1,3,640,640)
    int32_t input_H, input_W;
    if (input_properties.validShape.numDimensions == 4)
    {
        input_H = input_properties.validShape.dimensionSize[2];
        input_W = input_properties.validShape.dimensionSize[3];
        std::cout << "input tensor valid shape: (" << input_properties.validShape.dimensionSize[0];
        std::cout << ", " << input_properties.validShape.dimensionSize[1];
        std::cout << ", " << input_H;
        std::cout << ", " << input_W << ")" << std::endl;
    }
    else
    {
        std::cout << "input tensor validShape.numDimensions is not 4 such as (1,3,640,640), please check!" << std::endl;
        return -1;
    }

    // 2.4 模型输出检查
    // 2.4 Model output check
    int32_t output_count = 0;
    RDK_CHECK_SUCCESS(
        hbDNNGetOutputCount(&output_count, dnn_handle),
        "hbDNNGetOutputCount failed");

    begin_time = std::chrono::system_clock::now();

    // 3.4 将准备好的输入数据放入hbDNNTensor
    // 3.4 Put input data into hbDNNTensor
    hbDNNTensor input;
    input.properties = input_properties;
    int input_memSize = input.properties.stride[0] * input.properties.validShape.dimensionSize[0];
    std::cout<<"memsize: "<<input_memSize<<std::endl;
    hbSysAllocCachedMem(&input.sysMem[0], input_memSize);

    std::string file_name = "/root/adaptation_input_050.bin";

    std::ifstream file(file_name, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open the file!" << std::endl;
        return 1;
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<float> numbers(size / sizeof(float));
    file.read(reinterpret_cast<char*>(numbers.data()), size);

    if (!file) {
        std::cerr << "Failed to read the file!" << std::endl;
        return 1;
    }
    file.close();

    float *data_dst = reinterpret_cast<float *>(input.sysMem[0].virAddr);
    for(int i = 0; i < numbers.size(); i++){
      *data_dst++ = numbers[i];
    }

    hbSysFlushMem(&input.sysMem[0], HB_SYS_MEM_CACHE_CLEAN);

    // 4. 准备模型输出数据的空间
    // 4. Prepare the space for model output data
    hbDNNTensor *output = new hbDNNTensor;

    hbDNNTensorProperties &output_properties = output->properties;
    hbDNNGetOutputTensorProperties(&output_properties, dnn_handle, 0);
    int out_aligned_size = output_properties.alignedByteSize;
    hbSysMem &mem = output->sysMem[0];
    std::cout<<"alignedByteSize: "<<out_aligned_size<<std::endl;
    hbSysAllocCachedMem(&mem, out_aligned_size);

    // 5. 推理模型
    // 5. Inference
    hbDNNTaskHandle_t task_handle = nullptr;
    hbDNNInferCtrlParam infer_ctrl_param;
    HB_DNN_INITIALIZE_INFER_CTRL_PARAM(&infer_ctrl_param);
    hbDNNInfer(&task_handle, &output, &input, dnn_handle, &infer_ctrl_param);

    hbDNNWaitTaskDone(task_handle, 0);
    hbSysFlushMem(&(output->sysMem[0]), HB_SYS_MEM_CACHE_INVALIDATE);

    float* data = reinterpret_cast<float *>(output->sysMem[0].virAddr);
    std::cout<<"output_size: "<<output->properties.validShape.dimensionSize[1]<<std::endl;
    for (auto i = 0; i < output->properties.validShape.dimensionSize[1]; i++) {
      float score = data[i];
      std::cout<<score<<" ";
    }
    // // 10. 释放任务
    // // 10. Release task
    // hbDNNReleaseTask(task_handle);

    // // 11. 释放内存
    // // 11. Release memory
    // hbSysFreeMem(&(input.sysMem[0]));
    // hbSysFreeMem(&(output->sysMem[0]));

    // // 12. 释放模型
    // // 12. Release model
    // hbDNNRelease(packed_dnn_handle);

    return 0;
}