#include "result.h"

#include <stdio.h>

void _result_print(const Result *result) {
    if (!result_is_err(*result)) {
        printf("OK\n");
        return;
    }

    printf("Error");
    if (result->message) {
        printf(": %s", result->message);
    }
    putc('\n', stdout);

    printf("Stack trace (most recent call last):\n");
    for (size_t i = 0; i < RESULT_STACK_SIZE; i++) {
        char *entry = result->stack[i];
        if (!entry) {
            break;
        }

        printf("\t%s\n", entry);
    }
}
