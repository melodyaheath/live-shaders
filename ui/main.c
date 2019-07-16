#include "main.h"
#include "../actions/file.h"
#include "../events/main.h"
#include <gtk/gtk.h>
#include <stdbool.h>

static MainUi *main_ui_new() { return (MainUi *)malloc(sizeof(MainUi)); }

static void draw_menu_bar_top(MainUi *main_ui) {
    main_ui->menu_bar_top = gtk_menu_bar_new();
    main_ui->menu_bar_items.load_file =
        gtk_menu_item_new_with_label("Load File");

    gtk_container_add(GTK_CONTAINER(main_ui->menu_bar_top),
                      main_ui->menu_bar_items.load_file);
}

static void draw_code_area(MainUi *main_ui) {
    main_ui->code_frame = gtk_frame_new(NULL);
    main_ui->code_scrolled_window = gtk_scrolled_window_new(NULL, NULL);

    // create a text view
    main_ui->code_text_view = gtk_text_view_new();
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(main_ui->code_text_view), 12);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(main_ui->code_text_view), 12);
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(main_ui->code_text_view), 6);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(main_ui->code_text_view), 6);

    // Wrap it in a scrolled window, wrap that in a frame. This creates thin
    // borders on the text-view.
    gtk_container_add(GTK_CONTAINER(main_ui->code_scrolled_window),
                      main_ui->code_text_view);
    gtk_container_add(GTK_CONTAINER(main_ui->code_frame),
                      main_ui->code_scrolled_window);

    main_ui->code_text_buffer =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(main_ui->code_text_view));

    gtk_text_buffer_set_text(main_ui->code_text_buffer,
                             file_read_contents("shaders/fragment.frag"), -1);

    g_object_set(main_ui->code_text_view, "expand", TRUE, NULL);
}

static void draw_opengl_area(MainUi *main_ui) {
    main_ui->opengl_area = gtk_gl_area_new();
    g_object_set(main_ui->opengl_area, "expand", TRUE, NULL);
}

static void draw_log(MainUi *main_ui) {
    main_ui->log_frame = gtk_frame_new(NULL);
    main_ui->log_scrolled_window = gtk_scrolled_window_new(NULL, NULL);

    // create a text view
    main_ui->log_text_view = gtk_text_view_new();
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(main_ui->log_text_view), 12);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(main_ui->log_text_view), 12);
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(main_ui->log_text_view), 6);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(main_ui->log_text_view), 6);

    // Wrap it in a scrolled window, wrap that in a frame. This creates thin
    // borders on the text-view.
    gtk_container_add(GTK_CONTAINER(main_ui->log_scrolled_window),
                      main_ui->log_text_view);
    gtk_container_add(GTK_CONTAINER(main_ui->log_frame),
                      main_ui->log_scrolled_window);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(main_ui->log_text_view), FALSE);

    g_object_set(main_ui->log_text_view, "expand", TRUE, NULL);
}

static void draw_layout(MainUi *main_ui) {
    main_ui->grid_main = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(main_ui->grid_main), main_ui->code_frame, 0, 0, 1,
                    8);
    gtk_grid_attach_next_to(GTK_GRID(main_ui->grid_main), main_ui->opengl_area,
                            main_ui->code_frame, GTK_POS_RIGHT, 1, 8);
    gtk_grid_attach_next_to(GTK_GRID(main_ui->grid_main), main_ui->log_frame,
                            main_ui->code_frame, GTK_POS_BOTTOM, 2, 2);

    main_ui->verticle_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(main_ui->verticle_box), main_ui->menu_bar_top,
                       false, false, 0);
    gtk_box_pack_end(GTK_BOX(main_ui->verticle_box), main_ui->grid_main, true,
                     true, 0);
    gtk_container_add(GTK_CONTAINER(main_ui->window), main_ui->verticle_box);
}

extern void ui_main_draw(GApplication *app, gpointer data) {
    MainUi *main_ui = main_ui_new();
    main_ui->application = app;
    main_ui->window =
        gtk_application_window_new(GTK_APPLICATION(main_ui->application));
    gtk_window_set_default_size(GTK_WINDOW(main_ui->window), 1080, 768);

    draw_menu_bar_top(main_ui);
    draw_code_area(main_ui);
    draw_opengl_area(main_ui);
    draw_log(main_ui);
    draw_layout(main_ui);

    ui_main_register_events(main_ui);

    // display the window
    gtk_widget_show_all(GTK_WIDGET(main_ui->window));
}