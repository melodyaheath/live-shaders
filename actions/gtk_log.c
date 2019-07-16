#include "gtk_log.h"
#include "log.h"
#include <gtk/gtk.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// Log context
static LogContext *kLogContext;

// Area to log content to
const static GtkTextView *kTextView;

static void gtk_log_append_text(const char *message, size_t size) {
    if (!kTextView) {
        log_append_failover(kLogContext, message);
        return;
    }
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(kTextView));
    GtkTextIter end;

    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, message, size);
}

extern void gtk_log_init(GtkTextView *area) {
    kTextView = area;
    kLogContext = log_context_new();
    log_context_set_apppender(kLogContext, gtk_log_append_text);
}
extern void gtk_log_append(const char *message, ...) {
    va_list format;
    va_start(format, message);
    log_append(kLogContext, message, format);
    va_end(format);
}
extern void gtk_log_free() {
    if (kLogContext) {
        log_context_free(kLogContext);
    }
}
