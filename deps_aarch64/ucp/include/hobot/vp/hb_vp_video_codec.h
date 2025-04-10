// Copyright (c) [2021-2023] [Horizon Robotics][Horizon Bole].
//
// You can use this software according to the terms and conditions of
// the Apache v2.0.
// You may obtain a copy of Apache v2.0. at:
//
//     http: //www.apache.org/licenses/LICENSE-2.0
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See Apache v2.0 for more details.

#ifndef VP_HB_VP_VIDEO_CODEC_H_
#define VP_HB_VP_VIDEO_CODEC_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "hb_vp.h"

/**
 * HB_VP_VIDEO_TYPE_H264: video type is H264
 * HB_VP_VIDEO_TYPE_H265: video type is H265
 */
typedef enum { HB_VP_VIDEO_TYPE_H264, HB_VP_VIDEO_TYPE_H265 } hbVPVideoType;

/**
 * the rate control mode
*/
typedef enum {
  HB_VP_VIDEO_RC_MODE_H264_CBR,
  HB_VP_VIDEO_RC_MODE_H264_VBR,
  HB_VP_VIDEO_RC_MODE_H264_AVBR,
  HB_VP_VIDEO_RC_MODE_H264_FIXQP,
  HB_VP_VIDEO_RC_MODE_H264_QPMAP,
  HB_VP_VIDEO_RC_MODE_H265_CBR,
  HB_VP_VIDEO_RC_MODE_H265_VBR,
  HB_VP_VIDEO_RC_MODE_H265_AVBR,
  HB_VP_VIDEO_RC_MODE_H265_FIXQP,
  HB_VP_VIDEO_RC_MODE_H265_QPMAP
} hbVPVideoRcMode;

/**
 * H264 profile
*/
typedef enum {
  HB_VP_H264_PROFILE_UNSPECIFIED,
  HB_VP_H264_PROFILE_BP,
  HB_VP_H264_PROFILE_MP,
  HB_VP_H264_PROFILE_EXTENDED,
  HB_VP_H264_PROFILE_HP,
  HB_VP_H264_PROFILE_HIGH10,
  HB_VP_H264_PROFILE_HIGH422,
  HB_VP_H264_PROFILE_HIGH444
} hbVPH264Profile;

/**
 * H264 levle
*/
typedef enum {
  HB_VP_H264_LEVEL_UNSPECIFIED,
  HB_VP_H264_LEVEL1b = 9,
  HB_VP_H264_LEVEL1 = 10,
  HB_VP_H264_LEVEL1_1 = 11,
  HB_VP_H264_LEVEL1_2 = 12,
  HB_VP_H264_LEVEL1_3 = 13,
  HB_VP_H264_LEVEL2 = 20,
  HB_VP_H264_LEVEL2_1 = 21,
  HB_VP_H264_LEVEL2_2 = 22,
  HB_VP_H264_LEVEL3 = 30,
  HB_VP_H264_LEVEL3_1 = 31,
  HB_VP_H264_LEVEL3_2 = 32,
  HB_VP_H264_LEVEL4 = 40,
  HB_VP_H264_LEVEL4_1 = 41,
  HB_VP_H264_LEVEL4_2 = 42,
  HB_VP_H264_LEVEL5 = 50,
  HB_VP_H264_LEVEL5_1 = 51,
  HB_VP_H264_LEVEL5_2 = 52
} hbVPH264Level;

/**
 * H265 levle
*/
typedef enum {
  HB_VP_H265_LEVEL_UNSPECIFIED,
  HB_VP_H265_LEVEL1 = 30,
  HB_VP_H265_LEVEL2 = 60,
  HB_VP_H265_LEVEL2_1 = 63,
  HB_VP_H265_LEVEL3 = 90,
  HB_VP_H265_LEVEL3_1 = 93,
  HB_VP_H265_LEVEL4 = 120,
  HB_VP_H265_LEVEL4_1 = 123,
  HB_VP_H265_LEVEL5 = 150,
  HB_VP_H265_LEVEL5_1 = 153
} hbVPH265Level;

/**
 * the parameter of H264 Constant Bit Rate(CBR).
 * intraPeriod: I frame interval, values[0, 2047] and the default value is 28.
 * intraQp: the quantization parameter of intra picture,
 *          values[0, 51] and the default value is 30
 * bitRate: the target average bitrate of the encoded data in kbps,
 *          values[0, 700000]kbps and the default value is 0
 * frameRate: the target frame rate of the encoded data in fps, 
 *            values[1, 240] and the default value is 30
 * initialRcQp: specifies the initial QP by user,
 *              values[0, 63] and the default value is 63
 * vbvBufferSize: specifies the size of the VBV buffer in msec (10 ~ 3000), 
 *                values(10, 3000)ms and the default value is 10.
 *                For example, 3000 should be set for 3 seconds. 
 *                This value is valid when RateControl is 1. 
 *                vbvBufferSize in bits is  bitRate * vbv_buffer_size / 1000.
 *                vbvBufferSize has relevance to picture quality and bitrate accuracy.
 *                As vbvBufferSize is shorter, encoder can reach target bitrate accurately with worse quality. 
 *                On the other hand, as vbvBufferSize is longer, it can achieve rather better quality under rate control.  
 * mbLevelRcEnable: enable h264 the rate control in frame level and 
 *                  mb level, the default value is 0
 * minQpI: a minimum QP of I picture for rate control,
 *          values[0, 51] and the default value is 8
 * maxQpI: a maximum QP of I picture for rate control,
 *          values[0, 51] and the default value is 51
 * minQpP: a minimum QP of P picture for rate control,
 *          values[0, 51] and the default value is 8
 * maxQpP: a maximum QP of P picture for rate control,
 *          values[0, 51] and the default value is 51
 * minQpB: a minimum QP of B picture for rate control,
 *          values[0, 51] and the default value is 8
 * maxQpB: a maximum QP of B picture for rate control,
 *          values[0, 51] and the default value is 51
 * hvsQpEnable: enable CU QP derivation based on CU variance, 
 *              the default value is 1
 * hvsQpScale: QP scaling factor for subCTU QP adjustment when hvsQpEnable is 1,
 *             values[0, 4] and the default value is 2
 * hvsMaxDeltaQp: specifies maximum delta QP of HVS QP,
 *                values[0, 12] and the default value is 10
 * qpMapEnable: enable QP map, the default value is 0
*/
typedef struct {
  uint32_t intraPeriod;
  uint32_t intraQp;
  uint32_t bitRate;
  uint32_t frameRate;
  uint32_t initialRcQp;
  int32_t vbvBufferSize;
  uint32_t mbLevelRcEnable;
  uint32_t minQpI;
  uint32_t maxQpI;
  uint32_t minQpP;
  uint32_t maxQpP;
  uint32_t minQpB;
  uint32_t maxQpB;
  uint32_t hvsQpEnable;
  int32_t hvsQpScale;
  uint32_t hvsMaxDeltaQp;
  uint8_t qpMapEnable;
} hbVPVideoH264Cbr;

/**
 * the parameter of H264 Variable Bit Rate(VBR).
 * intraPeriod: I frame interval, values[0, 2047] and the default value is 28.
 * intraQp: the quantization parameter of intra picture,
 *          values[0, 51] and the default value is 30
 * frameRate: the target frame rate of the encoded data in fps, 
 *            values[1, 240] and the default value is 30
 * qpMapEnable: enable QP map, the default value is 0
*/
typedef struct {
  uint32_t intraPeriod;
  uint32_t intraQp;
  uint32_t frameRate;
  uint8_t qpMapEnable;
} hbVPVideoH264Vbr;

/**
 * the parameter of H264 Fix Qp.
 * intraPeriod: I frame interval, values[0, 2047] and the default value is 28.
 * frameRate: the target frame rate of the encoded data in fps, 
 *            values[1, 240] and the default value is 30
 * qpI: a picture quantization parameter for I picture,
 *      values[0, 51] and the default value is 0
 * qpP: a picture quantization parameter for P picture,
 *      values[0, 51] and the default value is 0
 * qpB: a picture quantization parameter for B picture,
 *      values[0, 51] and the default value is 0
*/
typedef struct {
  uint32_t intraPeriod;
  uint32_t frameRate;
  uint32_t qpI;
  uint32_t qpP;
  uint32_t qpB;
} hbVPVideoH264FixQp;

/**
 * the parameter of H264 Qp Map.
 * intraPeriod: I frame interval, values[0, 2047] and the default value is 28.
 * frameRate: the target frame rate of the encoded data in fps, 
 *            values[1, 240] and the default value is 30
 * qpMapArrayCount: specify the qp map number, values[0, 64] and the default value is 0
 * qpMapArray: specify the qp map, QP values[0, 51] and the default value is nullptr
*/
typedef struct {
  uint32_t intraPeriod;
  uint32_t frameRate;
  uint32_t qpMapArrayCount;
  uint8_t *qpMapArray;
} hbVPVideoH264QpMap;

/**
 * the parameter of H265 Constant Bit Rate(CBR).
 * intraPeriod: I frame interval, values[0, 2047] and the default value is 28.
 * intraQp: the quantization parameter of intra picture,
 *          values[0, 51] and the default value is 30
 * bitRate: the target average bitrate of the encoded data in kbps,
 *          values[0, 700000]kbps and the default value is 0
 * frameRate: the target frame rate of the encoded data in fps, 
 *            values[1, 240] and the default value is 30
 * initialRcQp: specifies the initial QP by user,
 *              values[0, 63] and the default value is 63
 * vbvBufferSize: specifies the size of the VBV buffer in msec (10 ~ 3000), 
 *                values(10, 3000)ms and the default value is 10.
 *                For example, 3000 should be set for 3 seconds. 
 *                This value is valid when RateControl is 1. 
 *                vbvBufferSize in bits is  bitRate * vbv_buffer_size / 1000.
 *                vbvBufferSize has relevance to picture quality and bitrate accuracy.
 *                As vbvBufferSize is shorter, encoder can reach target bitrate accurately with worse quality. 
 *                On the other hand, as vbvBufferSize is longer, it can achieve rather better quality under rate control.  
 * ctuLevelRcEnable: enable h265 the rate control in frame level and 
 *                   ctu level, the default value is 0
 * minQpI: a minimum QP of I picture for rate control,
 *          values[0, 51] and the default value is 8
 * maxQpI: a maximum QP of I picture for rate control,
 *          values[0, 51] and the default value is 51
 * minQpP: a minimum QP of P picture for rate control,
 *          values[0, 51] and the default value is 8
 * maxQpP: a maximum QP of P picture for rate control,
 *          values[0, 51] and the default value is 51
 * minQpB: a minimum QP of B picture for rate control,
 *          values[0, 51] and the default value is 8
 * maxQpB: a maximum QP of B picture for rate control,
 *          values[0, 51] and the default value is 51
 * hvsQpEnable: enable CU QP derivation based on CU variance, 
 *              the default value is 1
 * hvsQpScale: QP scaling factor for subCTU QP adjustment when hvsQpEnable is 1,
 *             values[0, 4] and the default value is 2
 * hvsMaxDeltaQp: specifies maximum delta QP of HVS QP,
 *                values[0, 12] and the default value is 10
 * qpMapEnable: enable QP map, the default value is 0
*/
typedef struct {
  uint32_t intraPeriod;
  uint32_t intraQp;
  uint32_t bitRate;
  uint32_t frameRate;
  uint32_t initialRcQp;
  int32_t vbvBufferSize;
  uint32_t ctuLevelRcEnable;
  uint32_t minQpI;
  uint32_t maxQpI;
  uint32_t minQpP;
  uint32_t maxQpP;
  uint32_t minQpB;
  uint32_t maxQpB;
  uint32_t hvsQpEnable;
  int32_t hvsQpScale;
  uint32_t hvsMaxDeltaQp;
  uint8_t qpMapEnable;
} hbVPVideoH265Cbr;

typedef hbVPVideoH264Cbr hbVPVideoH264Avbr;
typedef hbVPVideoH264Vbr hbVPVideoH265Vbr;
typedef hbVPVideoH264QpMap hbVPVideoH265QpMap;
typedef hbVPVideoH264FixQp hbVPVideoH265FixQp;
typedef hbVPVideoH265Cbr hbVPVideoH265Avbr;

/**
 * the parameter of rate control.
 * mode: rate control mode
*/
typedef struct {
  hbVPVideoRcMode mode;
  union {
    hbVPVideoH264Cbr h264Cbr;
    hbVPVideoH264Vbr h264Vbr;
    hbVPVideoH264Avbr h264Avbr;
    hbVPVideoH264QpMap h264QpMap;
    hbVPVideoH264FixQp h264FixQp;

    hbVPVideoH265Cbr h265Cbr;
    hbVPVideoH265Vbr h265Vbr;
    hbVPVideoH265Avbr h265Avbr;
    hbVPVideoH265QpMap h265QpMap;
    hbVPVideoH265FixQp h265FixQp;
  };
} hbVPVideoRcParam;

/**
 * the parameters of GOP structure.
 * decodingRefreshType: the type of I picture to be inserted at every intraPeriod, the default value is 2                
 * gopPresetIdx: a GOP structure preset option, the default value is 2
*/
typedef struct {
  int32_t decodingRefreshType;
  uint32_t gopPresetIdx;
} hbVPVideoGopParam;

/**
 * the encoding parameters of H264 video codec.
 * h264Profile: user add profile information to SPS by setting the profile register.
 *              However, if set 0 or have done nothing to the register, 
 *              VPU automatically encodes a profile by using the bit depth of source picture.
 * h264Level: H.264/AVC level_idc in SPS. If set 0, FW calculates level instead of host setting value.
*/
typedef struct {
  hbVPH264Profile h264Profile;
  hbVPH264Level h264Level;
} hbVPH264EncConfig;

/**
 * the encoding parameters of H265 video codec.
 * h265Tier: VPU is able to support up to main and high tier. The tier 
 *           information can also be given to SPS by setting the tier 
 *           register. If set 'h265_level=0', FW calculate level & tier 
 *           instead of host  setting value. The default value is 0.
 *            0: Main tier, 1: High tier
 * transformSkipEnable: enable transform skip for intra CU, the default value is 1
 *                      0 : Disable intra transform skip.
 *                     1 : Enable intra transform skip for chroma of CU_8x8 and luma/chroma of IntraNxN.
 * losslessMode: VPU can encode in lossless mode where no distortion is 
 *               allowed in reconstructed frames, the default value is 0
 * tmvpEnable: enable temporal motion vector prediction for H265 data,  
 *             the default value is 1
 * wppEnable: enable wave-front parallel processing for H265 data, 
 *            the default value is 0
 * mainStillPictureProfileEnable: User add profile information to SPS by 
 *                                setting the profile register. However, 
 *                                if set 0 or have done nothing to the 
 *                                register, VPU automatically encodes a 
 *                                profile by using the bit depth of source picture.
 *                                The deault value is 0.
 * h265Level: H.265/HEVC general_level_idc. If set 0, FW calculates level 
 *            instead of host setting value.
*/
typedef struct {
  int32_t h265Tier;
  uint32_t transformSkipEnable;
  uint32_t losslessMode;
  uint32_t tmvpEnable;
  uint32_t wppEnable;
  uint8_t mainStillPictureProfileEnable;
  hbVPH265Level h265Level;
} hbVPH265EncConfig;

/**
 * the video codec encoding parameters.
 * pixelFormat: the format of input video.
 *              support HB_VP_IMAGE_FORMAT_NV12 and HB_VP_IMAGE_FORMAT_YUV420. 
 * width: the width of input video. 
 *        H265: values[256, 8192], aligned with 8.
 *        H264: values[128, 8192], aligned with 8.
 * height: the height of input video.
 *         H265: values[256, 8192], aligned with 8.
 *         H264: values[128, 8192], aligned with 8.
 * outBufCount: the count of output buffers.
 *               values[1, 65536], default 5.
 * videoType: the encoding types of video
 * rcParams: the rate control parameters.
 * gopParams: the gop parameters
 * h264EncConfig：h265 encoding config
 * h265EncConfig: h264 encoding config
*/
typedef struct {
  uint8_t pixelFormat;
  int32_t width;
  int32_t height;
  uint32_t outBufCount;
  hbVPVideoType videoType;
  hbVPVideoRcParam rcParam;
  hbVPVideoGopParam gopParam;
  union {
    hbVPH264EncConfig h264EncConfig;
    hbVPH265EncConfig h265EncConfig;
  };
} hbVPVideoEncParam;

typedef void *hbVPVideoContext;

/**
 * @brief generate video encoding default parameters
 *
 * @param[int] param: get video encoding default parameters
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbVPGetDefaultVideoEncParam(hbVPVideoEncParam *param);

/**
 * @brief generate video encoding context
 *
 * @param[out] context: video encoding context, dispatched on VPU
 * @param[in] param: video encoding parameters
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbVPCreateVideoEncContext(hbVPVideoContext *context,
                                  hbVPVideoEncParam const *param);

/**
 * @brief release video encoding context
 *
 * @param[in] context: video encoding context
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbVPReleaseVideoEncContext(hbVPVideoContext context);

/**
 * @brief encode YUV to H264/H265 data
 * 
 * @param[out] taskHandle: task handle
 * @param[in] srcImg: the image to be encoded
 * @param[in] context: video encoding param context

 * @return 0 if success, return defined error code otherwise
 */
int32_t hbVPVideoEncode(hbUCPTaskHandle_t *taskHandle, hbVPImage const *srcImg,
                        hbVPVideoContext context);

/**
 * @brief get output buffer from codec
 *
 * @param[in] taskHandle: task handle
 * @param[out] outBuf: the buffer to store the encoded H264/H265 data.
 *                     The buffer is allocated internally by the codec, 
 *                     contains valid data upon the task is successfully completed, 
 *                     and is released during the task release phase.
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbVPGetVideoEncOutputBuffer(hbUCPTaskHandle_t taskHandle,
                                    hbVPArray *outBuf);

/**
 * @brief the video codec decoding parameters.
 *  
 * @param pixelFormat: the format of input video. 
 *                     support HB_VP_IMAGE_FORMAT_NV12 and HB_VP_IMAGE_FORMAT_YUV420. 
 * @param inBufSize: Specify the size of bitstream buffer for codec inner using. 
 *                   It's size should be larger than the feeding size and should align with 1024,
 *                   values[1024, 2^31-1] and the default value is 10*1024*1024.
 *                    usually set its value to height * width * Size(pixelFormat)
 * @param outBufCount: the count of output buffers. values[1, 31], default 5.
 * @param videoType: the decoding types of video
*/
typedef struct {
  uint8_t pixelFormat;
  uint32_t inBufSize;
  uint32_t outBufCount;
  hbVPVideoType videoType;
} hbVPVideoDecParam;

/**
 * @brief generate video decoding default parameters
 *
 * @param[in] param: get video decoding default parameters
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbVPGetDefaultVideoDecParam(hbVPVideoDecParam *param);

/**
 * @brief generate video decoding context
 *
 * @param[out] context: video decoding context, dispatched on VPU
 * @param[in] param: video decoding parameters
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbVPCreateVideoDecContext(hbVPVideoContext *context,
                                  hbVPVideoDecParam const *param);

/**
 * @brief release video decoding context
 *
 * @param[in] context: video decoding context
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbVPReleaseVideoDecContext(hbVPVideoContext context);

/**
 * @brief decode H264/H265 data to YUV
 * 
 * @param[out] taskHandle: task handle
 * @param[in] srcBuf: the H264/H265 data to be decoded
 * @param[in] context: video decoding context
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbVPVideoDecode(hbUCPTaskHandle_t *taskHandle, hbVPArray const *srcBuf,
                        hbVPVideoContext const context);

/**
 * @brief get output buffer from codec
 *
 * @param[in] taskHandle: task handle
 * @param[out] outImg: the buffer to store the decoded Image data.
 *                     The buffer is allocated internally by the codec, 
 *                     contains valid data upon the task is successfully completed, 
 *                     and is released during the task release phase.
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbVPGetVideoDecOutputBuffer(hbUCPTaskHandle_t taskHandle,
                                    hbVPImage *outImg);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // VP_HB_VP_VIDEO_CODEC_H_
