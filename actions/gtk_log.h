#pragma once
#include <gtk/gtk.h>

extern void gtk_log_init(GtkTextView*);
extern void gtk_log_append(const char *, ...);
extern void gtk_log_free();
