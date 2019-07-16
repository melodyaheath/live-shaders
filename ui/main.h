#include <gtk/gtk.h>
#pragma once

typedef struct _MainUi {
    GApplication* application;
    GtkWidget* window;
    GtkWidget* verticle_box;
    GtkWidget* menu_bar_top;
    struct _menu_bar_items {
        GtkWidget* load_file;
    } menu_bar_items;
    GtkWidget* grid_main;
    GtkWidget* code_frame;
    GtkWidget* code_scrolled_window;
    GtkWidget* code_text_view;
    GtkTextBuffer* code_text_buffer;
    GtkWidget* opengl_area;
    GtkWidget* log_frame;
    GtkWidget* log_scrolled_window;
    GtkWidget* log_text_view;
    GtkWidget* variables;
} MainUi;

extern void ui_main_draw(GApplication *, gpointer);

