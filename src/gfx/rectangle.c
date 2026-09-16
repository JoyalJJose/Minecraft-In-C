#include "rectangle.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <linmath.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static GLuint shaderProgram;
static GLuint VAO;

GLuint EBO;

static int CompileShader(const char* source, GLenum type) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    // Check if shader compiled successfully
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

static int CreateShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
    GLuint program = glCreateProgram();
    GLuint vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check if shader programlinked successfully
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

    // Detach and delete shader objects after linking
    // glDetachShader(program, vs);
    // glDetachShader(program, fs);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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

float vertices2[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

// Indexed drawing using EBO (element buffer object)
float vertices3[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
   -0.5f, -0.5f, 0.0f,  // bottom left
   -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
   0, 1, 3,   // first triangle
   1, 2, 3    // second triangle
}; 

void initRectangle(void)
{
    // Need to create and bind a vertex array object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // we should unbind after configuring and then bind before use

    // we should generate/configure all VAOs and store them for later use.
    // When we want to draw an object, we take corresponding VAO, bind it,
    // then draw the object and unbind the VAO again.

    // Copy vertex array in a vertex buffer
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

    // Copy index array in an element buffer
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    // Create shader programs
    // move to top of file
    const char* vertexShaderSource =
        // "#version 330 core\n"
        // "layout(location = 0) in vec4 position;\n"
        // "void main() {\n"
        // "    gl_Position = position;\n"
        // "}\n";
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\n";
    const char* fragmentShaderSource =
        // "#version 330 core\n"
        // "out vec4 color;\n"
        // "void main() {\n"
        // "    color = vec4(0.0, 1.0, 0.0, 1.0);\n"
        // "}\n";
        "#version 330 core\n"
        "out vec4 Fragcolor;\n"
        "void main() {\n"
        "    Fragcolor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";
    
    shaderProgram = CreateShader(vertexShaderSource, fragmentShaderSource);
}

void renderRectangle(GLFWwindow *window)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Set back to its default using glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Render triangles from an index buffer
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Unbind VAO - but no need to do this every time
    // glBindVertexArray(0);
}
