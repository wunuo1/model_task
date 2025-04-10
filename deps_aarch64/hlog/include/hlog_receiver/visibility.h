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

#if defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#define HLRECEIVER_EXPORT __attribute__((dllexport))
#define HLRECEIVER_IMPORT __attribute__((dllimport))
#else
#define HLRECEIVER_EXPORT __declspec(dllexport)
#define HLRECEIVER_IMPORT __declspec(dllimport)
#endif

#ifdef HLRECEIVER_BUILDING_LIB
#define HLRECEIVER_PUBLIC
#else
#ifdef HLRECEIVER_BUILDING_DLL
#define HLRECEIVER_PUBLIC HLRECEIVER_EXPORT
#else
#define HLRECEIVER_PUBLIC HLRECEIVER_IMPORT
#endif
#endif

#define HLRECEIVER_LOCAL

// STL Export warning
#pragma warning(disable : 4251)

#else  // UNIX

#ifdef HLRECEIVER_BUILDING_LIB
#define HLRECEIVER_PUBLIC
#else
#define HLRECEIVER_PUBLIC __attribute__((visibility("default")))
#endif

#define HLRECEIVER_LOCAL __attribute__((visibility("hidden")))

#endif
