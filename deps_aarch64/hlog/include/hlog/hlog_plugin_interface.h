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

#ifndef HLOG_HLOG_PLUGIN_INTERFACE_H_
#define HLOG_HLOG_PLUGIN_INTERFACE_H_

#include "hlog/compiler.h"

#ifndef STR2
#define STR2(a) #a
#endif

#ifndef STR
#define STR(a) STR2(a)
#endif

#define HLOG_PLUGIN_INIT_FUNC hlog_plugin_init
#define HLOG_PLUGIN_RELEASE_FUNC hlog_plugin_release
#define HLOG_PLUGIN_GET_SINK_TYPE_FUNC hlog_plugin_get_sink_type
#define HLOG_PLUGIN_CREATE_SINK_FUNC hlog_plugin_create_sink
#define HLOG_PLUGIN_DESTROY_SINK_FUNC hlog_plugin_destroy_sink

#define HLOG_PLUGIN_INIT_FUNC_NAME STR(HLOG_PLUGIN_INIT_FUNC)
#define HLOG_PLUGIN_RELEASE_FUNC_NAME STR(HLOG_PLUGIN_RELEASE_FUNC)
#define HLOG_PLUGIN_GET_SINK_TYPE_FUNC_NAME STR(HLOG_PLUGIN_GET_SINK_TYPE_FUNC)
#define HLOG_PLUGIN_CREATE_SINK_FUNC_NAME STR(HLOG_PLUGIN_CREATE_SINK_FUNC)
#define HLOG_PLUGIN_DESTROY_SINK_FUNC_NAME STR(HLOG_PLUGIN_DESTROY_SINK_FUNC)

#ifdef __cplusplus
extern "C" {
#endif
/// each hlog sink plugin MUST implement those four C interfaces

typedef int (*hlog_plugin_init_t)(void *);
typedef void (*hlog_plugin_release_t)(void);
typedef const char *(*hlog_plugin_get_sink_type_t)(void);
typedef void *(*hlog_plugin_create_sink_t)(const char *);
typedef void (*hlog_plugin_destroy_sink_t)(void *);

/// init the plugin, return 0 on success and -1 on failure
HLOG_PUBLIC int HLOG_PLUGIN_INIT_FUNC(void *);

HLOG_PUBLIC void HLOG_PLUGIN_RELEASE_FUNC(void);

/// indeed returned the BaseSink object
HLOG_PUBLIC const char *HLOG_PLUGIN_GET_SINK_TYPE_FUNC(void);

/// indeed returned the BaseSink object
HLOG_PUBLIC void *HLOG_PLUGIN_CREATE_SINK_FUNC(const char *);

/// destory the returned BaseSink object
HLOG_PUBLIC void HLOG_PLUGIN_DESTROY_SINK_FUNC(void *);

#ifdef __cplusplus
}
#endif

#endif  // HLOG_HLOG_PLUGIN_INTERFACE_H_
