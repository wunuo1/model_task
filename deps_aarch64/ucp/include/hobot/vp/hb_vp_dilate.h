// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef VP_HB_VP_DILATE_H_
#define VP_HB_VP_DILATE_H_

#include "hb_vp.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * pointLocX: reserved field
 * pointLocY: reserved field
 * iterations: reserved field
 * borderType: supports enum hbVPBorderType only
 * borderValue: reserved field
 */
typedef struct {
  int8_t pointLocX;
  int8_t pointLocY;
  int8_t iterations;
  int8_t borderType;
  uint8_t borderValue;
} hbVPDilateParam;

/**
 * @brief Dilates an image by using a specific structuring element
 * @param[out] taskHandle task handle
 * @param[out] dstImg destiny image
 * @param[in] srcImg source image
 * @param[in] dilateKernel kernel
 * @param[in] dilateParam param
 * @return 0 if success
 */
int32_t hbVPDilate(hbUCPTaskHandle_t *taskHandle, hbVPImage *dstImg,
                   hbVPImage const *srcImg,
                   hbVPFilterKernel const *dilateKernel,
                   hbVPDilateParam const *dilateParam);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // VP_HB_VP_DILATE_H_
