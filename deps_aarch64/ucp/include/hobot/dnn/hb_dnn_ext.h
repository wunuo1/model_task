// Copyright (c) 2020 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef DNN_HB_DNN_EXT_H_

#include "hb_dnn.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * Extra layout, supplement to hbDNNTensorLayout
 */
typedef enum {
  HB_DNN_LAYOUT_NHCW_NATIVE = 1,
  // TODO(@horizon.ai): complete layout, see hbrt_layout_type_t
} hbDNNExtraTensorLayout;

typedef enum {
  HB_DNN_INPUT_FROM_DDR = 0,
  HB_DNN_INPUT_FROM_RESIZER,
  HB_DNN_INPUT_FROM_PYRAMID,
} hbDNNInputSource;

typedef enum {
  HB_DNN_OUTPUT_OPERATOR_TYPE_UNKNOWN = 0,
  HB_DNN_OUTPUT_OPERATOR_TYPE_CONV = 1,
  HB_DNN_OUTPUT_OPERATOR_TYPE_DETECTION_POST_PROCESS = 2,
  HB_DNN_OUTPUT_OPERATOR_TYPE_RCNN_POST_PROCESS = 3,
  HB_DNN_OUTPUT_OPERATOR_TYPE_DETECTION_POST_PROCESS_STABLE_SORT = 4,
  HB_DNN_OUTPUT_OPERATOR_TYPE_CHANNEL_ARGMAX = 5,
  HB_DNN_OUTPUT_OPERATOR_TYPE_AUX_DPP_STABLE_SORT = 6,
  HB_DNN_OUTPUT_OPERATOR_TYPE_CHANNEL_ARGMAX_SPLIT = 7,
  HB_DNN_OUTPUT_OPERATOR_TYPE_FILTER = 8,
  HB_DNN_OUTPUT_OPERATOR_TYPE_CONV3D = 9,
  HB_DNN_OUTPUT_OPERATOR_TYPE_MAGIC = 10,
  HB_DNN_OUTPUT_OPERATOR_TYPE_RLE = 11,
  HB_DNN_OUTPUT_OPERATOR_TYPE_MODEL_INPUT_SCATTER = 12,
  HB_DNN_OUTPUT_OPERATOR_TYPE_MODEL_INPUT_GATHER = 13,
  HB_DNN_OUTPUT_OPERATOR_TYPE_MODEL_INPUT_GATHER_ELEMENTS = 14,
  HB_DNN_OUTPUT_OPERATOR_TYPE_MODEL_INPUT_TOP_K = 15
} hbDNNOutputOperatorType;

typedef enum {
  HB_DNN_DESC_TYPE_UNKNOWN = 0,
  HB_DNN_DESC_TYPE_STRING,
  HB_DNN_DESC_TYPE_BINARY,
} hbDNNDescType;

typedef hbDNNTensorShape hbDNNDimension;
/**
 * Get model input source
 * @param[out] inputSource
 * @param[in] dnnHandle
 * @param[in] inputIndex
 * @return  0 if success, return defined error code otherwise
 */
int32_t hbDNNGetInputSource(int32_t *inputSource, hbDNNHandle_t dnnHandle,
                            int32_t inputIndex);

/**
 * Get model input description
 * @param[out] desc
 * @param[out] size
 * @param[out] type
 * @param[in] dnnHandle
 * @param[in] inputIndex
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetInputDesc(char const **desc, uint32_t *size, int32_t *type,
                          hbDNNHandle_t dnnHandle, int32_t inputIndex);

/**
 * Get model output description
 * @param[out] desc
 * @param[out] size
 * @param[out] type
 * @param[in] dnnHandle
 * @param[in] outputIndex
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetOutputDesc(char const **desc, uint32_t *size, int32_t *type,
                           hbDNNHandle_t dnnHandle, int32_t outputIndex);

/**
 * Get model description
 * @param[out] desc
 * @param[out] size
 * @param[out] type
 * @param[in] dnnHandle
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetModelDesc(char const **desc, uint32_t *size, int32_t *type,
                          hbDNNHandle_t dnnHandle);

/**
 * Get model output operator type
 * @param[out] operatorType
 * @param[in] dnnHandle
 * @param[in] outputIndex
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetOutputOperatorType(int32_t *operatorType,
                                   hbDNNHandle_t dnnHandle,
                                   int32_t outputIndex);

/**
 * Get model estimate execute latency, it's real-time calculated based
 *  on historical statistics
 * @param[out] estimateLatency
 * @param[in] dnnHandle
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetEstimateLatency(int32_t *estimateLatency,
                                hbDNNHandle_t dnnHandle);

/**
 * Get estimate time for task
 * @param[out] estimateTime:
 * @param[in] taskHandle: pointer to the task
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetTaskEstimateTime(int32_t *estimateTime,
                                 hbDNNTaskHandle_t taskHandle);

/**
 * Estimated start time of upcoming tasks
 * @param[out] estimateTime :
 * @param[in] inferCtrlParam: task inferCtrlParam
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetTaskEstimateStartTime(int32_t *estimateTime,
                                      hbDNNInferCtrlParam *inferCtrlParam);

/**
 * Get the model tag
 * @param[out] tag: the model tag
 * @param[in] dnnHandle: pointer to the model
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNGetModelTag(char const **tag, hbDNNHandle_t dnnHandle);

/**
 * Add padding to data
 * @param output data with padding will be written to this address
 * @param input source data without padding
 * @param dimNum the dimension num of data
 * @param dim real dimension of input, uint32 array
 * @param stride the stride of output, int32 array, stride's uint is Byte
 * @param elementSize data element size
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNAddPaddingWithStride(void *output, void const *input,
                                  uint32_t dimNum, const uint32_t *dim,
                                  const int32_t *stride, uint32_t elementSize);
/**
 * Remove padding from data
 * @param output data without padding will be written to this address
 * @param input source data with padding
 * @param dimNum the dimension num of data
 * @param dim real dimension of output, uint32 array
 * @param stride the stride of input, int32 array, stride's uint is Byte
 * @param elementSize data element size
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNRemovePaddingWithStride(void *output, void const *input,
                                     uint32_t dimNum, const uint32_t *dim,
                                     const int32_t *stride,
                                     uint32_t elementSize);

/**
 * Add padding to data
 * @param output data with padding will be written to this address
 * @param outputShape shape of data with padding.  should be 4-element uint32 array
 * @param input source data without padding
 * @param inputShape shape of data without padding.  should be 4-element uint32 array
 * @param dataType element type of the data
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNAddPadding(void *output, hbDNNTensorShape outputShape,
                        void const *input, hbDNNTensorShape inputShape,
                        int32_t dataType);

/**
 * Remove padding from data
 * @param output data without padding will be written to this address
 * @param outputShape shape of data without padding.  should be 4-element uint32 array
 * @param input source data with padding
 * @param inputShape shape of data with padding.  should be 4-element uint32 array
 * @param dataType element type of the data
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNRemovePadding(void *output, hbDNNTensorShape outputShape,
                           void const *input, hbDNNTensorShape inputShape,
                           int32_t dataType);

#ifdef __cplusplus
}
#endif  // __cplusplus

#define DNN_HB_DNN_EXT_H_

#endif  // DNN_HB_DNN_EXT_H_
