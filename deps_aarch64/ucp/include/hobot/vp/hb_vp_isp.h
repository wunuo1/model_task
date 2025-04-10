// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef VP_HB_VP_ISP_H_
#define VP_HB_VP_ISP_H_

#include "hb_vp.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define ISP_CONTEXT_SIZE (0x20000u)

typedef enum {
  HB_VP_BAYER_START_R = 0,
  HB_VP_BAYER_START_GR,
  HB_VP_BAYER_START_GB,
  HB_VP_BAYER_START_B,
  HB_VP_BAYER_START_C = 5
} hbVPBayerStart;

/**
 * bayerStart  : The start color of the bayer pattern
 * maxBitWidth : Corresponds to the data bit width after decompression of the sensor
 * datawidth   : Raw data bit width
 * sensorClk   : The number of exposure time lines per second corresponding to the sensor
 * context     : The pointer to ISP context memory, the memory should to be allocated using hbUCPMalloc or hbUCPMallocCached.
 */
typedef struct {
  hbVPBayerStart bayerStart;
  int8_t maxBitWidth;
  int8_t dataWidth;
  int32_t sensorClk;
  void *context;
} hbVPISPParam;

/**
 * @brief Call isp hardware to process raw image and get yuv image.
 * 
 * @param[out] taskHandle task handle
 * @param[out] dstImg destination image
 * @param[in] srcImg source image
 * @param[in] ispCfg isp config param
 * @return 0 if success
 */
int32_t hbVPISP(hbUCPTaskHandle_t *taskHandle, hbVPImage *dstImg,
                hbVPImage const *srcImg, hbVPISPParam const *ispCfg);

#ifdef __cplusplus
};
#endif  // __cplusplus

#endif  // VP_HB_VP_ISP_H_
