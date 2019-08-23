// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef _OE_SWITCHLESS_CALLS_H
#define _OE_SWITCHLESS_CALLS_H

#include <openenclave/internal/switchless.h>

oe_result_t _handle_init_switchless(uint64_t arg_in);

oe_result_t _oe_post_switchless_ocall(oe_call_host_function_args_t* args);

#endif // _OE_SWITCHLESS_CALLS_H