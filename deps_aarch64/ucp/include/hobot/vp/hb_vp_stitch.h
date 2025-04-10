// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef VP_HB_VP_STITCH_H_
#define VP_HB_VP_STITCH_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "hb_vp.h"

/**
 * x: pos x(top left corner)
 * y: pos y(top left corner)
 */
typedef struct hbVPPoint {
  uint32_t x;
  uint32_t y;
} hbVPPoint;

typedef void *hbVPAlphaBlendLut;

/**
 * @brief Create alpha-blend lut
 * 
 * @param[out] alphaBlendLut: alpha-blend lut for stitch
 * @param[in] alphaDatas: alpha lut mems 
 * @param[in] alphaBlendRegions: location of needing alphablend in dst
 * @param[in] alphaBlendRegionNum: number of location of needing alpha-blend in dst
 * @return return 0 if success
 */
int32_t hbVPCreateAlphaBlendLut(hbVPAlphaBlendLut *alphaBlendLut,
                                uint8_t **alphaDatas,
                                hbVPRoi const *alphaBlendRegions,
                                int32_t alphaBlendRegionNum);

/**
 * @brief Run stitch
 * 
 * @param[out/in] taskHandle: return a pointer represent the task if success, given nullptr run task synchronous
 * @param[out/in] dstImg: pointer to the output image array (only supppot 1)
 * @param[in] srcImgs: pointer to the input image array (4 images supported at most)
 * @param[in] dstPoses: srcImgs's location in dstImg
 * @param[in] srcImgCount: src image count (srcImgs and dstPoses size is the same as srcImgCount)
 * @param[in] alphaBlendLut: alpha-blend lut for stitch
 * @return return 0 if success
 */
int32_t hbVPStitch(hbUCPTaskHandle_t *taskHandle, hbVPImage *dstImg,
                   hbVPImage const *srcImgs, hbVPPoint const *dstPoses,
                   int32_t srcImgCount, hbVPAlphaBlendLut alphaBlendLut);

/**
 * @brief Release alpha-blend lut
 * 
 * @param[in] alphaBlendLut: alpha-blend lut for stitch
 * @return return 0 if success
 */
int32_t hbVPReleaseAlphaBlendLut(hbVPAlphaBlendLut alphaBlendLut);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // VP_HB_VP_STITCH_H_
