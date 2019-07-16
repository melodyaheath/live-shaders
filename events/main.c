#include "main.h"
#include "../actions/display.h"
#include "../actions/file.h"
#include "../actions/gtk_log.h"
#include "../ui/main.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct _EventData {
    MainUi *references;
    void *data;
} EventData;

static gpointer gpointer_ui_with_data(MainUi *ui, gpointer *data) {
    EventData *event = (EventData *)malloc(sizeof(EventData));
    if (event != NULL) {
        event->references = ui;
        event->data = data;
    }
    return (gpointer)event;
}

#define EVENT_DATA_TO_GPOINTER(x) ((gpointer)(x))
#define GPOINTER_TO_EVENT_DATA(x) (*((EventData *)(x)))

static void register_log_area_events(MainUi *);
static void register_menu_bar_top_events(MainUi *);
static void register_code_area_events(MainUi *);
static void register_opengl_area_events(MainUi *);

static void event_menu_bar_load(GtkMenuItem *, gpointer);
static void event_code_buffer_change(GtkTextBuffer *, gpointer);
static void event_log_auto_scroll(GtkTextView *, gpointer);

extern void ui_main_register_events(MainUi *main_ui) {
    gtk_log_init(GTK_TEXT_VIEW(main_ui->log_text_view));

    register_log_area_events(main_ui);
    register_menu_bar_top_events(main_ui);
    register_code_area_events(main_ui);
    register_opengl_area_events(main_ui);
}
static void register_log_area_events(MainUi *main_ui) {
    g_signal_connect(main_ui->log_text_view, "size-allocate",
                     G_CALLBACK(event_log_auto_scroll),
                     gpointer_ui_with_data(main_ui, NULL));
}

static void register_menu_bar_top_events(MainUi *main_ui) {
    g_signal_connect(main_ui->menu_bar_items.load_file, "activate",
                     G_CALLBACK(event_menu_bar_load),
                     gpointer_ui_with_data(main_ui, NULL));
}
static void register_code_area_events(MainUi *main_ui) {
    g_signal_connect(main_ui->code_text_buffer, "end-user-action",
                     G_CALLBACK(event_code_buffer_change), NULL);
}
static void register_opengl_area_events(MainUi *main_ui) {
    g_signal_connect(main_ui->opengl_area, "realize", G_CALLBACK(display_setup),
                     NULL);

    g_signal_connect(main_ui->opengl_area, "render", G_CALLBACK(display_render),
                     NULL);
}

static void event_menu_bar_load(GtkMenuItem *item, gpointer data) {
    EventData event = GPOINTER_TO_EVENT_DATA(data);
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Open File", GTK_WINDOW(event.references->window), action, "_Cancel",
        GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

    gint res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        const char *file_contents = file_read_contents(filename);
        if (file_contents != NULL) {
            gtk_text_buffer_set_text(event.references->code_text_buffer,
                                     file_contents, -1);
            free((void *)file_contents);
        }
        g_signal_emit_by_name(
            (gpointer)event.references->code_text_buffer,
            "end-user-action");
        free((void *)filename);
    }
    gtk_widget_destroy(dialog);
}

static void event_code_buffer_change(GtkTextBuffer *buffer, gpointer data) {
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    display_update_fragment_shader(text);
    free((void *)text);
}

static void event_log_auto_scroll(GtkTextView *log, gpointer data) {
    GtkAdjustment *vertical =
        gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(log));
    gdouble value = gtk_adjustment_get_upper(vertical) -
                    gtk_adjustment_get_page_size(vertical);
    gtk_adjustment_set_value(vertical, value);
}