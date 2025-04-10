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

#ifndef HLOGC_LOG_TYPES_H_
#define HLOGC_LOG_TYPES_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  /// log trace
  log_trace = 0,
  /// log debug
  log_debug,
  /// log info
  log_info,
  /// log warn
  log_warn,
  /// log error
  log_err,
  /// log critical
  log_critical,
  /// the highest one, never output any log
  log_never
} LogLevel;

typedef struct {
  // the name of log file
  char *file_name;
  // the max size of all log file, can not lager than the global max size, which
  // is set by SetMaxFileSize()
  uint64_t max_size;
  // the max rotate number of log file
  uint32_t rotate_number;
} FileSinkConfig;

typedef struct {
  // the config name
  char *cfg_name;
  // the config value
  char *cfg_value;
} SinkConfig;

typedef struct {
  // the burst log number in 1 second
  int burst_number;
  // the interval timeframe, the limit meantained duration
  int interval_in_millisecond;
  // the interval log number. <=0 means no interval log number, the limit mean
  // tained number of log
  int interval_in_number;
} RateLimit;

typedef char ** ModuleList;
typedef char ** SinkList;
typedef SinkConfig * SinkConfigList;

#ifdef __cplusplus
}
#endif

#endif  // HLOGC_LOG_TYPES_H_
