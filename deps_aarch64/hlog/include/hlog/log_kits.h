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
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "hlog/log_types.h"
#include "hlog/logging.h"

namespace hobot {
namespace hlog {

class HLOG_PUBLIC LogKits {
  using EnvConfigFunc = std::function<void(const char*)>;
  using EnvConfigMap = std::unordered_map<std::string, EnvConfigFunc>;

 public:
  /// @brief get the available environment variables description
  ///
  /// @return the string map of possible environment variables:
  ///         the first one: environment variable name
  ///         the second one: the description of the variable
  /// @note
  ///     available environment variables
  ///
  ///     HLOG_DEFAULT_LOGLEVEL
  ///     HLOG_MODULE_LOGLEVEL
  ///     HLOG_SINK_LOGLEVEL
  ///     HLOG_PREFIX_PATTERN
  ///     HLOG_ERROR_OUTPUT_FILE
  ///     HLOG_FILESINK_ARGUMENTS
  ///     HLOG_MAX_FILE_SIZE
  ///     HLOG_BLOCKING_MODE

  EnvConfig GetEnvironVarDescription(void) const;

  /// @brief get the current environment variables setting
  ///
  /// @return the string map of settings:
  ///         the first one: environment variable name
  ///         the second one: the value of the environment

  EnvConfig GetEnvironVarSetting(void) const;

  /// HLOG_DEFAULT_LOGLEVEL
  /// HLOG_MODULE_LOGLEVEL
  /// HLOG_SINK_LOGLEVEL
  /// HLOG_PREFIX_PATTERN
  /// HLOG_ERROR_OUTPUT_FILE
  /// HLOG_FILESINK_ARGUMENTS
  /// HLOG_MAX_FILE_SIZE
  /// HLOG_BLOCKING_MODE
  void CheckEnvVars(void);

  void DumpEnvConfig(const EnvConfig& config, std::ostream& ostr = std::cerr);

  void DumpLogStats(std::ostream& ostr = std::cerr);

  void DumpOneSink(const std::string& sink_name,
                   std::ostream& ostr = std::cerr);

  void DumpAllSink(std::ostream& ostr = std::cerr);

  /// @brief enable the backtrace in signal handler
  ///
  /// @param[in] signal_no: the signal number
  /// @param[in] quit: the progress exits after backtrace dump
  /// @param[in] force_exit: using _exit() to quit instead of exit()
  ///
  /// @return
  ///       true: done
  ///
  /// @note
  ///     only supported in Linux. return false on other S/W platforms
  ///     when exception happens in hlog_init_by_user(), records will be
  ///     outputed to stderr
  ///     when force_exit is true, the destructor of global variables are not
  ///     called
  ///

  bool EnableBacktraceForSignal(int signal_no,
                                bool quit = true,
                                bool force_exit = false);

  /// @brief get the call stack backtrace
  ///
  /// @return the trace result
  static std::vector<std::string> Backtrace(void);

  ////////  constructor ////////

  static std::shared_ptr<LogKits> Create(HobotLog* log);

  explicit LogKits(HobotLog* log);
  ~LogKits(void) = default;

 private:
  void InitEnvVars(void);
  void EnvHandleDefaultLogLevel(const char* env_val);
  void EnvHandleModuleLogLevel(const char* env_val);
  void EnvHandleSinkLogLevel(const char* env_val);
  void EnvHandleSinkConfig(const char* env_val);
  void EnvHandlePrefixPattern(const char* env_val);
  void EnvHandleErrorOutputFile(const char* env_val);
  void EnvHandleFileSinkArguments(const char* env_val);
  void EnvHandleMaxFileSize(const char* env_val);
  void EnvHandleBlockingMode(const char* env_val);

  bool ParseLogLevel(const std::string& env_val, LogLevel& level);

  EnvConfig env_var_description_;
  EnvConfig env_var_setting_;
  EnvConfigMap env_func_map_;
  HobotLog* log_;
};

}  // namespace hlog
}  // namespace hobot
