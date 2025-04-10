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

#ifndef HLOG_HOBOT_LOG_H_
#define HLOG_HOBOT_LOG_H_

#include "hlog/compiler.h"

HLOG_MSVC_SUPPRESS_WARNING_WITH_PUSH(4505)
#include <fmt/format.h>
#include <fmt/printf.h>
HLOG_MSVC_SUPPRESS_WARNING_POP
#include <stdio.h>
#include <stdarg.h>

#include <cstdarg>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "hlog/compiler.h"
#include "hlog/file_policy.h"
#include "hlog/fmt_printf.h"
#include "hlog/log_error.h"
#include "hlog/log_types.h"
#include "hlog/stream_log.h"
#include "hlog/utils.h"

/// wrapper for generate module id from module_name
/// module_name MUST be a string literal
#define HLOG_MODULE_ID(module_name) \
  (hobot::hlog::utils::gen_module_id(module_name))

/// wrapper for generate file position id
/// file MUST be a string literal
#define HLOG_FILE_POS_ID(file, line) \
  (hobot::hlog::utils::gen_file_pos_id(file, line))

#define LOGLEVEL_ENABLED(module_name, module_id, level)                \
  ((hobot::hlog::HobotLog::GetGlobalLevel() == hobot::hlog::log_never) \
       ? (level >= hobot::hlog::HobotLog::GetGlobalMinLevel()          \
              ? hobot::hlog::HobotLog::Instance()->LogLevelEnabled(    \
                    module_name, module_id, level)                     \
              : false)                                                 \
       : (level >= hobot::hlog::HobotLog::GetGlobalLevel()))

#define HPLOGM_IMPL(module_name, module_id, level, file, line, fmt, ...)      \
  (LOGLEVEL_ENABLED(module_name, module_id, level)                            \
       ? (hobot::hlog::HobotLog::Instance()->CheckPrintfParam(fmt,            \
                                                              ##__VA_ARGS__), \
          hobot::hlog::HobotLog::Instance()->LogPrintf(                       \
              module_name,                                                    \
              module_id,                                                      \
              level,                                                          \
              file,                                                           \
              line,                                                           \
              HLOG_FILE_POS_ID(file, line),                                   \
              fmt,                                                            \
              ##__VA_ARGS__))                                                 \
       : true)

/// printf style Log interface
#ifdef __GNUC__
#define PRINT_ARGUMENT_CHECK(m, n) __attribute__((format(printf, m, n)))
#else
#define PRINT_ARGUMENT_CHECK(m, n)
#endif

/// format style Log interface
#define HFLOGM_IMPL(module_name, module_id, level, file, line, fmt, ...) \
  (LOGLEVEL_ENABLED(module_name, module_id, level)                       \
       ? hobot::hlog::HobotLog::Instance()->LogFmt(                      \
             module_name,                                                \
             module_id,                                                  \
             level,                                                      \
             file,                                                       \
             line,                                                       \
             HLOG_FILE_POS_ID(file, line),                               \
             FMT_STRING(fmt),                                            \
             ##__VA_ARGS__)                                              \
       : true)

/// stream style log interface
/// here is a trick to optimize when loglevel is low priority
#define HSLOGM_IMPL(module_name, module_id, level, file, line) \
  !LOGLEVEL_ENABLED(module_name, module_id, level)             \
      ? false                                                  \
      : hobot::hlog::StreamAbsorber() &&                       \
            hobot::hlog::StreamLog(module_name,                \
                                   module_id,                  \
                                   level,                      \
                                   file,                       \
                                   line,                       \
                                   HLOG_FILE_POS_ID(file, line))

/// stream style cond log interface
/// only stream log needs special interface for conditional log
#define HSLOGM_COND_IMPL(module_name, module_id, level, cond, file, line) \
  !(LOGLEVEL_ENABLED(module_name, module_id, level) && (cond))            \
      ? false                                                             \
      : hobot::hlog::StreamAbsorber() &&                                  \
            hobot::hlog::StreamLog(module_name,                           \
                                   module_id,                             \
                                   level,                                 \
                                   file,                                  \
                                   line,                                  \
                                   HLOG_FILE_POS_ID(file, line))

/// binary style log interface
#define HBLOGM_IMPL(                                                \
    module_name, module_id, level, file, line, prefix, buffer, len) \
  (LOGLEVEL_ENABLED(module_name, module_id, level)                  \
       ? hobot::hlog::HobotLog::Instance()->LogBinary(              \
             module_name,                                           \
             module_id,                                             \
             level,                                                 \
             file,                                                  \
             line,                                                  \
             HLOG_FILE_POS_ID(file, line),                          \
             prefix,                                                \
             reinterpret_cast<const uint8_t*>(buffer),              \
             len)                                                   \
       : true)

/// set log level with module_name
#define HLOGM_SET_LOGLEVEL(module_name, level) \
  hobot::hlog::HobotLog::Instance()->SetLogLevel(module_name, level)

/// get log level with module_name
#define HLOGM_GET_LOGLEVEL(module_name) \
  hobot::hlog::HobotLog::Instance()->GetLogLevel(module_name)

namespace hobot {
namespace hlog {

/// Forward declare
class LogContext;

/// HobotLog: the interface class of log module
///           provides APIs that application can use
class HLOG_PUBLIC HobotLog {
 public:
  /// @brief LogPrintf: the printf style log interface
  ///
  /// @param[in] module_name: the module name
  /// @param[in] module_id:   the module id
  /// @param[in] level:       the log level
  /// @param[in] file:        the file name
  /// @param[in] line:        the line in the file
  /// @param[in] pos_id:      the hash value of the file and the line
  /// @param[in] fmt:         the printf style format string
  /// @param[in] ...args      var arguments
  ///
  /// @return   true: loggging goes well
  ///           false: logging failed
  ///
  /// @note
  ///        return true does not mean the log has beed saved already
  ///        when return false, please use GetLastError() to get the errorno
  //
  using ArgFunc = std::function<std::string()>;
  using ArgFuncPtr = std::string(*)();
  template <typename S, typename... Args>
  bool LogPrintf(const char* module_name,
                 uint64_t module_id,
                 LogLevel level,
                 const char* file,
                 int line,
                 uint64_t pos_id,
                 const S& format,
                 Args&&... args) {
    PrintBuffer print_buffer;

    ResetErrorNo();

    if (!CreatePrintBuffer(
            module_name, module_id, level, file, line, pos_id, print_buffer))
      return false;

    using context = fmt::basic_printf_context_t<char>;
    auto print_args = fmt::make_format_args<context>(args...);

    try {
      fmt::detail::vprintf(print_buffer,
                           fmt::string_view(format),
                           fmt::basic_format_args<context>(print_args));
    }

    catch (const std::overflow_error& e) {
      static_cast<void>(e);
      // nothing need to do
    } catch (const std::exception& e) {
      static_cast<void>(e);
      SetLastErrorNo(HB_EINVAL);
      ErrorOutput(file, line, " mismatched format: %s\n", format);
      DestroyPrintBuffer(&print_buffer, true);
      return false;
    }

    DestroyPrintBuffer(&print_buffer, false);

    return true;
  }

  /// @brief LogPrintfC: the printf style log interface implemented in C style
  ///
  /// @param[in] module_name: the module name
  /// @param[in] module_id:   the module id
  /// @param[in] level:       the log level
  /// @param[in] file:        the file name
  /// @param[in] line:        the line in the file
  /// @param[in] pos_id:      the hash value of the file and the line
  /// @param[in] fmt:         the printf style format string
  /// @param[in] args         var arguments
  ///
  /// @return   true: loggging goes well
  ///           false: logging failed
  ///
  /// @note
  ///        return true does not mean the log has beed saved already
  ///        when return false, please use GetLastError() to get the errorno
  //
  bool LogPrintfC(const char* module_name,
                  uint64_t module_id,
                  LogLevel level,
                  const char* file,
                  int line,
                  uint64_t pos_id,
                  const char* fmt,
                  va_list args);

  /// helper functions for printf
  bool CreatePrintBuffer(const char* module_name,
                         uint64_t module_id,
                         LogLevel level,
                         const char* file,
                         int line,
                         uint64_t pos_id,
                         PrintBuffer& print_buffer);

  void DestroyPrintBuffer(PrintBuffer* print_buffer, bool free_only);

  /// parameter checking for gcc
  void PRINT_ARGUMENT_CHECK(2, 3) CheckPrintfParam(const char* fmt, ...) {
    (void)fmt;
  }

  /// @brief LogFmt: the std::fmt style log interface
  ///
  /// @param[in] module_name: the module name
  /// @param[in] module_id:   the module id
  /// @param[in] level:       the log level
  /// @param[in] file:        the file name
  /// @param[in] line:        the line in the file
  /// @param[in] pos_id:      the hash value of the file and the line
  /// @param[in] format:      the std::fmt style format string
  /// @param[in] args         var arguments
  ///
  /// @return   true: loggging goes well
  ///           false: logging failed
  ///
  /// @note
  ///        return true does not mean the log has beed saved already
  ///        when return false, please use GetLastError() to get the errorno
  template <typename S, typename... Args>
  bool LogFmt(const char* module_name,
              uint64_t module_id,
              LogLevel level,
              const char* file,
              int line,
              uint64_t pos_id,
              const S& format,
              Args&&... args) {
    return LogFmtInternal(module_name,
                          module_id,
                          level,
                          file,
                          line,
                          pos_id,
                          format,
                          std::move(fmt::make_format_args(args...)));
  }

  /// @brief LogFmtInternal: the internal interface for fmt style log
  ///
  /// @param[in] module_name: the module name
  /// @param[in] module_id:   the module id
  /// @param[in] level:       the log level
  /// @param[in] file:        the file name
  /// @param[in] line:        the line in the file
  /// @param[in] pos_id:      the hash value of the file and the line
  /// @param[in] format:      the std::format style format string
  /// @param[in] args         the wrapped fmt args
  ///
  /// @return   true: loggging goes well
  ///           false: logging failed
  ///
  /// @note
  ///        return true does not mean the log has beed saved already
  ///        when return false, please use GetLastError() to get the errorno
  bool LogFmtInternal(const char* module_name,
                      uint64_t module_id,
                      LogLevel level,
                      const char* file,
                      int line,
                      uint64_t pos_id,
                      fmt::string_view format,
                      fmt::format_args&& args);

  /// @brief LogBinary: the log interface to save binary data, the generated
  /// log format is:
  ///                   <log prefix> <prefix_str> <length: 4 bytes int> <real
  ///                   saved length: 4 bytes int>
  ///
  /// @param[in] module_name: the module name
  /// @param[in] module_id:   the module id
  /// @param[in] level:       the log level
  /// @param[in] file:        the file name
  /// @param[in] line:        the line in the file
  /// @param[in] pos_id:      the hash value of the file and the line
  /// @param[in] prefix_str:  the annotation string for the data
  /// @param[in] buffer:      the data address
  /// @param[in] len:         the data size
  ///
  /// @return   true: loggging goes well
  ///           false: logging failed
  ///
  /// @note
  ///        return true does not mean the log has beed saved already
  ///        when return false, please use GetLastError() to get the errorno

  bool LogBinary(const char* module_name,
                 uint64_t module_id,
                 LogLevel level,
                 const char* file,
                 int line,
                 uint64_t pos_id,
                 const char* prefix_str,
                 const uint8_t* buffer,
                 int len);

  /// @brief set the application name
  ///
  /// @param[in] app_name:   the application name
  ///
  /// @return true: done
  ///         false: called after InitDone() is called
  ///
  bool SetAppName(const char* app_name);

  /// @brief get the application name
  ///
  /// @return  the appliation name string
  ///
  std::string GetAppName(void) const;

  /// @brief set the log level of the module with module_name
  ///
  /// @param[in] module_name: if nullptr, the "default" module
  /// @param[in] level:        the new log level
  ///
  /// @return    true:   done
  ///            false:  level is outof range
  ///                   or cannot create new module with module_name
  ///
  bool SetLogLevel(const char* module_name, LogLevel level);

  /// @brief get the log level of the module with module_name
  ///
  /// @param[in] module_name: if nullptr, the "default"  module
  ///
  /// @return    the log level
  ///
  /// @note
  ///        if no such module exists, log_never will be returned
  LogLevel GetLogLevel(const char* module_name) const;

  /// @brief check if the log level enabled in a module
  ///
  /// @param[in] module_name: if nullptr, the "default" module
  /// @param[in] level:       the log level
  ///
  /// @return    true:  log enabled for the target level
  ///            false: log is disabled for the target level, or
  ///                   the module does not exist and
  ///                   failed to create new module
  ///
  bool LogLevelEnabled(const char* module_name, LogLevel level);

  /// @brief check if the log level enabled in a module
  ///
  /// @param[in] module_name: if nullptr, the "default" module
  /// @param[in] module_id:  the module_id
  /// @param[in] level:       the log level
  ///
  /// @return    true:  log enabled for the target level
  ///            false: log is disabled for the target level, or
  ///                   the module does not exist and
  ///                   failed to create new module

  bool LogLevelEnabled(const char* module_name,
                       uint64_t module_id,
                       LogLevel level);

  /// @brief set the log prefix format, such as the position of pid,app_id,..
  /// etc
  ///        supported token are:
  ///         %P  process id             "1232"
  ///         %t  Thread id              "1232"
  ///         %n  Application name       "nginx"
  ///         %k  Module name            "communication"
  ///         %K  Mono ts counter        "123409"
  ///         %L  Short log level        "D","I"
  ///         %l  Long log level         "debug","info"
  ///         %s  Basename of source file "a.cpp"
  ///         %#  Source line             "123"
  ///         %y  Year  1900-xxxx         "2023"
  ///         %m  Month 01-12             "11"
  ///         %d  Day   01-31             "30"
  ///         %H  Hours 00-23             "23"
  ///         %M  Minutes 00-59           "59"
  ///         %S  Seconds 00-59           "59"
  ///         %e  Millisecond 000-999     "678"
  ///         %E  Microsecond xxxxxx      "002030"
  ///
  /// @param[in] format_str: the format string
  ///
  /// @return  true: done
  ///          false: format_str includes unsupported token
  ///                 or called after InitDone() is called

  bool SetLogPrefixFormat(const std::string& format_str);

  /// @brief get the log format string
  ///
  /// @return the prefix format string
  std::string GetLogPrefixFormat(void) const;

  /// fast-path for console sink and file sink

  /// @brief set the config arguments for file sink.
  ///        after setting, file sink is working
  ///
  /// @param[in] file_name: the file name to store the log
  /// @param[in] max_size:  the max size(in Mbytes), including rotate parts
  /// @param[in] rotate_number: at least 1,at most 128
  /// @param[in] sink_name: the target file-releated sink name, nullptr means
  ///                        the default one "file"
  /// @param[in] enable: automatically enable the file sink if setting is ok
  ///
  /// @return true: done
  ///         false: rotate_number or max_size is wrong,
  ///                or file_name cannot be openned
  ///                or target sink does not exist
  /// @note
  ///     default max_size is 8192 M, it can be changed by SetMaxFileSize()
  ///
  ///     if the sink is enabled, this function will disable sink first and
  ///      enable it after setting

  bool SetFileSinkConfig(const std::string& file_name,
                         uint64_t max_size,
                         uint32_t rotate_number,
                         const char* sink_name = nullptr,
                         bool enable = true);

  /// @brief get the config arguments for file sink
  ///
  /// @param[out] file_name: the file name to store the log
  /// @param[out] max_size:  the max size(in Mbytes), including rotate parts
  /// @param[out] rotate_number: rotate number
  /// @param[in] sink_name: the target file-releated sink name, nullptr means
  /// the default one
  ///
  /// @return  false: no file sink with sink_name

  bool GetFileSinkConfig(std::string& file_name,
                         uint64_t& max_size,
                         uint32_t& rotate_number,
                         const char* sink_name = nullptr) const;

  /// @brief  set the file policy for a generic file sink instance
  ///
  /// @param[in] file_sink_name: the target file sink name
  /// @param[in] ptr: the file policy shared_ptr
  ///
  /// @return  false: file_sink_name is not found or not a file sink
  ///                 ptr is nullptr
  ///                 sink with file_sink_name already has a file policy
  /// @note
  ///      use CreateSink("generic_file",file_sink_name) to create a new sink

  bool SetFilePolicy(const std::string& file_sink_name,
                     const FilePolicyPtr& ptr);

  /// @breif set the max log file size, the upbound of the max_size in
  /// SetFileSinkConfig
  ///
  /// @pararm[in]: max_size: max file size permitted for a single file sink,
  /// in Mbyte. Permitted range: 1<= max_size<=(1<<20)
  ///
  /// @return true: done
  ///         false: max_size is out of range

  bool SetMaxFileSize(uint64_t max_size);

  /// @breif get the max log file size, the upbound of the max_size in
  /// SetFileSinkConfig
  ///
  /// @return the max file size in Mbyte
  ///

  uint64_t GetMaxFileSize(void) const;

  /// @brief set the redirect function of console sink
  ///
  /// @param[in] redirect_func: the redirect function
  /// @param[in] flush_func:  the flush function
  /// @param[in] shared: if true, the original console output still work,
  ///                     otherwise, only the redirect fuction works
  /// @return true: done
  ///         false: console is enabled
  /// @note
  ///       console has to be disabled before calling this function

  bool SetConsoleLogRedirectFunc(const ConsoleLogFunc redirect_func,
                                 const LogFlushFunc flush_func = nullptr,
                                 bool shared = false);

  /// @brief get the redirect function set by SetConsoleLogRedirectFunc()
  ///
  /// @return the redirect func setted before
  ConsoleLogFunc GetConsoleLogRedirectFunc(void);

  /// @brief get the last error number
  ///        each thread has its own error number
  ///
  /// @return the error number

  uint32_t GetErrorNo(void);

  /// @brief get the last error message, per thread
  ///
  /// @return the error message string

  std::string GetErrorMsg(void);

  /// @brief get log stats of all module
  ///
  /// @param[out] stats: the returned stats data
  ///
  void GetLogStats(LogStats& stats) const;

  /// @brief get log stats of a module
  ///
  /// @param[in] module_name: the module name. nullptr means the "default"
  /// module
  /// @param[out] stats: the returned stats data
  ///
  /// @return true: done
  ///         false: no such module
  bool GetLogStats(const char* module_name, LogStats& stats) const;

  /// @brief set a module's log rate limit
  ///        if log bursted more than burst_number in  1 second
  ///         further logs will be dropped until the interval time or number
  ///         is over
  ///
  /// @param[in] module_name: the module_name
  /// @param[in] burst_number: the burst log number in 1 second
  /// @param[in] interval_in_millisecond: the interval timeframe
  /// @param[in] interval_in_number: the interval number,<=0 means no check
  ///
  /// @return true: done
  ///         false: cannot create new module with module_name due to  module
  ///         number limit
  /// @note
  ///      if burst_number <=0 or interval_in_millisecond <=0, this rate limit
  ///      is disabled.
  ///        this interface can create new log module with module_name

  bool SetLogRateLimit(const char* module_name,
                       int burst_number,
                       int interval_in_millisecond,
                       int interval_in_number = -1);

  /// @brief get a module's log rate limit setting
  ///
  /// @param[in]  module_name: the module_name
  /// @param[out] burst_number: the burst log number
  /// @param[out] interval_in_millisecond: the interval timeframe
  /// @param[out] interval_in_number: the interval number
  ///
  /// @return  true: done
  ///          false: no such module, or rate limit is not enabled, not data
  ///          retrievaled

  bool GetLogRateLimit(const char* module_name,
                       int& burst_number,
                       int& interval_in_millisecond,
                       int& interval_in_number) const;

  /// @brief set the rate limit on single log generation statement
  ///        if a single log statement generated burst_number logs in 1 second
  ///         further logs will be dropped until interval_in_number logs
  ///         dropped or interval_in_millisecond passed
  ///
  /// @param[in] burst_number: the burst log number
  /// @param[in] interval_in_millisecond: the interval timeframe
  /// @param[in] interval_in_number: the interval log number. <=0 means no
  /// interval log number
  ///
  /// @note
  ///       if burst_number <=0 or interval_in_millisecond <=0, this rate
  ///       limit is disabled

  void SetRateLimitOnSingleLog(int burst_number,
                               int interval_in_millisecond,
                               int interval_in_number = -1);

  /// @brief get the rate limit on single log generation statement
  ///
  /// @param[out] burst_number: the burst log number
  /// @param[out] interval_in_millisecond: the interval timeframe
  /// @param[out] interval_in_number: the interval log number
  ///
  /// @return true: done
  ///         false: rate limit is not enabled, and not date retrievaled
  bool GetRateLimitOnSingleLog(int& burst_number,
                               int& interval_in_millisecond,
                               int& interval_in_number) const;

  /// @brief set the buffered log number limit
  ///
  /// @param[in] number: the number of log can be buffered
  ///
  /// @return true: done
  ///         false: number is zero or too big
  bool SetBufferedLogLimit(uint32_t number);

  /// @brief get the buffered log number limit
  ///
  /// @return the buffered log number limit
  uint32_t GetBufferedLogLimit(void) const;

  /// @brief get the max buffered log limit number
  ///
  /// @return the max number
  uint32_t GetMaxBufferedLogLimit(void) const;

  /// @brief set the file to save the log module error output
  ///        if not set, log module error will be printed in console
  ///
  /// @param[in] the file to save the error output
  ///
  /// @return true: done
  ///         false: file cannot create or access the target file
  ///                or the output file has been set already

  bool SetErrorOutputFile(const std::string& file_name);

  /// @brief get the log module error output file
  ///
  /// @return the output file, maybe empty string if no file is set

  std::string GetErrorOutputFile(void) const;

  /// @brief enable or disable the error message to be printed in console
  ///        if the error message has been redirected or saved to file
  ///        the output to console can be disabled
  ///
  /// @param[in] enable: true to enable console print

  void SetErrorOutputConsoleEnable(bool enable);

  /// @brief redirect the log module error output
  ///        if not set, log module error will be printed in console
  ///
  /// @param[in] output_func: the redirect output function
  ///
  void SetErrorOutputFunc(const ErrorOutputFunc& output_func);

  /// @brief get the log module error output func set by SetErrorOutputFunc()
  ///
  /// @return the error output func
  ///
  ErrorOutputFunc GetErrorOutputFunc(void) const;

  /// @brief get the module list
  ///
  /// @return the module_name list, at least have one, the "default" module

  std::vector<std::string> GetModuleList(void) const;

  /// @brief load the library implemented a sink
  ///
  /// @param[in] lib_path: the dynamic library path
  /// @param[in] arg: argument passed to plugin init function
  ///
  /// @return true: done
  ///         false: some error happens
  bool LoadSinkLibrary(const std::string& lib_path, void* arg = nullptr);

  /// @brief get the sink list
  ///
  /// @return the sink name list, at least includes "console" and "file"
  std::vector<std::string> GetSinkList(void) const;

  /// @brief config the sink
  ///
  /// @param[in] sink_name: the sink name
  /// @param[in] config_name: the config name
  /// @param[in] config_value: the config value represented in string
  ///
  /// @return true: done
  ///         false: sink name cannot be found
  ///                or config item is wrong
  bool SetSinkConfig(const std::string& sink_name,
                     const std::string& config_name,
                     const std::string& config_value);

  /// @brief get the configs of a sink
  ///
  /// @param[in] sink_name: the sink name
  ///
  /// @return the config map of config items
  SinkConfig GetSinkConfig(const std::string& sink_name) const;

  /// @brief toggle sink is enable or disable
  ///
  /// @param[in] sink_name: the sink name
  /// @param[in] enable_status:   the sink enable target status
  ///
  /// @return true: done
  ///         fasle: the sink name cannot found
  bool ToggleSinkEnable(const std::string& sink_name,
                        bool enable_status = true);

  /// @brief check if sink is enabled
  ///
  /// @param[in] sink_name: the sink name
  ///
  /// @return the enable status of the sink
  ///         if sink_name is not found, also return false
  bool IsSinkEnabled(const std::string& sink_name) const;

  /// @brief set the log level of a sink
  ///
  /// @param[in] sink_name: the sink name
  /// @param[in] level: the log level
  ///
  /// @return true: done
  ///         false: sink_name wrong or level wrong
  ///                or the concrete sink denied the setting

  bool SetSinkLogLevel(const std::string& sink_name, LogLevel level);

  /// @brief get the log level of a sink
  ///
  /// @param[in] sink_name: the sink name
  ///
  /// @return the log level of the sink
  ///         log_never if no such sink exists
  LogLevel GetSinkLogLevel(const std::string& sink_name) const;

  /// @brief get the sink statistics
  ///
  /// @param[in] sink_name: the sink name
  /// @param[in] stats: the statistics data
  ///
  /// @return true: done
  ///         false: the sink name cannot found
  bool GetSinkStats(const std::string& sink_name, LogStats& stats) const;

  /// @brief get the log module version
  ///
  /// @param[out] major: the version major
  /// @param[out] minor: the version minor
  /// @param[out] patch: the version patch
  /// @param[out] suffix: the suffix string
  ///
  void GetVersion(int& major,
                  int& minor,
                  int& patch,
                  std::string& suffix) const;

  /// @brief get string of the log module version
  ///
  /// @return the version string
  std::string GetVersionString(void) const;

  /// @brief output message through the log module error output
  ///        can be used inside hlog_init_by_user() to output info
  ///
  /// @param[in] file: the file name
  /// @param[in] line: the file line
  /// @param[in] fmt: the message format

  void ErrorOutput(const char* file, int line, const char* fmt, ...);

  /// @brief set blocking log mode
  ///
  /// @param[in] blocking: blocking mode
  ///
  void SetBlockingMode(bool blocking = true);

  /// @brief get the block mode
  ///
  /// @return the current blocking mode setting
  bool GetBlockingMode(void) const;

  /// @brief create a new sink instance
  ///
  /// @param[in] sink_type: the type of sink to be created
  /// @param[in] sink_name: the name for the created sink
  ///
  /// @return true: done
  ///         false: failure, check the errorno

  bool CreateSink(const std::string& sink_type, const std::string& sink_name);

  /// @brief set the binding sink for a module
  ///
  /// @param[in] module_name: the specific module name
  /// @param[in] sink_name: the sink name
  ///
  /// @return true: done
  ///         false: no sink with the sink_name or
  ///                the module does not exist and
  ///                failed to create new module

  bool SetModuleSink(const std::string& module_name,
                     const std::string& sink_name);

#define FlushBufferedLog() SendFlushMarker(__FILE__, __LINE__)
  /// @brief send a marker log to  flush all logs bufferred before the marker
  ///        when the function returns, flush done
  ///
  /// @param[in] file: the file name to call the function
  /// @param[in] line: the line to call the function

  void SendFlushMarker(const char* file, int line);

  /// @brief set the un-buffered log level
  ///
  /// @param[in] level: logs with equal or higher level is un-buffered
  ///
  /// @note
  ///     when a un-buffered log saved, all logs before the log are flushed
  ///     too
  ///
  void SetUnbufferedLevel(LogLevel level);

  /// @brief quit threading mode
  ///    all backend working thread will quit, so that the log interface will
  ///    call sink log function directly
  ///    it is necessary for windows app, as the return from main()will kill
  ///    other threads
  ///
  ///     when the function returns, all buffered logs are flushed
  ///
  /// @note
  ///     after this function, log will be sent to sink directly

  void QuitThreadingMode(void);

  /// @brief progress exit() when a critical log is sent
  ///
  /// @param[in] exit_code: the exit code when the process exists
  /// @param[in] the callback function before exit(). If return false,
  ///              the progress won't exit

  void EnableExitOnCritical(int exit_code = CRITICAL_LOG_EXIT_CODE,
                            const UserExitFunc& user_exit_func = {});

  /// @brief notify the log module, the progress is going to exit execution
  /// abnormally
  ///
  /// @note
  ///      it is only necessary in multiple threading progress, one threads
  ///      called exit() to quit early while other threads may still print
  ///      logs
  ///

  static void NotifyAbnormalExit(void);

  /// @brief mark the init procedure has done, so that some config API
  ///        SetAppName()/SetLogPrefixFormat() will fail
  ///

  void InitDone(void);

  /**
   * @brief Set the Global Level of hlog
   *
   * @param level global log level
   *
   * @note default global level is log_never, means that the global level is
   * disabled, the log will be print according to the level of each module. When
   * the global level is enabled(set the global level to other value), log print
   * will only be controled by the global log level. Disable the global log
   * level by setting it back to log_never.
   *
   */
  inline static void SetGlobalLevel(LogLevel level) { global_level_ = level; }

  /**
   * @brief Get the Global Level of hlog
   * 
   * @return global log level
   *
   */
  inline static LogLevel GetGlobalLevel() { return global_level_; }
  /// @brief get the log context instance
  LogContext* GetContext(void);

  bool EnableCmdControl(bool enable);

  bool SetConfigFromFile(const std::string &config_file);

  /// constructor & destructor
  HobotLog(const HobotLog&) = delete;
  HobotLog(HobotLog&&) = delete;

  HobotLog(void);
  ~HobotLog(void);

  template<typename... Args>
  bool SetCustomFileNameFormat(const std::string& format, Args... args) {
    std::vector<std::string> arg_string;
    std::vector<ArgFunc> arg_func;
    ProcessArgs(arg_string, arg_func, args...);
    return SetCustomFileNameFormat(arg_string, arg_func, format);
  }

  static bool SetMaxPayloadSize(int max_payload_size);

  /// if in hlog_init_by_user()?
  static bool InUserInitHook(void);

  /// the singleton interface
  static HobotLog* Instance(void);
  static void Release(void);

  inline static LogLevel GetGlobalMinLevel() {
    return global_min_module_level_;
  }

 private:
  std::size_t CountSubStr(const std::string &str, const std::string &sub_str);


  template<typename T>
  typename std::enable_if<std::is_same<T, ArgFunc>::value ||
                          std::is_same<T, ArgFuncPtr>::value>::type
  SaveArgsToVector(std::vector<std::string> &arg_string,
                         std::vector<ArgFunc> &arg_func,
                         T arg) {
    (void)arg_string;
    arg_func.push_back(arg);
  }

  template<typename T>
  typename std::enable_if<std::is_same<T, std::string>::value ||
                          std::is_same<T, const char*>::value ||
                          std::is_same<T, char*>::value>::type
  SaveArgsToVector(std::vector<std::string> &arg_string,
                   std::vector<ArgFunc> &arg_func,
                   T arg) {
    (void)arg_func;
    arg_string.push_back(arg);
  }

  template<typename T>
  typename std::enable_if<!std::is_same<T, std::string>::value &&
                          !std::is_same<T, const char*>::value &&
                          !std::is_same<T, char*>::value &&
                          !std::is_same<T, ArgFuncPtr>::value &&
                          !std::is_same<T, ArgFunc>::value>::type
  SaveArgsToVector(std::vector<std::string> &, std::vector<ArgFunc> &, T) {}

  void ProcessArgs(std::vector<std::string> &, std::vector<ArgFunc> &) {}

  template<typename T, typename... Args>
  void ProcessArgs(std::vector<std::string> &arg_str,
                   std::vector<ArgFunc> &arg_func, T first, Args... args) {
    SaveArgsToVector(arg_str, arg_func, first);
    ProcessArgs(arg_str, arg_func, args...);
  }

  bool SetCustomFileNameFormat(const std::vector<std::string>& arg_str,
                               const std::vector<ArgFunc>& arg_func,
                               const std::string& format);

 private:
  /// the real worker
  LogContext* log_context_;
  static LogLevel global_min_module_level_;
  static LogLevel global_level_;
};

/// HobotLogInit

class HLOG_PUBLIC HobotLogInit {
 public:
  HobotLogInit();
  ~HobotLogInit();

 private:
  /// the counter to control init procedure
  static std::atomic<int32_t> counter_;
};

/// the stub init instance
static HobotLogInit hlog_log_init_stub;

}  // namespace hlog
}  // namespace hobot

#ifdef _WIN32
#ifndef HLOG_BUILDING_LIB
#ifndef HLOG_BUILDING_DLL
/// for hlog dll client
#define HLOG_INIT_API HLOG_EXPORT
#endif
#endif
#endif

#ifndef HLOG_INIT_API
#define HLOG_INIT_API
#endif

/// @brief  the hook for user to init the log library
///
/// @param[in] logger: the pointer of the Log instance
///
/// @return  the init function result: true or false
///
/// @note
///   user can choose to implement the function to do some initialization work
///   it is executed in single thread and *HobotLog::Instance()* cannot be
///   called in the function, use the hobot_log pointer passed
///
///   When return false, an error message will be printed in log module error
///   output

extern "C" {
bool HLOG_INIT_API hlog_init_by_user(hobot::hlog::HobotLog* hobot_log);
}

#endif  // HLOG_HOBOT_LOG_H_
