// Copyright (c) 2020 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef DNN_HB_DNN_H_
#define DNN_HB_DNN_H_

#include "hb_dnn_v1.h"
#include "hobot/hb_ucp.h"
#include "hobot/hb_ucp_sys.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define HB_DNN_TENSOR_MAX_DIMENSIONS (8)

typedef void *hbDNNPackedHandle_t;
typedef void *hbDNNHandle_t;

typedef enum {
  HB_DNN_IMG_TYPE_Y,
  HB_DNN_IMG_TYPE_NV12,
  HB_DNN_IMG_TYPE_NV12_SEPARATE,
  HB_DNN_IMG_TYPE_YUV444,
  HB_DNN_IMG_TYPE_RGB,
  HB_DNN_IMG_TYPE_BGR,
  HB_DNN_TENSOR_TYPE_S4,
  HB_DNN_TENSOR_TYPE_U4,
  HB_DNN_TENSOR_TYPE_S8,
  HB_DNN_TENSOR_TYPE_U8,
  HB_DNN_TENSOR_TYPE_F16,
  HB_DNN_TENSOR_TYPE_S16,
  HB_DNN_TENSOR_TYPE_U16,
  HB_DNN_TENSOR_TYPE_F32,
  HB_DNN_TENSOR_TYPE_S32,
  HB_DNN_TENSOR_TYPE_U32,
  HB_DNN_TENSOR_TYPE_F64,
  HB_DNN_TENSOR_TYPE_S64,
  HB_DNN_TENSOR_TYPE_U64,
  HB_DNN_TENSOR_TYPE_BOOL8,
  HB_DNN_TENSOR_TYPE_MAX
} hbDNNDataType;

typedef struct hbDNNTensorShape {
  int32_t dimensionSize[HB_DNN_TENSOR_MAX_DIMENSIONS];
  int32_t numDimensions;
} hbDNNTensorShape;

typedef struct hbDNNQuantiShift {
  int32_t shiftLen;
  uint8_t *shiftData;
} hbDNNQuantiShift;

/**
 * Quantize/Dequantize by scale
 * For Dequantize:
 * if zeroPointLen = 0 f(x_i) = x_i * scaleData[i]
 * if zeroPointLen > 0 f(x_i) = (x_i - zeroPointData[i]) * scaleData[i]
 * 
 * For Quantize:
 * if zeroPointLen = 0 f(x_i) = g(x_i / scaleData[i])
 * if zeroPointLen > 0 f(x_i) = g(x_i / scaleData[i] + zeroPointData[i])
 * which g(x) = clip(nearbyint(x)), use fesetround(FE_TONEAREST), U8: 0 <= g(x) <= 255, S8: -128 <= g(x) <= 127
 */
typedef struct hbDNNQuantiScale {
  int32_t scaleLen;
  float *scaleData;
  int32_t zeroPointLen;
  int32_t *zeroPointData;
} hbDNNQuantiScale;

typedef enum {
  NONE,  // no quantization
  SHIFT,
  SCALE
} hbDNNQuantiType;

typedef struct hbDNNTensorProperties {
  hbDNNTensorShape validShape;
  hbDNNTensorShape alignedShape;  // Deprecated. Please use stride instead.
  int32_t tensorLayout;           // Deprecated.
  int32_t tensorType;
  hbDNNQuantiShift shift;
  hbDNNQuantiScale scale;
  hbDNNQuantiType quantiType;
  int32_t quantizeAxis;
  int32_t alignedByteSize;
  int32_t stride[HB_DNN_TENSOR_MAX_DIMENSIONS];
} hbDNNTensorProperties;

typedef struct hbDNNTensor {
  hbUCPSysMem sysMem[4];
  hbDNNTensorProperties properties;
} hbDNNTensor;

typedef struct hbDNNRoi {
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
} hbDNNRoi;

/**
 * Get DNN version
 * @return DNN version info
 */
char const *hbDNNGetVersion();

/**
 * Creates and initializes Horizon DNN Networks from file list
 * @param[out] dnnPackedHandle
 * @param[in] modelFileNames
 * @param[in] modelFileCount
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNInitializeFromFiles(hbDNNPackedHandle_t *dnnPackedHandle,
                                 char const **modelFileNames,
                                 int32_t modelFileCount);

/**
 * Creates and initializes Horizon DNN Networks from memory
 * @param[out] dnnPackedHandle
 * @param[in] modelData
 * @param[in] modelDataLengths
 * @param[in] modelDataCount
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNInitializeFromDDR(hbDNNPackedHandle_t *dnnPackedHandle,
                               const void **modelData,
                               int32_t *modelDataLengths,
                               int32_t modelDataCount);

/**
 * Release DNN Networks in a given packed handle
 * @param[in] dnnPackedHandle
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNRelease(hbDNNPackedHandle_t dnnPackedHandle);

/**
 * Get model names from given packed handle
 * @param[out] modelNameList
 * @param[out] modelNameCount
 * @param[in] dnnPackedHandle
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetModelNameList(char const ***modelNameList,
                              int32_t *modelNameCount,
                              hbDNNPackedHandle_t dnnPackedHandle);

/**
 * Get DNN Network handle from packed Handle with given model name
 * @param[out] dnnHandle
 * @param[in] dnnPackedHandle
 * @param[in] modelName
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetModelHandle(hbDNNHandle_t *dnnHandle,
                            hbDNNPackedHandle_t dnnPackedHandle,
                            char const *modelName);

/**
 * Get input count
 * @param[out] inputCount
 * @param[in] dnnHandle
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetInputCount(int32_t *inputCount, hbDNNHandle_t dnnHandle);

/**
 * Get model input name
 * @param[out] name
 * @param[in] dnnHandle
 * @param[in] inputIndex
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetInputName(char const **name, hbDNNHandle_t dnnHandle,
                          int32_t inputIndex);

/**
 * Get input tensor properties
 * @param[out] properties
 * @param[in] dnnHandle
 * @param[in] inputIndex
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetInputTensorProperties(hbDNNTensorProperties *properties,
                                      hbDNNHandle_t dnnHandle,
                                      int32_t inputIndex);

/**
 * Get output count
 * @param[out] outputCount
 * @param[in] dnnHandle
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetOutputCount(int32_t *outputCount, hbDNNHandle_t dnnHandle);

/**
 * Get model output name
 * @param[out] name
 * @param[in] dnnHandle
 * @param[in] outputIndex
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetOutputName(char const **name, hbDNNHandle_t dnnHandle,
                           int32_t outputIndex);

/**
 * Get output tensor properties
 * @param[out] properties
 * @param[in] dnnHandle
 * @param[in] outputIndex
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetOutputTensorProperties(hbDNNTensorProperties *properties,
                                       hbDNNHandle_t dnnHandle,
                                       int32_t outputIndex);

/**
 * DNN inference
 * @param[in/out] taskHandle: 
 * case1: given *taskHandle is nullptr, create new task handle
 * case2: given *taskHandle is not nullptr, attach task to task handle, which represents multi model task
 * case3: given taskHandle is nullptr to run in sync mode with default ctrl param
 * @param[out] output: pointer to the output tensor array, the size of array should be equal to $(`hbDNNGetOutputCount`)
 * @param[in] input: input tensor array, the size of array should be equal to  $(`hbDNNGetInputCount`)
 * @param[in] dnnHandle: pointer to the dnn handle which represents model handle
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNInferV2(hbUCPTaskHandle_t *taskHandle, hbDNNTensor *output,
                     hbDNNTensor const *input, hbDNNHandle_t dnnHandle);

/**
 * DNN inference with rois
 * @param[in/out] taskHandle: 
 * case1: given *taskHandle is nullptr, create new task handle
 * case2: given *taskHandle is not nullptr, attach task to task handle, which represents multi model task
 * case3: given taskHandle is nullptr to run in sync mode with default ctrl param
 * @param[in] output: pointer to the output tensor array
 * @param[in] input: input tensor array, the size of array should be equal to  $(`hbDNNGetInputCount`) * `batch`
 *      range of [idx*$(`hbDNNGetInputCount`), (idx+1)*$(`hbDNNGetInputCount`)) represents input tensors
 *      for idxth batch. 
 * @param[in] rois: Rois. the size of array should be equal to roiCount. 
 *      Assuming that the model has the input of n resizer input sources, range of [idx*n, (idx+1)*n) represents 
 *      rois for idxth batch.
 * @param[in] roiCount: roi count. If the model has n resizer input sources, then roiCount=`batch` * n.
 * @param[in] dnnHandle: pointer to the dnn handle which represents model handle
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNRoiInferV2(hbUCPTaskHandle_t *taskHandle, hbDNNTensor *output,
                        hbDNNTensor const *input, hbDNNRoi *rois,
                        int32_t roiCount, hbDNNHandle_t dnnHandle);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // DNN_HB_DNN_H_
