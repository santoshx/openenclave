// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <openenclave/bits/defs.h>
#include <openenclave/internal/trace.h>

oe_result_t oe_log(log_level_t level, const char* fmt, ...)
{
    OE_UNUSED(level);
    OE_UNUSED(fmt);
    return OE_UNSUPPORTED;
}
