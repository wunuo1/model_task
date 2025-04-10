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

#ifndef HLOG_LOG_ERROR_H_
#define HLOG_LOG_ERROR_H_

#include <string>

#include "hlog/compiler.h"

#define SetLastErrorNo(error_no) \
  SetLastErrorNoWithPos(__FILE__, __LINE__, error_no)

namespace hobot {
namespace hlog {

enum LogErrorNo {
  HB_ENOERROR = 0,
  HB_E2BIG,
  HB_EINVAL,
  HB_EEXIST,
  HB_ENOENT,
  HB_EACCESS,
  // new
  HB_ENORESOURCE,
  HB_ENODATA,
  HB_ENOMEM,
  HB_EAGAIN,
  HB_EBUSY,

  HB_ENOTSUP,
  HB_EPERM,
  HB_ETIMEOUT,
  HB_ECANCELED,
  HB_EINTERNAL,
  HB_ENOTLOGERROR,
  HB_EBLOCKSIZEINVALID
};

HLOG_PUBLIC void ResetErrorNo(void);

HLOG_PUBLIC uint32_t GetLastErrorNo(void);

HLOG_PUBLIC void SetLastErrorNoWithPos(const char* file,
                                       int line,
                                       LogErrorNo error_no);

HLOG_PUBLIC std::string GetLastErrorMsg(void);

HLOG_PUBLIC void SetLastErrorMsg(const std::string& msg);

}  // namespace hlog
}  // namespace hobot

#endif  // HLOG_LOG_ERROR_H_
