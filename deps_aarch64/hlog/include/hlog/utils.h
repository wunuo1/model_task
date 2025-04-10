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

#ifndef HLOG_UTILS_H_
#define HLOG_UTILS_H_

#include <chrono>
#include <cstring>
#include <ctime>
#include <memory>
#include <string>
#include <vector>

#ifdef __GNUC__
#include <cxxabi.h>
#endif

#include "hlog/log_types.h"

namespace hobot {
namespace hlog {
namespace utils {

#define MAGIC_HASH_FUSE_NUMBER1 0xdeadbeaf
#define MAGIC_HASH_FUSE_NUMBER2 0x5a5a5a5a

// constexpr, for log print
template <typename T>
constexpr uint32_t name_hash(uint32_t a, T b) {
  return b ^ (a * b);
}

template <std::size_t N>
constexpr uint32_t gen_first_hash(const char (&s)[N], int n = 0) {
  return n == N - 2 ? s[n]
                    : name_hash(s[n], gen_first_hash(s, n + 1)) ^
                          MAGIC_HASH_FUSE_NUMBER1;
}

template <std::size_t N>
constexpr uint32_t gen_second_hash(const char (&s)[N], int n = N - 2) {
  return n == 0 ? s[n]
                : name_hash(s[n], gen_second_hash(s, n - 1)) ^
                      MAGIC_HASH_FUSE_NUMBER2;
}

/// @brief generate a module id from a string literal
///
/// @return the generated uint64_t id

template <std::size_t N>
constexpr uint64_t gen_module_id(const char (&s)[N]) {
  return (((uint64_t)gen_first_hash(s) << 32) + gen_second_hash(s));
}

template <>
constexpr uint64_t gen_module_id(const char (&s)[2]) {
  return ((uint64_t)MAGIC_HASH_FUSE_NUMBER2) * s[0];
}

template <>
constexpr uint64_t gen_module_id(const char (&s)[1]) {
  return MAGIC_HASH_FUSE_NUMBER1 + s[0];
}

constexpr uint64_t gen_module_id(std::nullptr_t) { return 0; }

//
// gen file hash id
//
template <std::size_t N>
constexpr uint64_t gen_file_pos_id(const char (&s)[N], int line) {
  return gen_module_id(s) * line;
}

static inline uint32_t get_first_hash(const char* s, int len, int n) {
  if (n == len - 1) return s[n];

  return name_hash(s[n], get_first_hash(s, len, n + 1)) ^
         MAGIC_HASH_FUSE_NUMBER1;
}

static inline uint32_t get_second_hash(const char* s, int len, int n) {
  if (n == 0) return s[n];

  return name_hash(s[n], get_second_hash(s, len, n - 1)) ^
         MAGIC_HASH_FUSE_NUMBER2;
}

/// @brief get module id from a string
///
/// @return the module id: uint64_t

static inline uint64_t get_module_id(const char* module_name) {
  if (module_name == nullptr) return 0;

  // special case for MAGIC_DEFAULT_MODULE_NAME
  if (!strcmp(module_name, MAGIC_DEFAULT_MODULE_NAME)) return 0;

  int len = static_cast<int>(strlen(module_name));

  if (len == 0) return gen_module_id("");

  if (len == 1) return ((uint64_t)MAGIC_HASH_FUSE_NUMBER2) * module_name[0];

  uint64_t h0 = get_first_hash(module_name, len, 0);
  uint64_t h1 = get_second_hash(module_name, len, len - 1);

  return (h0 << 32) + h1;
}

/// @brief get the timestamp from wall time, in us
static inline int64_t get_sys_ts(void) {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

/// @brief get the timestamp from mono time, in us
static inline int64_t get_mono_ts(void) {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

/// @brief convert a system timestamp to wall time
static inline void get_localtime(int64_t sys_ts, std::tm& tm) {
  auto dr = std::chrono::duration<int64_t, std::micro>(sys_ts);
  auto tp = std::chrono::time_point<std::chrono::system_clock>(dr);
  std::time_t time_in_second = std::chrono::system_clock::to_time_t(tp);

#ifdef _WIN32
  ::localtime_s(&tm, &time_in_second);
#else
  ::localtime_r(&time_in_second, &tm);
#endif
}

static inline std::vector<std::string> split_string(const std::string& str,
                                                    char delimiter) {
  const char* buf = str.c_str();
  const char* p = strchr(buf, delimiter);
  std::vector<std::string> str_list;

  while (p != nullptr) {
    if (p != buf) str_list.push_back(std::string(buf, p - buf));

    buf = p + 1;
    p = strchr(buf, delimiter);
  }

  if (buf[0] != 0) str_list.push_back(std::string(buf));

  return str_list;
}

static inline void save_int(uint8_t* buf, int n) {
  // big endian save
  for (int i = 3; i >= 0; i--) {
    buf[i] = (n & 0xff);
    n = n >> 8;
  }
}

static inline void load_int(const uint8_t* buf, int& n) {
  n = 0;

  for (int i = 0; i < 4; i++) {
    n = (n << 8) + buf[i];
  }
}

}  // namespace utils

template <typename T>
static inline std::string nice_type_name(void) {
  typedef typename std::remove_reference<T>::type TO;
  char* demangled_name = nullptr;
  const char* type_name = typeid(TO).name();

#if defined(__GNUC__) && !defined(NO_CXA_DEMANGLE)
  demangled_name = abi::__cxa_demangle(type_name, nullptr, nullptr, nullptr);
#endif

  std::string s;

  if (demangled_name) {
    s = demangled_name;
    std::free(demangled_name);
  } else {
    s = type_name;
  }

  if (std::is_const<TO>::value) s += " const";
  if (std::is_volatile<TO>::value) s += " volatile";
  if (std::is_lvalue_reference<T>::value)
    s += "&";
  else if (std::is_rvalue_reference<T>::value)
    s += "&&";
  return s;
}

}  // namespace hlog
}  // namespace hobot

#endif  // HLOG_UTILS_H_
