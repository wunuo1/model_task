// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef HPL_HB_FFT_2D_H_
#define HPL_HB_FFT_2D_H_

#include "hb_hpl.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * @brief fft2d parameter
 * 
 * nx: select fft point x-axis 
 * ny: select fft point y-axis 
 * normalize: reserved field, normalize fft result or not, default is 0
 */
typedef struct {
  hbFFTPointSize nx;
  hbFFTPointSize ny;
  int8_t normalize;
} hbFFT2DParam;

/**
 * @brief FFT operator on 2d complex data for type hbHPLImaginaryData
 * 
 * @param[out] taskHandle task handle
 * @param[out] dst destination data
 * @param[in] src source data
 * @param[in] param op param
 * @return 0 if success
 */
int32_t hbFFT2D(hbUCPTaskHandle_t *taskHandle, hbHPLImaginaryData *dst,
                hbHPLImaginaryData const *src, hbFFT2DParam const *param);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // HPL_HB_FFT_2D_H_
