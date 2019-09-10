// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "switchlesscalls.h"
#include <openenclave/enclave.h>
#include <openenclave/internal/atomic.h>
#include <openenclave/internal/raise.h>

// The number of host thread workers. Initialized by host through ECALL
static size_t _num_host_workers = 0;

// The array of host worker contexts. Initialized by host through ECALL
static host_worker_thread_context_t* _host_worker_contexts;

/*
**==============================================================================
**
** oe_is_switchless_initialized
**
** Return whether oe_handle_init_switchless has been called or not.
**
**==============================================================================
*/
bool oe_is_switchless_initialized()
{
    return _num_host_workers != 0;
}

/*
**==============================================================================
**
** oe_handle_init_switchless()
**
** Handle the OE_ECALL_INIT_SWITCHLESS from host.
**
**==============================================================================
*/
oe_result_t oe_handle_init_switchless(uint64_t arg_in)
{
    oe_result_t result = OE_OK;

    if (arg_in == 0)
        OE_RAISE(OE_INVALID_PARAMETER);

    oe_switchless_call_manager_t* manager =
        (oe_switchless_call_manager_t*)arg_in;
    oe_switchless_call_manager_t safe_manager = *manager;

    size_t contexts_size =
        sizeof(host_worker_thread_context_t) * safe_manager.num_host_workers;
    size_t threads_size = sizeof(oe_thread_t) * safe_manager.num_host_workers;

    // Ensure the switchless manager and its arrays are outside of enclave
    if (!oe_is_outside_enclave(manager, sizeof(oe_switchless_call_manager_t)) ||
        !oe_is_outside_enclave(
            safe_manager.host_worker_contexts, contexts_size) ||
        !oe_is_outside_enclave(
            safe_manager.host_worker_threads, threads_size) ||
        safe_manager.num_host_workers == 0)
    {
        OE_RAISE(OE_INVALID_PARAMETER);
    }

    // Copy the worker context array pointer and its size to avoid TOCTOU
    _num_host_workers = safe_manager.num_host_workers;
    _host_worker_contexts = safe_manager.host_worker_contexts;

done:
    return result;
}

/*
**==============================================================================
**
** oe_post_switchless_ocall()
**
**  Post the function call (wrapped in args) to a free host worker thread
**  by writing to its context.
**
**==============================================================================
*/
oe_result_t oe_post_switchless_ocall(oe_call_host_function_args_t* args)
{
    oe_result_t result = OE_UNEXPECTED;
    args->result = __OE_RESULT_MAX; // Means the call hasn't been processed.

    // Cycle through the worker contexts until we find a free worker.
    size_t tries = _num_host_workers;
    while (tries--)
    {
        if (_host_worker_contexts[tries].call_arg == NULL)
        {
            if (oe_atomic_compare_and_swap_ptr(
                    (void* volatile*)&_host_worker_contexts[tries].call_arg,
                    NULL,
                    args))
            {
                return OE_OK;
            }
        }
    }

    result = OE_CONTEXT_SWITCHLESS_OCALL_MISSED;

    return result;
}

/*
**==============================================================================
**
** oe_switchless_call_host_function()
**
**==============================================================================
*/

oe_result_t oe_switchless_call_host_function(
    size_t function_id,
    const void* input_buffer,
    size_t input_buffer_size,
    void* output_buffer,
    size_t output_buffer_size,
    size_t* output_bytes_written)
{
    return oe_call_host_function_by_table_id(
        OE_UINT64_MAX,
        function_id,
        input_buffer,
        input_buffer_size,
        output_buffer,
        output_buffer_size,
        output_bytes_written,
        true /* switchless */);
}
