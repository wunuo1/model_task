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

#ifndef HLOG_LOGGING_H_
#define HLOG_LOGGING_H_

#include <atomic>

#include "hlog/hobot_log.h"

/// to support HLOG_C(var)
#ifdef FMT_STRING
#undef FMT_STRING
#define FMT_STRING(var) (fmt::format("{}", var))
#endif

// no module interface
// HPLOG printf format
#define HPLOG_T(fmt, ...) \
  HPLOGM(                 \
      nullptr, hobot::hlog::log_trace, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define HPLOG_D(fmt, ...) \
  HPLOGM(                 \
      nullptr, hobot::hlog::log_debug, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define HPLOG_I(fmt, ...) \
  HPLOGM(nullptr, hobot::hlog::log_info, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define HPLOG_W(fmt, ...) \
  HPLOGM(nullptr, hobot::hlog::log_warn, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define HPLOG_E(fmt, ...) \
  HPLOGM(nullptr, hobot::hlog::log_err, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define HPLOG_C(fmt, ...)           \
  HPLOGM(nullptr,                   \
         hobot::hlog::log_critical, \
         __FILE__,                  \
         __LINE__,                  \
         fmt,                       \
         ##__VA_ARGS__)

// fmt
#define HFLOG_T(fmt, ...) \
  HFLOGM(                 \
      nullptr, hobot::hlog::log_trace, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define HFLOG_D(fmt, ...) \
  HFLOGM(                 \
      nullptr, hobot::hlog::log_debug, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define HFLOG_I(fmt, ...) \
  HFLOGM(nullptr, hobot::hlog::log_info, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define HFLOG_W(fmt, ...) \
  HFLOGM(nullptr, hobot::hlog::log_warn, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define HFLOG_E(fmt, ...) \
  HFLOGM(nullptr, hobot::hlog::log_err, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define HFLOG_C(fmt, ...)           \
  HFLOGM(nullptr,                   \
         hobot::hlog::log_critical, \
         __FILE__,                  \
         __LINE__,                  \
         fmt,                       \
         ##__VA_ARGS__)

// stream
#define HSLOG_T HSLOGM(nullptr, hobot::hlog::log_trace, __FILE__, __LINE__)

#define HSLOG_D HSLOGM(nullptr, hobot::hlog::log_debug, __FILE__, __LINE__)

#define HSLOG_I HSLOGM(nullptr, hobot::hlog::log_info, __FILE__, __LINE__)

#define HSLOG_W HSLOGM(nullptr, hobot::hlog::log_warn, __FILE__, __LINE__)

#define HSLOG_E HSLOGM(nullptr, hobot::hlog::log_err, __FILE__, __LINE__)

#define HSLOG_C HSLOGM(nullptr, hobot::hlog::log_critical, __FILE__, __LINE__)

// binary
#define HBLOG_T(prefix, buf, len) \
  HBLOGM(nullptr, hobot::hlog::log_trace, __FILE__, __LINE__, prefix, buf, len)

#define HBLOG_D(prefix, buf, len) \
  HBLOGM(nullptr, hobot::hlog::log_debug, __FILE__, __LINE__, prefix, buf, len)

#define HBLOG_I(prefix, buf, len) \
  HBLOGM(nullptr, hobot::hlog::log_info, __FILE__, __LINE__, prefix, buf, len)

#define HBLOG_W(prefix, buf, len) \
  HBLOGM(nullptr, hobot::hlog::log_warn, __FILE__, __LINE__, prefix, buf, len)

#define HBLOG_E(prefix, buf, len) \
  HBLOGM(nullptr, hobot::hlog::log_err, __FILE__, __LINE__, prefix, buf, len)

#define HBLOG_C(prefix, buf, len)   \
  HBLOGM(nullptr,                   \
         hobot::hlog::log_critical, \
         __FILE__,                  \
         __LINE__,                  \
         prefix,                    \
         buf,                       \
         len)

// module interface: module_name must be a string literal and cannot be a
// variable
#define HPLOGM_T(module_name, fmt, ...) \
  HPLOGM(module_name,                   \
         hobot::hlog::log_trace,        \
         __FILE__,                      \
         __LINE__,                      \
         fmt,                           \
         ##__VA_ARGS__)

#define HPLOGM_D(module_name, fmt, ...) \
  HPLOGM(module_name,                   \
         hobot::hlog::log_debug,        \
         __FILE__,                      \
         __LINE__,                      \
         fmt,                           \
         ##__VA_ARGS__)

#define HPLOGM_I(module_name, fmt, ...) \
  HPLOGM(module_name,                   \
         hobot::hlog::log_info,         \
         __FILE__,                      \
         __LINE__,                      \
         fmt,                           \
         ##__VA_ARGS__)

#define HPLOGM_W(module_name, fmt, ...) \
  HPLOGM(module_name,                   \
         hobot::hlog::log_warn,         \
         __FILE__,                      \
         __LINE__,                      \
         fmt,                           \
         ##__VA_ARGS__)

#define HPLOGM_E(module_name, fmt, ...) \
  HPLOGM(module_name,                   \
         hobot::hlog::log_err,          \
         __FILE__,                      \
         __LINE__,                      \
         fmt,                           \
         ##__VA_ARGS__)

#define HPLOGM_C(module_name, fmt, ...) \
  HPLOGM(module_name,                   \
         hobot::hlog::log_critical,     \
         __FILE__,                      \
         __LINE__,                      \
         fmt,                           \
         ##__VA_ARGS__)

// format
#define HFLOGM_T(module_name, fmt, ...) \
  HFLOGM(module_name,                   \
         hobot::hlog::log_trace,        \
         __FILE__,                      \
         __LINE__,                      \
         fmt,                           \
         ##__VA_ARGS__)

#define HFLOGM_D(module_name, fmt, ...) \
  HFLOGM(module_name,                   \
         hobot::hlog::log_debug,        \
         __FILE__,                      \
         __LINE__,                      \
         fmt,                           \
         ##__VA_ARGS__)

#define HFLOGM_I(module_name, fmt, ...) \
  HFLOGM(module_name,                   \
         hobot::hlog::log_info,         \
         __FILE__,                      \
         __LINE__,                      \
         fmt,                           \
         ##__VA_ARGS__)

#define HFLOGM_W(module_name, fmt, ...) \
  HFLOGM(module_name,                   \
         hobot::hlog::log_warn,         \
         __FILE__,                      \
         __LINE__,                      \
         fmt,                           \
         ##__VA_ARGS__)

#define HFLOGM_E(module_name, fmt, ...) \
  HFLOGM(module_name,                   \
         hobot::hlog::log_err,          \
         __FILE__,                      \
         __LINE__,                      \
         fmt,                           \
         ##__VA_ARGS__)

#define HFLOGM_C(module_name, fmt, ...) \
  HFLOGM(module_name,                   \
         hobot::hlog::log_critical,     \
         __FILE__,                      \
         __LINE__,                      \
         fmt,                           \
         ##__VA_ARGS__)

// stream
#define HSLOGM_T(module_name) \
  HSLOGM(module_name, hobot::hlog::log_trace, __FILE__, __LINE__)

#define HSLOGM_D(module_name) \
  HSLOGM(module_name, hobot::hlog::log_debug, __FILE__, __LINE__)

#define HSLOGM_I(module_name) \
  HSLOGM(module_name, hobot::hlog::log_info, __FILE__, __LINE__)

#define HSLOGM_W(module_name) \
  HSLOGM(module_name, hobot::hlog::log_warn, __FILE__, __LINE__)

#define HSLOGM_E(module_name) \
  HSLOGM(module_name, hobot::hlog::log_err, __FILE__, __LINE__)

#define HSLOGM_C(module_name) \
  HSLOGM(module_name, hobot::hlog::log_critical, __FILE__, __LINE__)

// binary
#define HBLOGM_T(module_name, prefix_str, buf, len) \
  HBLOGM(module_name,                               \
         hobot::hlog::log_trace,                    \
         __FILE__,                                  \
         __LINE__,                                  \
         prefix_str,                                \
         buf,                                       \
         len)

#define HBLOGM_D(module_name, prefix_str, buf, len) \
  HBLOGM(module_name,                               \
         hobot::hlog::log_debug,                    \
         __FILE__,                                  \
         __LINE__,                                  \
         prefix_str,                                \
         buf,                                       \
         len)

#define HBLOGM_I(module_name, prefix_str, buf, len) \
  HBLOGM(module_name,                               \
         hobot::hlog::log_info,                     \
         __FILE__,                                  \
         __LINE__,                                  \
         prefix_str,                                \
         buf,                                       \
         len)

#define HBLOGM_W(module_name, prefix_str, buf, len) \
  HBLOGM(module_name,                               \
         hobot::hlog::log_warn,                     \
         __FILE__,                                  \
         __LINE__,                                  \
         prefix_str,                                \
         buf,                                       \
         len)

#define HBLOGM_E(module_name, prefix_str, buf, len) \
  HBLOGM(module_name,                               \
         hobot::hlog::log_err,                      \
         __FILE__,                                  \
         __LINE__,                                  \
         prefix_str,                                \
         buf,                                       \
         len)

#define HBLOGM_C(module_name, prefix_str, buf, len) \
  HBLOGM(module_name,                               \
         hobot::hlog::log_critical,                 \
         __FILE__,                                  \
         __LINE__,                                  \
         prefix_str,                                \
         buf,                                       \
         len)

// module id wrapper
#define HPLOGM(module_name, level, file, line, fmt, ...) \
  HPLOGM_IMPL(module_name,                               \
              HLOG_MODULE_ID(module_name),               \
              level,                                     \
              file,                                      \
              line,                                      \
              fmt,                                       \
              ##__VA_ARGS__)

#define HFLOGM(module_name, level, file, line, fmt, ...) \
  HFLOGM_IMPL(module_name,                               \
              HLOG_MODULE_ID(module_name),               \
              level,                                     \
              file,                                      \
              line,                                      \
              fmt,                                       \
              ##__VA_ARGS__)

#define HSLOGM(module_name, level, file, line) \
  HSLOGM_IMPL(module_name, HLOG_MODULE_ID(module_name), level, file, line)

#define HBLOGM(module_name, level, file, line, prefix, buffer, len) \
  HBLOGM_IMPL(module_name,                                          \
              HLOG_MODULE_ID(module_name),                          \
              level,                                                \
              file,                                                 \
              line,                                                 \
              prefix,                                               \
              buffer,                                               \
              len)

//
// conditional, only needs for stream
//
#define HSLOG_COND(level, cond) HSLOGM_COND(nullptr, level, cond)

#define HSLOGM_COND(module_name, level, cond)   \
  HSLOGM_COND_IMPL(module_name,                 \
                   HLOG_MODULE_ID(module_name), \
                   level,                       \
                   cond,                        \
                   __FILE__,                    \
                   __LINE__)

// hlog 1.0 compatible interface
//

#define HLOG_T(fmt, ...) HFLOG_T(fmt, ##__VA_ARGS__)

#define HLOG_D(fmt, ...) HFLOG_D(fmt, ##__VA_ARGS__)

#define HLOG_I(fmt, ...) HFLOG_I(fmt, ##__VA_ARGS__)

#define HLOG_W(fmt, ...) HFLOG_W(fmt, ##__VA_ARGS__)

#define HLOG_E(fmt, ...) HFLOG_E(fmt, ##__VA_ARGS__)

#define HLOG_C(fmt, ...) HFLOG_C(fmt, ##__VA_ARGS__)

#define HTLOG_T(module_name, FMT, ...) HFLOGM_T(module_name, FMT, ##__VA_ARGS__)

#define HTLOG_D(module_name, FMT, ...) HFLOGM_D(module_name, FMT, ##__VA_ARGS__)

#define HTLOG_I(module_name, FMT, ...) HFLOGM_I(module_name, FMT, ##__VA_ARGS__)

#define HTLOG_W(module_name, FMT, ...) HFLOGM_W(module_name, FMT, ##__VA_ARGS__)

#define HTLOG_E(module_name, FMT, ...) HFLOGM_E(module_name, FMT, ##__VA_ARGS__)

#define HTLOG_C(module_name, FMT, ...) HFLOGM_C(module_name, FMT, ##__VA_ARGS__)

#define HSLOG_IF(level, cond) HSLOG_COND(level, cond)

#define HLOG_CHECK_PRINT(arg1, arg2, comm)          \
  HFLOGM_IMPL(nullptr,                              \
              HLOG_MODULE_ID(nullptr),              \
              hobot::hlog::log_err,                 \
              __FILE__,                             \
              __LINE__,                             \
              "[{} {} {}]  failed!",                \
              #arg1,                                \
              #comm,                                \
              #arg2);

#define HLOG_EQ(arg1, arg2)             \
  if ((arg1) != (arg2)) {               \
    HLOG_CHECK_PRINT(arg1, arg2, EQUAL) \
  }

#define HLOG_NE(arg1, arg2)                 \
  if ((arg1) == (arg2)) {                   \
    HLOG_CHECK_PRINT(arg1, arg2, NOT EQUAL) \
  }

#define HLOG_LE(arg1, arg2)                  \
  if ((arg1) > (arg2)) {                     \
    HLOG_CHECK_PRINT(arg1, arg2, LESS EQUAL) \
  }

#define HLOG_LT(arg1, arg2)                 \
  if ((arg1) >= (arg2)) {                   \
    HLOG_CHECK_PRINT(arg1, arg2, LESS THAN) \
  }

#define HLOG_GE(arg1, arg2)                     \
  if ((arg1) < (arg2)) {                        \
    HLOG_CHECK_PRINT(arg1, arg2, GREATER EQUAL) \
  }

#define HLOG_GT(arg1, arg2)                    \
  if ((arg1) <= (arg2)) {                      \
    HLOG_CHECK_PRINT(arg1, arg2, GREATER THAN) \
  }


#define HLOG_IF(level, condition, arg, ...)                       \
  if (condition &&                                                \
    LOGLEVEL_ENABLED(nullptr, HLOG_MODULE_ID(nullptr), level)) {  \
    hobot::hlog::HobotLog::Instance()->LogFmt(                    \
      nullptr,                                                    \
      HLOG_MODULE_ID(nullptr),                                    \
      level,                                                      \
      __FILE__,                                                   \
      __LINE__,                                                   \
      HLOG_FILE_POS_ID(__FILE__, __LINE__),                       \
      FMT_STRING(arg),                                            \
      ##__VA_ARGS__);                                             \
  }


#define HLOG_OCCURRENCES occurrence_##__FUNCTION__##__LINE__
#define HLOG_OCCURRENCES_MOD_N occurrence_mode_n##__FUNCTION__##__LINE__


#define HLOG_FIRST_N(n, level, arg, ...)               \
  {                                                    \
    static std::atomic<unsigned> HLOG_OCCURRENCES(0);  \
    if (HLOG_OCCURRENCES.fetch_add(1) < n &&           \
      LOGLEVEL_ENABLED(nullptr, HLOG_MODULE_ID(nullptr), level)) {           \
      hobot::hlog::HobotLog::Instance()->LogFmt(                    \
        nullptr,                                                    \
        HLOG_MODULE_ID(nullptr),                                    \
        level,                                                      \
        __FILE__,                                                   \
        __LINE__,                                                   \
        HLOG_FILE_POS_ID(__FILE__, __LINE__),                       \
        FMT_STRING(arg),                                            \
        ##__VA_ARGS__);                                             \
    }                                                  \
  }

#define HLOG_ONCE(level, arg, ...) HLOG_FIRST_N(1, level, arg, ##__VA_ARGS__)


#define HLOGM_EVERY_N(module_name, n, level, arg, ...)                    \
  do {                                                      \
    if (n <= 0) {                                          \
      continue;                                             \
    }                                                       \
    uint32_t num = static_cast<uint32_t>(n);                 \
    static std::atomic<uint32_t> HLOG_OCCURRENCES_MOD_N(0U); \
    uint32_t old_value = HLOG_OCCURRENCES_MOD_N;                \
    uint32_t new_value = 0U;                             \
    if (num == 1U) {                                       \
      new_value = 1U;                                    \
    } else {                                            \
      do {                                                    \
        new_value = old_value;                                \
        new_value++;                                          \
        if (new_value == num) {                                 \
        new_value = 0U;                                        \
        }                                                     \
      } while (!HLOG_OCCURRENCES_MOD_N.compare_exchange_weak( \
        old_value, new_value,                                 \
        std::memory_order_acq_rel, std::memory_order_acquire));       \
    }                                                         \
    if (new_value == 1U &&                                    \
      LOGLEVEL_ENABLED(module_name, HLOG_MODULE_ID(module_name), level)) {           \
      hobot::hlog::HobotLog::Instance()->LogFmt(                    \
        module_name,                                                    \
        HLOG_MODULE_ID(module_name),                                    \
        level,                                                      \
        __FILE__,                                                   \
        __LINE__,                                                   \
        HLOG_FILE_POS_ID(__FILE__, __LINE__),                       \
        FMT_STRING(arg),                                            \
        ##__VA_ARGS__);                                             \
    }                                                       \
  } while (0)



#define HLOG_EVERY_N(n, level, arg, ...)                              \
          HLOGM_EVERY_N(nullptr, n, level, arg,  ##__VA_ARGS__)       \

// set log level
#define HLOG_SET_LOGLEVEL(level) HLOGM_SET_LOGLEVEL(nullptr, level)

#define HLOG_GET_LOGLEVEL() HLOGM_GET_LOGLEVEL(nullptr)



#endif  // HLOG_LOGGING_H_
