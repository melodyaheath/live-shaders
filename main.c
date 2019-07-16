#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>
#include "ui/main.h"

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("app.melody.live-shaders", G_APPLICATION_FLAGS_NONE);
    MainUi* main_ui = (MainUi*)malloc(sizeof(MainUi));

    g_signal_connect(app, "activate", G_CALLBACK(ui_main_draw), main_ui);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
