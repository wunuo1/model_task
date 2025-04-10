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

#pragma once

#include <atomic>
#include <chrono>
#include <thread>

#include "hlog/log_types.h"

// please use the CONFIG_HLOG_PROFILE option in CMakeLists.txt
// #define HLOG_PROFILE_ENABLED

namespace hobot {
namespace hlog {

extern thread_local int64_t log_started_ts;

/// get timestamp in ns
static inline int64_t get_prof_ts(void) {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

#define PROF_BUCKET_NUM 8

#ifndef HLOG_PROFILE_ENABLED

#define SET_PROFILER_TS(var)
#define SAVE_PROFILER_DATA(log_prof)

#else
#define SET_PROFILER_TS(var)        \
  do {                              \
    if (LogProfiler::prof_enable) { \
      (var) = get_prof_ts();        \
    }                               \
  } while (0)

#define SAVE_PROFILER_DATA(log_prof)                            \
  do {                                                          \
    if (log_prof.msg_done_ts > 0 && LogProfiler::prof_enable) { \
      LogProfiler::Instance()->UpdateData(log_prof);            \
    }                                                           \
  } while (0)
#endif

struct HLOG_PUBLIC LogProfiler {
  LogProfiler(int warm_up_count, int stats_count);
  void Reset(int warm_up_count, int stats_count);
  void UpdateData(const LogProf& log_prof);
  void DumpStats(void);

  static LogProfiler* Instance(void);
  static bool prof_enable;

  int32_t warm_up_count;  // skip first samples
  int32_t stats_count;    //

  /// how many data samples
  std::atomic<int64_t> sample_count;

  /// log start-->msg create
  std::atomic<int64_t> create_msg_cost[PROF_BUCKET_NUM];
  /// format cost: msg create --> start enqueue
  std::atomic<int64_t> log_format_cost[PROF_BUCKET_NUM];
  std::atomic<int64_t> log_enqueue_cost[PROF_BUCKET_NUM];
  std::atomic<int64_t> log_dequeue_cost[PROF_BUCKET_NUM];
  std::atomic<int64_t> log_sink_cost[PROF_BUCKET_NUM];  // sink cost:

  std::atomic<int64_t> create_msg_count[PROF_BUCKET_NUM];
  /// format cost: msg create --> start enqueue
  std::atomic<int64_t> log_format_count[PROF_BUCKET_NUM];
  std::atomic<int64_t> log_enqueue_count[PROF_BUCKET_NUM];
  std::atomic<int64_t> log_dequeue_count[PROF_BUCKET_NUM];
  std::atomic<int64_t> log_sink_count[PROF_BUCKET_NUM];  // sink cost:

  int64_t max_create_msg_cost[PROF_BUCKET_NUM];
  int64_t min_create_msg_cost[PROF_BUCKET_NUM];

  int64_t max_log_format_cost[PROF_BUCKET_NUM];
  int64_t min_log_format_cost[PROF_BUCKET_NUM];

  int64_t max_log_enqueue_cost[PROF_BUCKET_NUM];
  int64_t min_log_enqueue_cost[PROF_BUCKET_NUM];

  int64_t max_log_dequeue_cost[PROF_BUCKET_NUM];
  int64_t min_log_dequeue_cost[PROF_BUCKET_NUM];

  int64_t max_log_sink_cost[PROF_BUCKET_NUM];
  int64_t min_log_sink_cost[PROF_BUCKET_NUM];
};

}  // namespace hlog
}  // namespace hobot
