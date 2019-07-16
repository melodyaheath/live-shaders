#define _GNU_SOURCE
#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

extern LogContext *log_context_new() {
    return (LogContext *)calloc(1, sizeof(LogContext));
}
extern void log_context_set_apppender(LogContext *context,
                                      void (*method)(const char *,
                                                     size_t size)) {
    if (!context || !method) {
        fprintf(stderr, "context or method pointer are null in %s\n", __func__);
        return;
    }
    context->log_appender_fn = method;
}
extern void log_append(LogContext *context, const char *message,
                       va_list format) {
    if (!context) {
        fprintf(stderr, "log context is null in %s\n", __func__);
        vfprintf(stderr, message, format);
        fprintf(stderr, "\n");
        return;
    }
    if (!context->log_appender_fn) {
        fprintf(stderr, "log context has no appender function set in %s\n",
                __func__);
        vfprintf(stderr, message, format);
        fprintf(stderr, "\n");
        return;
    }

    char *buffer;
    size_t size = vasprintf(&buffer, message, format);
    context->log_appender_fn(buffer, size);
    free(buffer);
}

extern void log_append_failover(LogContext *context, const char *message) {
    if (!context) {
        fprintf(stderr,
                "Log appender failed. Log context is null. Failover "
                "called!\nMessage during failover: %s",
                message);
        return;
    }
    fprintf(
        stderr,
        "Log appender failed. Fallover called!\nMessage during failover: %s",
        message);
    return;
}

extern void log_context_free(LogContext *context) {
    if (context) {
        free(context);
    }
}
