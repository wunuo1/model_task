// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef VP_HB_VP_FILTER2D_H_
#define VP_HB_VP_FILTER2D_H_

#include "hb_vp.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * delta: reserved field
 * pointLocX: reserved field
 * pointLocY: reserved field
 * borderType: supports enum hbVPBorderType only
 */
typedef struct {
  double delta;
  int8_t pointLocX;
  int8_t pointLocY;
  int8_t borderType;
} hbVPFilter2DParam;

/**
 * @brief Convolves an image with the kernel
 * 
 * @param[out] taskHandle task handle
 * @param[out] dstImg destination image
 * @param[in] srcImg source image
 * @param[in] filterKernel kernel
 * @param[in] filter2DParam param
 * @return 0 if success
 */
int32_t hbVPFilter2D(hbUCPTaskHandle_t *taskHandle, hbVPImage *dstImg,
                     hbVPImage const *srcImg,
                     hbVPFilterKernel const *filterKernel,
                     hbVPFilter2DParam const *filter2DParam);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // VP_HB_VP_FILTER2D_H_
