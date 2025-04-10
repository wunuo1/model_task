// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef HB_UCP_SYS_H_
#define HB_UCP_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stdint.h>

typedef struct hbUCPSysMem {
  uint64_t phyAddr;
  void *virAddr;
  uint64_t memSize;
} hbUCPSysMem;

typedef enum {
  HB_SYS_MEM_CACHE_INVALIDATE = 1,
  HB_SYS_MEM_CACHE_CLEAN = 2
} hbUCPSysMemFlushFlag;

/**
 * Allocate system memory
 * @param[out] mem
 * @param[in] size
 * @param[in] deviceId
 * @return 0 if success, return defined error code otherwise
*/
int32_t hbUCPMalloc(hbUCPSysMem *mem, uint64_t size, int32_t deviceId);

/**
 * Allocate cacheable system memory
 * @param[out] mem
 * @param[in] size
 * @param[in] deviceId
 * @return 0 if success, return defined error code otherwise
*/
int32_t hbUCPMallocCached(hbUCPSysMem *mem, uint64_t size, int32_t deviceId);

/**
 * Refreshes the cached memory.
 * @param[in] mem
 * @param[in] flag
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbUCPMemFlush(hbUCPSysMem const *mem, int32_t flag);

/**
 * Free mem
 * @param[in] mem
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbUCPFree(hbUCPSysMem *mem);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // HB_UCP_SYS_H_
