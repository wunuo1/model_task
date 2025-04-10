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

#ifndef HLOGC_LOG_CTL_H_
#define HLOGC_LOG_CTL_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "hlog/compiler.h"
#include "log_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// set/get log level
#define HLOGC_SET_LOGLEVEL(level) HLOGCM_SET_LOGLEVEL(NULL, level)
#define HLOGCM_SET_LOGLEVEL(module_name, level) SetLogLevel(module_name, level)
#define HLOGC_GET_LOGLEVEL() HLOGCM_GET_LOGLEVEL(NULL)
#define HLOGCM_GET_LOGLEVEL(module_name) GetLogLevel(module_name)

/**
 * @brief set the application name
 *
 * @param[in] app_name:   the application name
 *
 * @return true: done
 *         false: called after InitDone() is called
 *
 */
bool HLOG_PUBLIC SetAppName(const char* app_name);

/**
 * @brief get the application name
 *
 * @param[out] name_buf the buffer to store the app name
 * @param[in] buf_len length of the name buffer
 *
 * @return true: done
 *         false: input ptr is NULL
 *
 */
bool HLOG_PUBLIC GetAppName(char* name_buf, int buf_len);

/**
 * @brief set the log level of the module with module_name
 *
 * @param[in] module_name: if NULL, the "default" module
 * @param[in] level:        the new log level
 *
 * @return true:   done
 *         false:  level is outof range
 *                 or cannot create new module with module_name
 *
 */
bool HLOG_PUBLIC SetLogLevel(const char* module_name, LogLevel level);

/**
 * @brief get the log level of the module with module_name
 *
 * @param[in] module_name: if NULL, the "default"  module
 *
 * @return the log level
 *
 * @note if no such module exists, log_never will be returned
 *
 */
LogLevel HLOG_PUBLIC GetLogLevel(const char* module_name);

/**
 * @brief check if the log level enabled in a module
 *
 * @param[in] module_name: if NULL, the "default" module
 * @param[in] level:       the log level
 *
 * @return    true:  log enabled for the target level
 *            false: log is disabled for the target level, or
 *                   the module does not exist and
 *                   failed to create new module
 *
 */
bool HLOG_PUBLIC LogLevelEnabled(const char* module_name, LogLevel level);

/**
 * @brief set the log prefix format, such as the position of pid,app_id,..
 *
 *        supported token are:
 *         %P  process id             "1232"
 *         %t  Thread id              "1232"
 *         %n  Application name       "nginx"
 *         %k  Module name            "communication"
 *         %K  Mono ts counter        "123409"
 *         %L  Short log level        "D","I"
 *         %l  Long log level         "debug","info"
 *         %s  Basename of source file "a.cpp"
 *         %#  Source line             "123"
 *         %m  Month 01-12             "11"
 *         %d  Day   01-31             "30"
 *         %H  Hours 00-23             "23"
 *         %M  Minutes 00-59           "59"
 *         %S  Seconds 00-59           "59"
 *         %e  Millisecond 000-999     "678"
 *         %E  Microsecond xxxxxx      "002030"
 *
 * @param[in] format_str: the format string
 *
 * @return  true: done
 *          false: format_str includes unsupported token
 *                 or called after InitDone() is called
 */
bool HLOG_PUBLIC SetLogPrefixFormat(const char* format_str);

/**
 * @brief get the log format string
 *
 * @param[in] fmt_str format string buffer
 * @param[in] fmt_str_len format string buffer length
 *
 * @return true: done
 *         false: format string buffer is NULL or it is too small
 */
bool HLOG_PUBLIC GetLogPrefixFormat(char* fmt_str, int fmt_str_len);

/**
 * fast-path for console sink and file sink
 *
 * @brief set the config arguments for file sink.
 *        after setting, file sink is working
 *
 * @param[in] fs_config: the configuration of file sink
 * @param[in] sink_name: the target file-releated sink name, NULL means
 *                        the default one "file"
 * @param[in] enable: automatically enable the file sink if setting is ok
 *
 * @return true: done
 *         false: rotate_number or max_size is wrong,
 *                or file_name cannot be openned
 *                or target sink does not exist
 * @note
 *     default max_size is 8192 M, it can be changed by SetMaxFileSize(),
 *     the max_size of struct FileSinkConfig can not bigger than the global
 *     max size, which can be set by SetMaxFileSize()
 *
 *     if the sink is enabled, this function will disable sink first and
 *      enable it after setting
 */
bool HLOG_PUBLIC SetFileSinkConfig(const FileSinkConfig* fs_config,
                                   const char* sink_name,
                                   bool enable);

/**
 * @brief get the config arguments for file sink
 *
 * @param[in] sink_name: the target file-releated sink name, NULL means
 * the default one "file"
 * @param[out] fs_config: file sink config
 *
 * @return  no file sink with sink_name or fs_config is NULL
 *
 * @note the file sink config get from GetFileSinkConfig should be freed by
 * FreeFileSinkConfig
 */
bool HLOG_PUBLIC GetFileSinkConfig(const char* sink_name,
                                   FileSinkConfig* fs_config);

/**
 * @brief free the file sink config
 *
 * @param[in] fs_config the file sink config
 *
 * @note the file sink config get from GetFileSinkConfig should be freed by
 * FreeFileSinkConfig
 *
 */
void HLOG_PUBLIC FreeFileSinkConfig(FileSinkConfig fs_config);

/**
 * @breif set the max log file size, the upbound of the max_size in
 * SetFileSinkConfig
 *
 * @pararm[in]: max_size: max file size permitted for a single file sink,
 * in Mbyte. Permitted range: 1<= max_size<=(1<<20)
 *
 * @return true: done
 *         false: max_size is out of range
 */
bool HLOG_PUBLIC SetMaxFileSize(uint64_t max_size);

/**
 * @breif get the max log file size, the upbound of the max_size in
 * SetFileSinkConfig
 *
 * @return the max file size in Mbyte
 *
 */
uint64_t HLOG_PUBLIC GetMaxFileSize(void);

/**
 * @brief get the last error number
 *        each thread has its own error number
 *
 * @return the error number
 */
uint32_t HLOG_PUBLIC GetErrorNo(void);

/**
 * @brief get the last error message, per thread
 *
 * @param[in] msg_buf message buffer
 * @param[in] buf_len length of message buffer
 *
 * @return true: done
 *         false: message buffer is NULL or it is too small
 */
bool HLOG_PUBLIC GetErrorMsg(char* msg_buf, int buf_len);

/**
 * @brief set a module's log rate limit
 *        if log bursted more than burst_number in  1 second
 *         further logs will be dropped until the interval time or number
 *         is over
 *
 * @param[in] module_name: the module_name
 * @param[in] limit: limit of log out put rate
 *
 * @return true: done
 *         false: cannot create new module with module_name due to  module
 *         number limit
 * @note
 *      if burst_number <=0 or interval_in_millisecond <=0, this rate limit
 *      is disabled.
 *        this interface can create new log module with module_name
 */
bool HLOG_PUBLIC SetLogRateLimit(const char* module_name,
                                 const RateLimit* limit);

/**
 * @brief get a module's log rate limit setting
 *
 * @param[in]  module_name: the module_name
 * @param[out] limit: the rate limit of the target module
 *
 * @return  return limit: done
 *          NULL: no such module, or rate limit is not enabled, not data
 *          retrievaled, burst_number or interval_in_millisecond or
 *          interval_in_number is NULL
 */
bool HLOG_PUBLIC GetLogRateLimit(const char* module_name, RateLimit* limit);

/**
 * @brief set the rate limit on single log generation statement
 *        if a single log statement generated burst_number logs in 1 second
 *         further logs will be dropped until interval_in_number logs
 *         dropped or interval_in_millisecond passed
 *
 * @param[in] limit: the rate limit on single log output
 *
 * @note
 *       if burst_number <=0 or interval_in_millisecond <=0, this rate
 *       limit is disabled
 */
bool HLOG_PUBLIC SetRateLimitOnSingleLog(const RateLimit* limit);

/**
 * @brief get the rate limit on single log generation statement
 *
 * @param[out] limit: log limit on single log
 *
 * @return true: done
 *         false: rate limit is not enabled, and not date retrievaled
 */
bool HLOG_PUBLIC GetRateLimitOnSingleLog(RateLimit* limit);

/**
 * @brief set the file to save the log module error output
 *        if not set, log module error will be printed in console
 *
 * @param[in] file_name file to save the hlogc internal error output
 *
 * @return true: done
 *         false: file cannot create or access the target file
 *                or the output file has been set already
 */
bool HLOG_PUBLIC SetErrorOutputFile(const char* file_name);

/**
 * @brief get the log module error output file
 *
 * @param[in] file_name: file name buffer of error output file
 * @param[in] name_len: length of file name buffer
 *
 * @return the output file, maybe empty string if no file is set
 */
bool HLOG_PUBLIC GetErrorOutputFile(char* file_name, int name_len);

/**
 * @brief get the module list
 *
 * @param[out] module_list: list of all module
 * @param[out] module_num: number of modules
 *
 * @return true: done
 *         false: input ptr is NULL
 */
bool HLOG_PUBLIC GetModuleList(ModuleList* module_list, int* module_num);

/**
 * @brief free the module list
 *
 * @param[in] module_list: list of all module
 * @param[in] module_num: number of modules
 *
 */
void HLOG_PUBLIC FreeModuleList(ModuleList module_list, int module_num);

/**
 * @brief get the sink list
 *
 * @param[out] sink_list: list of all sink
 * @param[out] sink_num: number of sinks
 *
 * @return true: done
 *         false: input ptr is NULL
 */
bool HLOG_PUBLIC GetSinkList(SinkList* sink_list, int* sink_num);

/**
 * @brief free the sink list
 *
 * @param[in] sink_list: list of all sink
 * @param[in] sink_num: number of sinks
 *
 */
void HLOG_PUBLIC FreeSinkList(SinkList sink_list, int sink_num);

/**
 * @brief config the sink
 *
 * @param[in] sink_name: the sink name
 * @param[in] config: the config of sink
 *
 * @return true: done
 *         false: sink name cannot be found
 *                or config item is wrong or sink is already enabled
 */
bool HLOG_PUBLIC SetSinkConfig(const char* sink_name, const SinkConfig* config);

/**
 * @brief get the configs of a sink
 *
 * @param[in] sink_name: the sink name
 * @param[out] configs: all configs of the target sink
 * @param[out] config_num: the number of sink configs
 *
 * @return the config map of config items
 */
bool HLOG_PUBLIC GetSinkConfig(const char* sink_name,
                               SinkConfigList* configs,
                               int* config_num);

/**
 * @brief free the sink config
 *
 * @param[in] configs config to be free
 * @param[in] config_num number of config to be freed
 *
 * @note the sink config list get from GetSinkConfig should be freed by
 * FreeSinkConfig
 *
 */
void HLOG_PUBLIC FreeSinkConfig(SinkConfigList sink_configs, int config_num);

/**
 * @brief toggle sink is enable or disable
 *
 * @param[in] sink_name: the sink name
 * @param[in] enable_status:   the sink enable target status
 *
 * @return true: done
 *         fasle: the sink name cannot found
 */
bool HLOG_PUBLIC ToggleSinkEnable(const char* sink_name, bool enable_status);

/**
 * @brief check if sink is enabled
 *
 * @param[in] sink_name: the sink name
 *
 * @return the enable status of the sink
 *         if sink_name is not found, also return false
 */
bool HLOG_PUBLIC IsSinkEnabled(const char* sink_name);

/**
 * @brief set the log level of a sink
 *
 * @param[in] sink_name: the sink name
 * @param[in] level: the log level
 *
 * @return true: done
 *         false: sink_name wrong or level wrong
 *                or the concrete sink denied the setting
 */
bool HLOG_PUBLIC SetSinkLogLevel(const char* sink_name, LogLevel level);

/**
 * @brief get the log level of a sink
 *
 * @param[in] sink_name: the sink name
 *
 * @return the log level of the sink
 *         log_never if no such sink exists
 */
LogLevel HLOG_PUBLIC GetSinkLogLevel(const char* sink_name);

/**
 * @brief get the log module version
 *
 * @param[out] major major version number
 * @param[out] minor minor version number
 * @param[out] patch patch version number
 * @param[in] suffix_buf buffer to store suffix info of hlogc
 * @param[in] buf_len length of suffix buffer
 *
 * @return true: done
 *         false: input ptr is NULL
 *
 */
bool HLOG_PUBLIC GetHlogVersion(
    int* major, int* minor, int* patch, char* suffix_buf, int buf_len);

/**
 * @brief get string of the log module version
 *
 * @param[in] version_buf buffer to store version info of hlogc
 * @param[in] buf_len length of version_buf
 *
 * @return true: done
 *         false: input ptr is NULL
 *
 */
bool HLOG_PUBLIC GetHlogVersionString(char* version_buf, int buf_len);

/**
 * @brief set blocking log mode
 *
 * @param[in] blocking: blocking mode
 *
 */
void HLOG_PUBLIC SetBlockingMode(bool blocking);

/**
 * @brief get the block mode
 *
 * @return the current blocking mode setting
 *
 */
bool HLOG_PUBLIC GetBlockingMode(void);

/**
 * @brief create a new sink instance
 *
 * @param[in] sink_type: the type of sink to be created
 * @param[in] sink_name: the name for the created sink
 *
 * @return true: done
 *         false: failure, check the errorno
 *
 */
bool HLOG_PUBLIC CreateSink(const char* sink_type, const char* sink_name);

/**
 * @brief set the binding sink for a module
 *
 * @param[in] module_name: the specific module name
 * @param[in] sink_name: the sink name
 *
 * @return true: done
 *         false: no sink with the sink_name or
 *                the module does not exist and
 *                failed to create new module
 *
 */
bool HLOG_PUBLIC SetModuleSink(const char* module_name, const char* sink_name);

#define FlushBufferedLog() SendFlushMarker(__FILE__, __LINE__)

/**
 * @brief send a marker log to  flush all logs bufferred before the marker
 *        when the function returns, flush done
 *
 * @param[in] file: the file name to call the function
 * @param[in] line: the line to call the function
 *
 */
void HLOG_PUBLIC SendFlushMarker(const char* file, int line);

/**
 * @brief set the un-buffered log level
 *
 * @param[in] level: logs with equal or higher level is un-buffered
 *
 * @note
 *     when a un-buffered log saved, all logs before the log are flushed
 *     too
 *
 */
void HLOG_PUBLIC SetUnbufferedLevel(LogLevel level);

/**
 * @brief quit threading mode all backend working thread will quit, so that the
 * log interface will call sink log function directly it is necessary for
 * windows app, as the return from main()will kill other threads
 *
 * when the function returns, all buffered logs are flushed
 *
 * @note
 *     after this function, log will be sent to sink directly
 */
void HLOG_PUBLIC QuitThreadingMode(void);

/**
 * @brief mark the init procedure has done, so that some config API
 *        SetAppName()/SetLogPrefixFormat() will fail
 *
 */
void HLOG_PUBLIC InitDone(void);

#ifdef __cplusplus
}
#endif

#endif  // HLOGC_LOG_CTL_H_
