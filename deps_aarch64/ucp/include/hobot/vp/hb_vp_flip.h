// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef VP_HB_VP_FLIP_H_
#define VP_HB_VP_FLIP_H_

#include "hb_vp.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * @brief Flips a 2D array around vertical, horizontal
 * 
 * @param[out] taskHandle task handle
 * @param[out] dstImg destination image
 * @param[in] srcImg source image
 * @param[in] flipMode 0 means flipping around the x-axis and positive value (for example, 1) means flipping around y-axis
 * @return return 0 if success
 */
int32_t hbVPFlip(hbUCPTaskHandle_t *task, hbVPImage *dstImg,
                 hbVPImage const *srcImg, uint8_t flipMode);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // VP_HB_VP_FLIP_H_
