#include "rectangle.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <linmath.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static GLuint shaderProgram;
static GLuint vao;

static int CompileShader(const char* source, GLenum type) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    int result = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        printf("Failed to compile %s shader: %s\n",
               (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static int CreateShader(const char *vertexShader, const char *fragmentShader) {
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        int length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);
        printf("Failed to link shader: %s\n", message);
        glDeleteProgram(program);
        program = 0;
    }

    // glDetachShader(program, vs);
    // glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

typedef struct Vertex {
    vec2 pos;
} Vertex;

// 4 vertices for rectangle (2 triangles) in GL_TRIANGLE_STRIP order
static const Vertex vertices[4] = {
    { { -0.6f,  0.4f } },  //top left
    { {  0.6f,  0.4f } },  //top right
    { { -0.6f, -0.4f } },  //bottom left
    { {  0.6f, -0.4f } }   //bottom right
};

void initRectangle(void)
{
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Need to create and bind a vertex array object (VAO)
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    // Create shader programs
    char* vertexShader =
        "#version 330 core\n"
        "layout(location = 0) in vec4 position;\n"
        "void main() {\n"
        "    gl_Position = position;\n"
        "}\n";
    char* fragmentShader =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main() {\n"
        "    color = vec4(0.0, 1.0, 0.0, 1.0);\n"
        "}\n";
    
    shaderProgram = CreateShader(vertexShader, fragmentShader);
}

void renderRectangle(GLFWwindow *window)
{
    // Keep for now with fullscreen resizing but should move
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}