#include "display.h"
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <gtk/gtk.h>
#include <gtk/gtkglarea.h>
#include <stdbool.h>
#include <stdio.h>
#include "file.h"
#include "gtk_log.h"

static unsigned int VAO;
static unsigned int shader_vertex;
static unsigned int shader_program;

typedef struct _ShaderSet {
    int shader_vertex;
    int shader_fragment;
} ShaderSet;

static int display_make_shader(GLenum , const char* );
static ShaderSet display_make_default_shader_set();
static void display_set_shader_program(unsigned int);
static void display_make_shader_program(ShaderSet);

static int display_make_shader(GLenum shaderType, const char* contents) {
    int vertexShader = glCreateShader(shaderType);
    int success = 0;

    glShaderSource(vertexShader, 1, &contents, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char message[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, message);
        gtk_log_append("Error compiling shader :: %s\n", message);
        glDeleteShader(vertexShader);
        return -1;
    }
    return vertexShader;
}


static ShaderSet display_make_default_shader_set() {
    const char *default_fragment = file_read_contents("shaders/fragment.frag");
    if (  !default_fragment) {
        gtk_log_append("Failed to load default fragment shader\n");
        return (ShaderSet) { .shader_vertex = -1, .shader_fragment = -1};
    }
    const char *default_vertex = file_read_contents("shaders/vertex.vert");
    if (  !default_vertex) {
        gtk_log_append("Failed to load default vertex shader\n");
        free((void*)default_fragment);
        return (ShaderSet) { .shader_vertex = -1, .shader_fragment = -1};
    }

    ShaderSet shaders = (ShaderSet) { 
        .shader_vertex = display_make_shader(GL_VERTEX_SHADER, default_vertex),
        .shader_fragment = display_make_shader(GL_FRAGMENT_SHADER, default_fragment)
    };

    
    free((void*)default_vertex);
    free((void*)default_fragment);
    return shaders;
}

extern void display_setup(GtkGLArea *area, GdkGLContext *context) {
    gtk_log_append("Initializing display\n");
    gtk_gl_area_make_current(GTK_GL_AREA(area));

    ShaderSet shaders = display_make_default_shader_set();
    if ( shaders.shader_fragment == -1 || shaders.shader_fragment == -1) {
        gtk_log_append("Failed to intialized. Reason: failed to make shaders.\n");
        return;
    }
    shader_vertex = shaders.shader_vertex;
    display_make_shader_program(shaders);
    
    float vertices[] = {-1.f, -1.f, -1.f, 1.f, 1.f, -1.f, 1.f, 1.f};
        
    unsigned int indices[] = { 0, 1, 2, 3 };
    unsigned int VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}



static void display_set_shader_program(unsigned int program) {
    shader_program = program;
}

static void display_make_shader_program(ShaderSet shaders) {
    unsigned int program = glCreateProgram();
    glAttachShader(program, shaders.shader_vertex);
    glAttachShader(program, shaders.shader_fragment);
    glLinkProgram(program);

    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char message[512];
        glGetProgramInfoLog(program, 512, NULL, message);
        gtk_log_append("Error linking shader program :: %s\n", message);
        glDeleteProgram(program);
    }
    shader_program = program;
    display_set_shader_program(program);
    glDeleteShader(shaders.shader_fragment);
}

extern void display_update_fragment_shader( char* source ) {
    int fragment_shader = display_make_shader(GL_FRAGMENT_SHADER, source);
    if ( !fragment_shader ) {
        return;
    }
    ShaderSet shaders = (ShaderSet) { 
        .shader_vertex = shader_vertex,
        .shader_fragment = fragment_shader
    };

    if ( shaders.shader_fragment == -1) {
        return;
    }
    shader_vertex = shaders.shader_vertex;
    display_make_shader_program(shaders);
}



extern gboolean display_render(GtkGLArea * display, GdkGLContext * context) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(0.f, 0.f, 0.f, 1.f);

    glUseProgram(shader_program);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
    glFlush();

    gtk_widget_queue_draw (GTK_WIDGET(display));

    return true;
}