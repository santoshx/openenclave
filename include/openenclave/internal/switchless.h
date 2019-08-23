// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef _OE_SWITCHLESS_H
#define _OE_SWITCHLESS_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>
#include <openenclave/internal/calls.h>

typedef uint64_t oe_thread_t;

typedef struct _host_worker_thread_context
{
    oe_call_host_function_args_t* call_arg;
    uint64_t sleep_time;
    bool stopping;
    oe_enclave_t* enclave;
    oe_result_t ocall_result;
} host_worker_thread_context;

typedef struct _oe_switchless_call_manager
{
    host_worker_thread_context* host_worker_contexts;
    oe_thread_t* host_worker_threads;
    uint32_t num_host_workers;
} oe_switchless_call_manager;

void oe_stop_switchless_manager(oe_enclave_t* enclave);

#endif /* _OE_SWITCHLESS_H */
