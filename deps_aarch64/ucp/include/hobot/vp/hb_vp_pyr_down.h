// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef VP_HB_VP_PYR_DOWN_H_
#define VP_HB_VP_PYR_DOWN_H_

#include "hb_vp.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define HB_VP_PYM_MAX_LAYER (5)

/**
 * levels: pyramid calculation levels
 * interpolation: select interpolation type
 */
typedef struct hbVPPymParam {
  int8_t levels;
  int8_t interpolation;
} hbVPPymParam;

/**
 * @brief Run pyramid
 * 
 * @param[out/in] taskHandle: return a pointer represent the task if success, given nullptr run task synchronous
 * @param[out/in] dstImgs: pointer to the output images vector, the size needs to be equal to the levels in the hbVPPymParam.
 * @param[in] srcImg: pointer to the input image
 * @param[in] pymCfg: pointer to the pyramid config param
 * @return return 0 if success
 */
int32_t hbVPPyrDown(hbUCPTaskHandle_t* taskHandle, hbVPImage* dstImgs,
                    hbVPImage const* srcImg, hbVPPymParam const* pymCfg);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // VP_HB_VP_PYR_DOWN_H_
