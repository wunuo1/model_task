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

#ifndef HLOG_LOG_TYPES_H_
#define HLOG_LOG_TYPES_H_

#include <atomic>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "hlog/compiler.h"

#define DEFAULT_FILE_SINK_NAME "file"
#define DEFAULT_CONSOLE_SINK_NAME "console"
#define MAGIC_DEFAULT_MODULE_NAME "default"
#define CRITICAL_LOG_EXIT_CODE 63

#define MAX_HLOG_FILE_NAME_LEN 32

namespace hobot {
namespace hlog {

/// LogLevel Definition
enum HLOG_PUBLIC LogLevel {
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
};

/// LogStats: statistics data structure
struct LogStats {
  /// the number of log received
  std::atomic<uint64_t> received;
  /// the number of log saved
  std::atomic<uint64_t> saved;
  /// the number of log dropped
  std::atomic<uint64_t> dropped;

  /// the number of saved bytes
  std::atomic<int64_t> saved_bytes;

  LogStats(void) {
    received = 0;
    saved = 0;
    dropped = 0;
    saved_bytes = 0;
  }

  LogStats& operator=(const LogStats& r) {
    received = r.received.load();
    saved = r.saved.load();
    dropped = r.dropped.load();
    saved_bytes = r.saved_bytes.load();
    return *this;
  }
};

/// LogInfo records the meta information about a log
///
///
struct LogInfo {
  /// application name
  const char* app_name;
  /// process id
  size_t pid;
  /// module name
  const char* module_name;
  /// the #th log  of the module
  int64_t count;
  /// timestamp from steady_clock
  int64_t mono_ts;
  /// thread id
  size_t tid;
  /// log level
  LogLevel level;
  /// timestamp from walltime
  int64_t system_ts;
  /// local time of wall time
  std::tm system_tm;
  /// line in source file
  int file_line;
  /// binary log or text log
  bool binary_log;
  /// not buffered, flush me and all logs before me
  bool un_buffered;
  /// filename, no path included
  char file_name[MAX_HLOG_FILE_NAME_LEN];
  /// fatal error, please exit after logged
  bool do_exit;
  /// binded sink id
  int64_t bind_sink_id;

  /// module id
  uint64_t module_id;
};

struct LogBuf {
  /// the header of userable address for log
  char* header_addr;
  /// the tail of userable address for log
  char* tail_addr;
  /// payload start address
  char* payload_addr;
  /// header size
  int header_size;
  /// payload size
  int payload_size;
  /// buffer start address
  char* start_addr;
  /// buffer end address
  char* end_addr;
};

struct LogProf {
  /// the log request started
  int64_t log_started_ts;
  /// the log msg created
  int64_t msg_created_ts;
  /// the log will try to enter queue
  int64_t msg_enqueue_start_ts;
  int64_t msg_enqueue_done_ts;
  /// the log leave the queue
  int64_t msg_dequeue_start_ts;
  int64_t msg_dequeue_done_ts;
  //// the log finished done
  int64_t msg_done_ts;
};

///
/// LogMsg: the internal reprenstation of a log
///
struct LogMsg {
  /// log_info: the log meat inforamtion
  LogInfo log_info;
  /// log_buf: the buffer where the log is stored
  LogBuf log_buf;
  /// log_prof: the profiling data
  LogProf log_prof;
};

/// the output function type to save log module's error
using ErrorOutputFunc = std::function<void(const char*, int len)>;

/// the output function type  to display log in console
using ConsoleLogFunc =
    std::function<void(LogLevel, const char*, const char*, int len)>;

using LogFlushFunc = std::function<void(void)>;

/// the configuation type for log module
using LogConfig = std::unordered_map<std::string, std::string>;

/// the configuation type for log module
using EnvConfig = std::unordered_map<std::string, std::string>;

/// the configuation type for log module
using SinkConfig = std::unordered_map<std::string, std::string>;

using LogMsgDeleter = std::function<void(LogMsg*)>;
using LogMsgPtr = std::unique_ptr<LogMsg, LogMsgDeleter>;

using UserExitFunc =
    std::function<bool(const char* module_name, const char* msg)>;

}  // namespace hlog
}  // namespace hobot

#endif  // HLOG_LOG_TYPES_H_
