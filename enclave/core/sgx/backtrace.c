// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <openenclave/bits/safecrt.h>
#include <openenclave/enclave.h>
#include <openenclave/internal/backtrace.h>
#include <openenclave/internal/calls.h>
#include <openenclave/internal/globals.h>
#include <openenclave/internal/print.h>
#include <openenclave/internal/raise.h>

#if defined(__INTEL_COMPILER)
#error "optimized __builtin_return_address() not supported by Intel compiler"
#endif

/* Return null if address is outside of the enclave; else return ptr. */
const void* _check_address(const void* ptr)
{
    if (!oe_is_within_enclave(ptr, sizeof(uint64_t)))
        return NULL;

    return ptr;
}

/* Safe implementation of oe_backtrace.
 *
 * The original implementation used the ___builtin_return_address intrinsic.
 * The intrinsic however is unsafe and can crash if any function in the
 * call-stack does not preserve the stack-frame. This scenario can easily happen
 * if any function in the call-stack has been compiled with optimization, or is
 * a special function like global initializer.
 * This new implementation below safely walks up the call-stack, ensuring that
 * each potential-frame is not null and lies within the enclave.
 */
int oe_backtrace(void** buffer, int size)
{
    OE_UNUSED(buffer);
    OE_UNUSED(size);
#ifdef OE_USE_DEBUG_MALLOC
    // Fetch the frame-pointer of the current function.
    // The current function oe_backtrace is not expected to be inlined.
    // The rbp register contains the frame-pointer upon entry to the function.
    void** frame = NULL;
    asm volatile("movq %%rbp, %0"
                 : "=r"(frame)
                 : /* no inputs */
                 : /* no clobbers */
    );

    // Upon entry to a function, rsp + 0 contains the return address.
    // Generally, the first thing that a function does upong entry is
    //     push %rbp
    // rbp is expected to contain the callee's frame pointer.
    // Thus after saving rbp,
    //     rsp + 0  (frame[0]) contains callee's frame pointer.
    //     rsp + 8  (frame[1]) contains return address (within the callee).
    //
    // However, the compiler may not always store the callee's frame-ptr in the
    // rbp register. Within optimizations enabled, the compiler could use rbp
    // just like other general-purpose register and hold some value rather than
    // the frame-pointer. While frame[1] always contains the return address,
    // frame[0] may not always contain the pointer to callee's stack frame.
    // To be on the safer-side, we always check that the values we access
    // while traversing the stack always lie within the enclave.
    int n = 0;
    while (n < size)
    {
        // Ensure that the current frame is safe to access.
        if (!_check_address(frame))
            break;

        // Ensure that the return address is valid.
        if (!_check_address(frame[1]))
            break;

        // Store address and move to previous frame.
        buffer[n++] = frame[1];
        frame = (void**)*frame;
    }

    return n;
#else
    return 0;
#endif
}
