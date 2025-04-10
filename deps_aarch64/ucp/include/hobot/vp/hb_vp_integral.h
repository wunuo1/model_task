// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef VP_HB_VP_INTEGRAL_H_
#define VP_HB_VP_INTEGRAL_H_

#include "hb_vp.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * @brief Calculates the integral of an image
 * 
 * @param[out] taskHandle task handle
 * @param[out] dstImg destination image
 * @param[in] srcImg source image
 * @return return 0 if success
 */
int32_t hbVPIntegral(hbUCPTaskHandle_t* taskHandle, hbVPImage* dstImg,
                     hbVPImage const* srcImg);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // VP_HB_VP_INTEGRAL_H_
