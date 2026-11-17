#ifndef RESULT_H
#define RESULT_H

#include <ds.h>

#include <stddef.h>
#include <string.h>

typedef enum ResultCode {
    RESULT_OK = 0,
    RESULT_ERR,
} ResultCode;

#ifndef RESULT_STACK_SIZE
#define RESULT_STACK_SIZE (size_t)8
#endif

/**
 * A result for which result_is_err() returns true is expected to have all member pointers set to null.
 * Any other result is expected to have a nonnull string as the first element of stack.
 */
typedef struct Result {
    ResultCode code;
    char *message;
    char *stack[RESULT_STACK_SIZE];
} Result;

#define result_is_err(result) ((result).code != RESULT_OK)

#define result_free(result) do { \
    Result r = (result); \
    ds_free(r.message); \
    for (size_t i = 0; i < RESULT_STACK_SIZE; i++) { \
        ds_free(r.stack[i]); \
    } \
} while (0)

#define result_print(result) do { \
    Result r = (result); \
    _result_print(&r); \
} while (0)

#define result_ok ((Result){ \
    .code = RESULT_OK, \
    .message = nullptr, \
    .stack = { }, \
})

#define _result_printf_stack(str) ds_printf((str), "%s:%d", __FILE__, __LINE__)

#define result_return_err(msg_format, ...) do { \
    Result result = { \
        .code = RESULT_ERR, \
        .message = nullptr, \
        .stack = { }, \
    }; \
    ds_printf(result.message, (msg_format), __VA_ARGS__); \
    _result_printf_stack(result.stack[0]); \
    return result; \
} while (0)

#define result_return_on_err(result) do { \
    Result r = (result); \
    if (!result_is_err(r)) { \
        break; \
    } \
    size_t index = 0; \
    for (; index < RESULT_STACK_SIZE; index++) { \
        if (!r.stack[index]) { \
            break; \
        } \
    } \
    if (index >= RESULT_STACK_SIZE) { \
        memmove(r.stack, r.stack + 1, sizeof(*r.stack) * (RESULT_STACK_SIZE - 1)); \
        index = RESULT_STACK_SIZE - 1; \
    } \
    _result_printf_stack(r.stack[index]); \
    return r; \
} while (0)

void _result_print(const Result *result);

#endif
