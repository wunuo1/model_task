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

#ifndef HLOG_BASE_SINK_H_
#define HLOG_BASE_SINK_H_

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "hlog/log_error.h"
#include "hlog/log_types.h"
#include "hlog/utils.h"

namespace hobot {
namespace hlog {

///
/// BaseSink: the base class of all sink
///

class LogContext;

class HLOG_PUBLIC BaseSink {
 public:
  /// config set function, receive two parameters:
  ///   first: the config_name
  ///   second: the config value
  using ConfigSetFunc =
      std::function<bool(const std::string&, const std::string&)>;

  /// config set function registry
  ///  first: config_name
  ///  second: the function to handle the item
  using ConfigFunSetRegistry = std::unordered_map<std::string, ConfigSetFunc>;

  /// config get function, receive two parameters:
  ///   first: the config_name
  ///   second: the config value
  using ConfigGetFunc = std::function<bool(const std::string&, std::string&)>;

  /// config get function registry
  ///  first: config_name
  ///  second: the function to handle the item
  using ConfigFunGetRegistry = std::unordered_map<std::string, ConfigGetFunc>;

  /// @brief register configuration set handler
  ///
  /// @param[in] config_name: config item name
  /// @param[in] func: func to handle config item
  ///
  /// @return  true: done
  ///          false: config_name already set
  bool RegConfigSetFunc(const std::string& config_name, ConfigSetFunc& func);

  /// @brief register configuration get handler
  ///
  /// @param[in] config_name: config item name
  /// @param[in] func: func to handle config item
  ///
  /// @return  true: done
  ///          false: config_name already set
  bool RegConfigGetFunc(const std::string& config_name, ConfigGetFunc& func);

  /// @brief log the message, MUST be MT-Safe
  ///
  /// @param[in] log_msg: the pointer to log message
  ///
  /// @return true: done
  ///         false: depended on subclass sink
  bool Log(std::shared_ptr<LogMsg>& log_msg);

  /// @brief save the log into the sink, MUST be MT-Safe
  ///
  /// @param[in] log_msg: the log message
  /// @param[in] addr: the data start address
  /// @param[in] len:  the data length
  ///
  /// @return true: done
  ///         false: depends on subclass impl
  virtual bool Save(std::shared_ptr<LogMsg>& log_msg) = 0;

  /// @brief get sink status
  ///
  /// @return true: sink enabled
  ///         false: sink disabled
  bool IsEnabled(void) const { return enabled_; }

  /// @brief toggle the enable status of sink
  ///
  /// @param[in] enabled: the new status
  ///
  /// @return true: set done
  ///         false: set failed
  bool ToggleEnable(bool enable_status) {
    if (enabled_ == enable_status) return true;

    if (enabled_) {
      // switched from enabled to disabled
      if (OnSinkDisable()) {
        enabled_ = false;
        return true;
      } else {
        return false;
      }
    }

    // switched from disabled to enabled
    if (!CheckConfig()) return false;

    if (!OnSinkEnable()) return false;

    enabled_ = true;
    return true;
  }

  /// @brief set the config item, the value must be converted to string
  ///
  /// @param[in] config_name: config item name
  /// @param[in] config_value: the value
  ///
  /// @return  true: done
  ///          false: depends on real function

  bool SetConfig(const std::string& config_name,
                 const std::string& config_value) {
    auto ir = set_registry_.find(config_name);

    if (ir == set_registry_.end()) {
      SetLastErrorNo(HB_ENOENT);
      return false;
    }

    auto f = ir->second;

    return f(config_name, config_value);
  }

  /// @brief get the stats of the sink
  ///
  /// @param[out] stats: the returned statistics data
  void GetStats(LogStats& stats) const { stats = stats_; }

  /// @brief return the configged items
  ///
  /// @return the config mapping

  SinkConfig GetConfig(void) {
    SinkConfig config;

    auto ir = get_registry_.begin();

    while (ir != get_registry_.end()) {
      auto func = ir->second;
      std::string value;

      if (func(ir->first, value)) {
        config[ir->first] = value;
      }
      ir++;
    }

    return config;
  }

  /// @brief set the log level of the sink
  ///
  /// @param[in] level: the log level
  ///
  /// @return true: done
  ///         false: it may happen on the concrete sink implementation,
  ///                as the sink denies app to change the loglevel

  virtual bool SetLogLevel(LogLevel level) {
    log_level_ = level;
    return true;
  }

  /// @brief get the log level of the sink
  ///
  /// @return current log level

  LogLevel GetLogLevel(void) const { return log_level_; }

  void ErrorOutput(const char* file, int line, const char* fmt, ...);

  void ErrorOutput(const std::string& err_msg);

  void TryFlush(void) {
    int64_t log_count = stats_.saved;

    if (last_flush_count_ < log_count) {
      Flush();
      last_flush_count_ = log_count;
    }
  }

  /// @brief flush log, make sure all internal buffered logs are saved
  virtual bool Flush(void) { return true; }

  /// @brief reserved for additional bytes in Sink, if necesssary
  virtual int GetReservedSize(void) const { return 0; }

  /// @brief reserved for future extenstion
  virtual bool FormatLogPrefix(LogMsg* log_msg) {
    (void)log_msg;
    SetLastErrorNo(HB_ENOTSUP);
    return false;
  }
  /// @brief set the prefix pattern
  virtual bool SetPrefixFormat(const std::string& fmt_str) {
    (void)fmt_str;
    SetLastErrorNo(HB_ENOTSUP);
    return false;
  }
  /// @brief if need to do format prefix
  virtual bool NeedFormatPrefix(void) { return false; }

  /// @brief return the sink name
  std::string GetName(void) const { return name_; }

  /// @brief return the sink type
  std::string GetType(void) const { return type_; }

  /// @brief set the sink type
  void SetType(const std::string &sink_type) { type_ = sink_type; }

  /// destructor
  virtual ~BaseSink(void) {}

 protected:
  /// constructor
  explicit BaseSink(const std::string& name);
  /// @brief initalize and allocate resource for sink to work
  ///
  /// @return true: done
  ///         false: depends on subclass impl
  virtual bool OnSinkEnable(void) = 0;

  /// @brief initalize and allocate resource for sink to work
  ///
  /// @return true: done
  ///         false: depends on subclass impl
  virtual bool OnSinkDisable(void) = 0;

  /// @brief check if all the necessary config are set correctly
  ///
  /// @return true: done
  ///         false: depends on subclass impl
  virtual bool CheckConfig(void) = 0;

  /// the statistics for the sink
  LogStats stats_;

  /// last correctly saved log ts
  int64_t last_success_ts_;

  int64_t last_failure_ts_;

  int64_t last_flush_count_;

  /// sink status
  bool enabled_;

  /// sink log level
  LogLevel log_level_;

  /// log context pointer
  LogContext* log_context_;

  std::string name_;
  std::string type_;

 private:
  ConfigFunSetRegistry set_registry_;
  ConfigFunGetRegistry get_registry_;

  bool ConfigSetWhiteList(const std::string& key, const std::string& value);

  bool ConfigGetWhiteList(const std::string& key, std::string& value);

  std::vector<uint64_t> module_id_list_;
  std::vector<std::string> white_list_;
};

}  // namespace hlog
}  // namespace hobot

#endif  // HLOG_BASE_SINK_H_
