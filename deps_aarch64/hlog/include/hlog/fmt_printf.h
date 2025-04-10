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

#include <fmt/core.h>

#include <exception>
#include <memory>
#include <utility>

#include "hlog/log_types.h"

#pragma once

namespace hobot {
namespace hlog {

class Logger;

class PrintBuffer : public fmt::detail::buffer<char> {
 public:
  PrintBuffer(void) {
    overflowed_ = false;
    logger_ = nullptr;
  }

  void SetBuffer(char* buf, size_t capacity) { set(buf, capacity); }

  /// the second grow will throw exception
  void grow(size_t capacity) override {
    if (overflowed_) throw std::overflow_error("over sized");

    overflowed_ = true;
    (void)capacity;
  }

  Logger* GetLogger(void) const { return logger_; }

  void GrabLogMsg(LogMsgPtr& log_msg) { log_msg = std::move(log_msg_); }

  void SetLogger(Logger* logger) { logger_ = logger; }
  void SetLogMsg(LogMsgPtr&& log_msg) { log_msg_ = std::move(log_msg); }

 private:
  bool overflowed_;
  Logger* logger_;
  LogMsgPtr log_msg_;
};

}  // namespace hlog
}  // namespace hobot
