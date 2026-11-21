#ifndef RESULT_H
#define RESULT_H

#include <ds.h>

#include <stddef.h>

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

#define _result_new_printf_stack_entry(func, file, line) ds_new_printf("%s:%d - %s", (file), (line), (func))

#define result_err(msg_format, ...) (Result){ \
    .code = RESULT_ERR, \
    .message = ds_new_printf((msg_format) __VA_OPT__(,) __VA_ARGS__), \
    .stack = { _result_new_printf_stack_entry(__func__, __FILE__, __LINE__) }, \
}

#define result_pass(result) _result_pass(result, __func__, __FILE__, __LINE__)
/**
 * Checks if a result represents an error value. If it does, returns the passed result (see result_pass()) via out and returns true. Otherwise returns false.
 * @param result The result to check.
 */
#define result_pass_err(out, result) result_is_err(((out) = result_pass(result)))

void _result_print(const Result *result);
Result _result_pass(Result result, const char *func, const char *file, int line);

#endif
