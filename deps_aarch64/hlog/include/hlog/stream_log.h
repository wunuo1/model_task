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
// See Apache v2.0 for more details

#ifndef HLOG_STREAM_LOG_H_
#define HLOG_STREAM_LOG_H_

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "hlog/log_types.h"

namespace hobot {
namespace hlog {
/// forward class declaration
class Logger;

/// LogStreamBuf: the buf for ostream
class HLOG_PUBLIC LogStreamBuf : public std::streambuf {
 public:
  /// constructor, setting the buf and the len
  void SetBuf(char* buf, int len) { setp(buf, buf + len); }

  /// return the pointer to the last char
  char* pptr(void) { return std::streambuf::pptr(); }
};

/// StreamLog: the stream output class
class HLOG_PUBLIC StreamLog : public std::ostream {
 public:
  StreamLog(const char* module_name,
            uint64_t module_id,
            LogLevel level,
            const char* file,
            int line,
            uint64_t pos_id);

  ~StreamLog(void);

 private:
  /// the stream buffer
  LogStreamBuf stream_buf_;

  /// the backend logger
  Logger* logger_;

  /// the owned log_msg_
  LogMsgPtr log_msg_;

  char dummy_buf_[2];
};

class HLOG_PUBLIC StreamAbsorber {
 public:
  bool operator&&(const std::ostream& log_stream) {
    (void)log_stream;
    return true;
  }
};

}  // namespace hlog
}  // namespace hobot

#endif  // HLOG_STREAM_LOG_H_
