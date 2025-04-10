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

#ifndef HLOG_COMPILER_H_
#define HLOG_COMPILER_H_

#if defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#define HLOG_EXPORT __attribute__((dllexport))
#define HLOG_IMPORT __attribute__((dllimport))
#else
#define HLOG_EXPORT __declspec(dllexport)
#define HLOG_IMPORT __declspec(dllimport)
#endif  // __GNUC__

#ifdef HLOG_BUILDING_LIB
#define HLOG_PUBLIC
#else
#ifdef HLOG_BUILDING_DLL
#define HLOG_PUBLIC HLOG_EXPORT
#else
#define HLOG_PUBLIC HLOG_IMPORT
#endif
#endif  // HLOG_BUILDING_LIB

#define HLOG_LOCAL

// STL Export warning
#pragma warning(disable : 4251)

#else  // UNIX

#ifdef HLOG_BUILDING_LIB
#define HLOG_PUBLIC
#else
#define HLOG_PUBLIC __attribute__((visibility("default")))
#endif

#define HLOG_LOCAL __attribute__((visibility("hidden")))

#endif

#ifdef HLOG_BUILDING_UTEST
#define UTEST_HLOG_PUBLIC HLOG_PUBLIC
#else
#define UTEST_HLOG_PUBLIC
#endif

#if defined _WIN32 && defined _MSC_VER
#define HLOG_MSVC_SUPPRESS_WARNING_PUSH __pragma(warning(push))
#define HLOG_MSVC_SUPPRESS_WARNING(w) __pragma(warning(disable : w))
#define HLOG_MSVC_SUPPRESS_WARNING_POP __pragma(warning(pop))
#define HLOG_MSVC_SUPPRESS_WARNING_WITH_PUSH(w) \
    HLOG_MSVC_SUPPRESS_WARNING_PUSH HLOG_MSVC_SUPPRESS_WARNING(w)
#else
#define HLOG_MSVC_SUPPRESS_WARNING_PUSH
#define HLOG_MSVC_SUPPRESS_WARNING(w)
#define HLOG_MSVC_SUPPRESS_WARNING_POP
#define HLOG_MSVC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif

#endif  // HLOG_COMPILER_H_
