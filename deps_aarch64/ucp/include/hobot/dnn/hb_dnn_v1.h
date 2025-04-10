// Copyright (c) 2020 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

// This header is provided for compatibility purposes only.
// Please consider using a newer version of the header file(hb_dnn.h) for best performance and functionality.
#ifndef DNN_HB_DNN_V1_H_
#define DNN_HB_DNN_V1_H_

#include "hobot/hb_sys.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct hbDNNTensor hbDNNTensor;
typedef struct hbDNNRoi hbDNNRoi;
typedef void *hbPackedDNNHandle_t;
typedef void *hbDNNHandle_t;

#define HB_DNN_INITIALIZE_INFER_CTRL_PARAM(param) \
  {                                               \
    (param)->bpuCoreId = HB_BPU_CORE_ANY;         \
    (param)->dspCoreId = HB_DSP_CORE_ANY;         \
    (param)->priority = HB_DNN_PRIORITY_LOWEST;   \
    (param)->more = false;                        \
    (param)->customId = 0;                        \
    (param)->reserved1 = 0;                       \
    (param)->reserved2 = 0;                       \
  }

typedef void *hbDNNTaskHandle_t;

typedef enum {
  HB_DNN_LAYOUT_NHWC = 0,
  HB_DNN_LAYOUT_NCHW = 2,
  HB_DNN_LAYOUT_NONE = 255,
} hbDNNTensorLayout;

typedef enum {
  HB_DNN_PRIORITY_LOWEST = 0,
  HB_DNN_PRIORITY_HIGHEST = 255,
  HB_DNN_PRIORITY_PREEMP = HB_DNN_PRIORITY_HIGHEST,
} hbDNNTaskPriority;

typedef struct {
  int32_t bpuCoreId;
  int32_t dspCoreId;
  int32_t priority;
  int32_t more;
  int64_t customId;
  int32_t reserved1;
  int32_t reserved2;
} hbDNNInferCtrlParam;

typedef void (*hbDNNTaskDoneCb)(hbDNNTaskHandle_t taskHandle, int32_t status,
                                void *userdata);

/**
 * DNN inference
 * @param[out] taskHandle: return a pointer represent the task if success,  otherwise nullptr
 * @param[out] output: pointer to the output tensor array, the size of array should be equal to $(`hbDNNGetOutputCount`)
 * @param[in] input: input tensor array, the size of array should be equal to  $(`hbDNNGetInputCount`)
 * @param[in] dnnHandle: pointer to the dnn handle
 * @param[in] inferCtrlParam: infer control parameters
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNInfer(hbDNNTaskHandle_t *taskHandle, hbDNNTensor **output,
                   hbDNNTensor const *input, hbDNNHandle_t dnnHandle,
                   hbDNNInferCtrlParam *inferCtrlParam);

/**
 * DNN inference with rois
 * @param[out] taskHandle: return a pointer represent the task if success,  otherwise nullptr
 * @param[in] input: input tensor array, the size of array should be equal to  $(`hbDNNGetInputCount`) * `batch`
 *      range of [idx*$(`hbDNNGetInputCount`), (idx+1)*$(`hbDNNGetInputCount`)) represents input tensors
 *      for idxth batch. 
 * @param[in] rois: Rois. the size of array should be equal to roiCount. 
 *      Assuming that the model has the input of n resizer input sources, range of [idx*n, (idx+1)*n) represents 
 *      rois for idxth batch.
 * @param[in] roiCount: roi count. If the model has n resizer input sources, then roiCount=`batch` * n.
 * @param[in] output: pointer to the output tensor array
 * @param[in] dnnHandle: pointer to the dnn handle
 * @param[in] inferCtrlParam: infer control parameters
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNRoiInfer(hbDNNTaskHandle_t *taskHandle, hbDNNTensor **output,
                      hbDNNTensor const *input, hbDNNRoi *rois,
                      int32_t roiCount, hbDNNHandle_t dnnHandle,
                      hbDNNInferCtrlParam *inferCtrlParam);

/**
 * DNN set task done callback
 * @param[in] taskHandle: pointer to the task
 * @param[in] cb: callback function
 * @param[in] userdata: userdata 
 * @return 0 if success, return defined error code otherwise
*/
int32_t hbDNNSetTaskDoneCb(hbDNNTaskHandle_t taskHandle, hbDNNTaskDoneCb cb,
                           void *userdata);

/**
 * Wait util task completed or timeout.
 * @param[in] taskHandle: pointer to the task
 * @param[in] timeout: timeout of milliseconds
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNWaitTaskDone(hbDNNTaskHandle_t taskHandle, int32_t timeout);

/**
 * Release a task and its related resources. If the task has not been executed then it will be canceled,
 * and if the task has not been finished then it will be stopped.
 * This interface will return immediately, and all operations will run in the background
 * @param[in] taskHandle: pointer to the task
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbDNNReleaseTask(hbDNNTaskHandle_t taskHandle);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // DNN_HB_DNN_V1_H_
