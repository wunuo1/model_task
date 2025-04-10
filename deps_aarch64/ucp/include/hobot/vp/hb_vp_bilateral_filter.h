// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef VP_HB_VP_BILATERAL_FILTER_H_
#define VP_HB_VP_BILATERAL_FILTER_H_

#include "hb_vp.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * sigmaColor : Filter sigma in the color space
 * sigmaSpace : Filter sigma in the coordinate space
 * kernelSize : Diameter of each pixel neighborhood that is used during filtering, only supports 5 or 9
 * borderType : supports enum hbVPBorderType only
 */
typedef struct {
  double sigmaColor;
  double sigmaSpace;
  int8_t kernelSize;
  int8_t borderType;
} hbVPBilateralFilterParam;

/**
 * @brief Applies the bilateral filter to an image
 * 
 * @param[out] taskHandle task handle
 * @param[out] dstImg destination image
 * @param[in] srcImg source image
 * @param[in] bilateralParam op param
 * @return 0 if success
 */
int32_t hbVPBilateralFilter(hbUCPTaskHandle_t *taskHandle, hbVPImage *dstImg,
                            hbVPImage const *srcImg,
                            hbVPBilateralFilterParam const *bilateralParam);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // VP_HB_VP_BILATERAL_FILTER_H_
