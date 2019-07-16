#include <stdbool.h>
#include <gtk/gtk.h>

extern void display_update_fragment_shader( char*);
extern gboolean display_render(GtkGLArea *, GdkGLContext *);
extern void display_setup(GtkGLArea *, GdkGLContext *);
