#include <stdarg.h>
#include <stdlib.h>
#pragma once

typedef struct _LogContext {
    void (*log_appender_fn)(const char *, size_t);
} LogContext;

extern LogContext *log_context_new();
extern void log_context_set_apppender(LogContext *,
                                      void (*)(const char *, size_t));
extern void log_context_free(LogContext *);
extern void log_append(LogContext *, const char *, va_list);
extern void log_append_failover(LogContext *, const char *);
