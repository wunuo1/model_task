// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef VP_HB_VP_CORNER_HARRIS_H_
#define VP_HB_VP_CORNER_HARRIS_H_

#include "hb_vp.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * borderType: supports enum hbVPBorderType
 * kernelSize: supports 3, 5 and 7, means kernel size is 3x3, 5x5 and 7x7
 * blockSize: it should be odd value and in the range of [3, 27]
 * sensitivity: recommends the range of [0.04, 0.06]
 */
typedef struct {
  int8_t borderType;
  int8_t kernelSize;
  uint32_t blockSize;
  float sensitivity;
} hbVPCornerHarrisParam;

/**
 * @brief Harris corner detector
 * 
 * @param[out] taskHandle task handle
 * @param[out] dstImg destination image
 * @param[in] srcImg source image
 * @param[in] cornerHarrisParam op param
 * @return 0 if success
 */
int32_t hbVPCornerHarris(hbUCPTaskHandle_t *taskHandle, hbVPImage *dstImg,
                         hbVPImage const *srcImg,
                         hbVPCornerHarrisParam const *cornerHarrisParam);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // VP_HB_VP_CORNER_HARRIS_H_
