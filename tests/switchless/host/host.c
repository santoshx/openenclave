// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <limits.h>
#include <openenclave/host.h>
#include <openenclave/internal/error.h>
#include <openenclave/internal/tests.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../../host/strings.h"
#include "switchless_u.h"

#define NUM_HOST_THREADS 16

int host_echo_switchless(
    char* in,
    char* out,
    char* str1,
    char* str2,
    char str3[100])
{
    OE_TEST(strcmp(str1, "oe_host_strdup1") == 0);
    OE_TEST(strcmp(str2, "oe_host_strdup2") == 0);
    OE_TEST(strcmp(str3, "oe_host_strdup3") == 0);

    strcpy(out, in);

    return 0;
}

int host_echo_regular(
    char* in,
    char* out,
    char* str1,
    char* str2,
    char str3[100])
{
    OE_TEST(strcmp(str1, "oe_host_strdup1") == 0);
    OE_TEST(strcmp(str2, "oe_host_strdup2") == 0);
    OE_TEST(strcmp(str3, "oe_host_strdup3") == 0);

    strcpy(out, in);

    return 0;
}

int main(int argc, const char* argv[])
{
    oe_enclave_t* enclave = NULL;
    oe_result_t result;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s ENCLAVE_PATH\n", argv[0]);
        return 1;
    }

    const uint32_t flags = oe_get_create_flags();

    if ((result = oe_create_switchless_enclave(
             argv[1], OE_ENCLAVE_TYPE_SGX, flags, NULL, 0, &enclave)) != OE_OK)
        oe_put_err("oe_create_enclave(): result=%u", result);

    oe_start_switchless_manager(enclave, 2);

    char out[100];
    int return_val;

    double switchless_microseconds = 0;
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);
    OE_TEST(
        enc_echo_switchless(enclave, &return_val, "Hello World", out) == OE_OK);
    clock_gettime(CLOCK_REALTIME, &end);
    switchless_microseconds += (end.tv_sec - start.tv_sec) * 1000000.0 +
                               (end.tv_nsec - start.tv_nsec) / 1000.0;

    double regular_microseconds = 0;
    clock_gettime(CLOCK_REALTIME, &start);
    OE_TEST(
        enc_echo_regular(enclave, &return_val, "Hello World", out) == OE_OK);
    clock_gettime(CLOCK_REALTIME, &end);
    regular_microseconds += (end.tv_sec - start.tv_sec) * 1000000.0 +
                            (end.tv_nsec - start.tv_nsec) / 1000.0;

    result = oe_terminate_enclave(enclave);
    OE_TEST(result == OE_OK);

    printf(
        "Time spent in repeating OCALL 1 million times: switchless %d vs "
        "regular %d ms, speed up: %.2f\n",
        (int)switchless_microseconds / 1000,
        (int)regular_microseconds / 1000,
        (double)regular_microseconds / switchless_microseconds);
    printf("=== passed all tests (switchless)\n");

    return 0;
}
