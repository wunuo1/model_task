// Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

// This header is provided for compatibility purposes only.
// Please consider using a newer version of the header file for best performance and functionality.
#ifndef HB_SYS_H_
#define HB_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stdint.h>

#include "hb_ucp_status.h"
#include "hb_ucp_sys.h"

typedef enum {
  HB_BPU_CORE_ANY = 0,
  HB_BPU_CORE_0 = (1 << 0),
  HB_BPU_CORE_1 = (1 << 1)
} hbBPUCore;

typedef enum {
  HB_DSP_CORE_ANY = 0,
  HB_DSP_CORE_0 = (1 << 0),
  HB_DSP_CORE_1 = (1 << 1)
} hbDSPCore;

typedef hbUCPSysMem hbSysMem;

typedef hbUCPSysMemFlushFlag hbSysMemFlushFlag;

typedef enum {
  HB_SYS_SUCCESS = 0,
  HB_SYS_INVALID_ARGUMENT = hbUCPStatus::HB_UCP_INVALID_ARGUMENT,
  HB_SYS_OUT_OF_MEMORY = hbUCPStatus::HB_UCP_MEM_ALLOC_FAIL,
  HB_SYS_REGISTER_MEM_FAILED = -6000131,
} hbSysStatus;

/**
 * Allocate system memory
 * @param[out] mem
 * @param[in] size
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbSysAllocMem(hbSysMem *mem, uint64_t size);

/**
 * Allocate cachable system memory
 * @param[out] mem
 * @param[in] size
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbSysAllocCachedMem(hbSysMem *mem, uint64_t size);

/**
 * Flush cachable system memory
 * @param[in] mem
 * @param[in] flag
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbSysFlushMem(hbSysMem *mem, int32_t flag);

/**
 * Write mem
 * @param[out] dest
 * @param[in] src
 * @param[in] size
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbSysWriteMem(hbSysMem *dest, char *src, uint64_t size);

/**
 * Read mem
 * @param[out] dest
 * @param[in] src
 * @param[in] size
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbSysReadMem(char *dest, hbSysMem *src, uint64_t size);

/**
 * Free mem
 * @param[in] mem
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbSysFreeMem(hbSysMem *mem);

/**
 * Register mem (vio etc) so that it can be used by BPU
 * @deprecated kept for compatibility purpose
 * @param[in] mem
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbSysRegisterMem(hbSysMem *mem);

/**
 * Unregister mem (already registered vio mem etc)
 * @deprecated kept for compatibility purpose
 * @param[in] mem
 * @return 0 if success, return defined error code otherwise
 */
int32_t hbSysUnregisterMem(hbSysMem *mem);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // HB_SYS_H_
