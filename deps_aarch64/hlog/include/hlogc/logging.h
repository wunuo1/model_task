// Copyright (c) [2021-2023] [Horizon Robotics].
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

#ifndef HLOGC_LOGGING_H_
#define HLOGC_LOGGING_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "log_types.h"
#include "hlog/compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

// --------------------------- no module interface -----------------------------
// printf style
#define CHPLOG_T(fmt, ...) \
  CHPLOGM(NULL, log_trace, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define CHPLOG_D(fmt, ...) \
  CHPLOGM(NULL, log_debug, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define CHPLOG_I(fmt, ...) \
  CHPLOGM(NULL, log_info, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define CHPLOG_W(fmt, ...) \
  CHPLOGM(NULL, log_warn, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define CHPLOG_E(fmt, ...) \
  CHPLOGM(NULL, log_err, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define CHPLOG_C(fmt, ...) \
  CHPLOGM(NULL, log_critical, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

// binary
#define CHBLOG_T(prefix, buf, len) \
  CHBLOGM(NULL, log_trace, __FILE__, __LINE__, prefix, buf, len)

#define CHBLOG_D(prefix, buf, len) \
  CHBLOGM(NULL, log_debug, __FILE__, __LINE__, prefix, buf, len)

#define CHBLOG_I(prefix, buf, len) \
  CHBLOGM(NULL, log_info, __FILE__, __LINE__, prefix, buf, len)

#define CHBLOG_W(prefix, buf, len) \
  CHBLOGM(NULL, log_warn, __FILE__, __LINE__, prefix, buf, len)

#define CHBLOG_E(prefix, buf, len) \
  CHBLOGM(NULL, log_err, __FILE__, __LINE__, prefix, buf, len)

#define CHBLOG_C(prefix, buf, len) \
  CHBLOGM(NULL, log_critical, __FILE__, __LINE__, prefix, buf, len)

// --------------------------- module interface -----------------------------
// printf style
#define CHPLOGM_T(module_name, fmt, ...) \
  CHPLOGM(module_name, log_trace, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define CHPLOGM_D(module_name, fmt, ...) \
  CHPLOGM(module_name, log_debug, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define CHPLOGM_I(module_name, fmt, ...) \
  CHPLOGM(module_name, log_info, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define CHPLOGM_W(module_name, fmt, ...) \
  CHPLOGM(module_name, log_warn, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define CHPLOGM_E(module_name, fmt, ...) \
  CHPLOGM(module_name, log_err, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define CHPLOGM_C(module_name, fmt, ...) \
  CHPLOGM(module_name, log_critical, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

// binary
#define CHBLOGM_T(module_name, prefix_str, buf, len) \
  CHBLOGM(module_name, log_trace, __FILE__, __LINE__, prefix_str, buf, len)

#define CHBLOGM_D(module_name, prefix_str, buf, len) \
  CHBLOGM(module_name, log_debug, __FILE__, __LINE__, prefix_str, buf, len)

#define CHBLOGM_I(module_name, prefix_str, buf, len) \
  CHBLOGM(module_name, log_info, __FILE__, __LINE__, prefix_str, buf, len)

#define CHBLOGM_W(module_name, prefix_str, buf, len) \
  CHBLOGM(module_name, log_warn, __FILE__, __LINE__, prefix_str, buf, len)

#define CHBLOGM_E(module_name, prefix_str, buf, len) \
  CHBLOGM(module_name, log_err, __FILE__, __LINE__, prefix_str, buf, len)

#define CHBLOGM_C(module_name, prefix_str, buf, len) \
  CHBLOGM(module_name, log_critical, __FILE__, __LINE__, prefix_str, buf, len)

// --------------------------- internal -----------------------------
#define CHPLOGM(module_name, level, file, line, fmt, ...) \
  LogPrintf(module_name, level, file, line, fmt, ##__VA_ARGS__)

#define CHBLOGM(module_name, level, file, line, prefix, buffer, len) \
  LogBinary(module_name, level, file, line, prefix, (uint8_t*)buffer, len)

#ifdef __GNUC__
#define PRINT_ARGUMENT_CHECK(m, n) __attribute__((format(printf, m, n)))
#else
#define PRINT_ARGUMENT_CHECK(m, n)
#endif
bool PRINT_ARGUMENT_CHECK(5, 6) HLOG_PUBLIC LogPrintf(const char* module_name,
                                                       LogLevel level,
                                                       const char* file,
                                                       int line,
                                                       const char* format,
                                                       ...);

bool HLOG_PUBLIC LogBinary(const char* module_name,
                            LogLevel level,
                            const char* file,
                            int line,
                            const char* prefix,
                            const uint8_t* buffer,
                            int len);

#ifdef __cplusplus
}
#endif

#endif  // HLOGC_LOGGING_H_
