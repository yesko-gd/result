#include "result.h"

#include <stdio.h>
#include <string.h>

void _result_print(const Result *result) {
    if (!result_is_err(*result)) {
        printf("OK\n");
        return;
    }

    fprintf(stderr, "Error");
    if (result->message) {
        fprintf(stderr, ": %s", result->message);
    }
    putc('\n', stderr);

    fprintf(stderr, "Stack trace (most recent call last):\n");
    for (size_t i = 0; i < RESULT_STACK_SIZE; i++) {
        char *entry = result->stack[i];
        if (!entry) {
            break;
        }

        fprintf(stderr, "\t%s\n", entry);
    }
}

Result _result_pass(Result result, const char *file, int line) {
    if (result_is_err(result)) {
        size_t index = 0;
        for (; index < RESULT_STACK_SIZE; index++) {
            if (!result.stack[index]) {
                break;
            }
        }
        if (index >= RESULT_STACK_SIZE) {
            memmove(result.stack, result.stack + 1, sizeof(*result.stack) * (RESULT_STACK_SIZE - 1));
            index = RESULT_STACK_SIZE - 1;
        }
        result.stack[index] = _result_new_printf_stack_entry(file, line);
    }
    return result;
}
