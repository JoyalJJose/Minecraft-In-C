#include "triangle.h"
#include "shader.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <linmath.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static Shader shaderProgram;
static GLuint VAO;

// static int compileShader(const char* source, GLenum type) {
//     GLuint id = glCreateShader(type);
//     glShaderSource(id, 1, &source, NULL);
//     glCompileShader(id);

//     // Check if shader compiled successfully
//     int result = 0;
//     glGetShaderiv(id, GL_COMPILE_STATUS, &result);
//     if (result == GL_FALSE) {
//         int length = 0;
//         glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
//         char* message = (char*)alloca(length * sizeof(char));
//         glGetShaderInfoLog(id, length, &length, message);
//         printf("Failed to compile %s shader: %s\n",
//                (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
//         glDeleteShader(id);
//         return 0;
//     }

//     return id;
// }

// static int createShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
//     GLuint program = glCreateProgram();
//     GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
//     GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

//     glAttachShader(program, vertexShader);
//     glAttachShader(program, fragmentShader);
//     glLinkProgram(program);

//     // Check if shader programlinked successfully
//     int linked = 0;
//     glGetProgramiv(program, GL_LINK_STATUS, &linked);
//     if (linked == GL_FALSE) {
//         int length = 0;
//         glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
//         char* message = (char*)alloca(length * sizeof(char));
//         glGetProgramInfoLog(program, length, &length, message);
//         printf("Failed to link shader: %s\n", message);
//         glDeleteProgram(program);
//         program = 0;
//     }

//     // Detach and delete shader objects after linking
//     // glDetachShader(program, vs);
//     // glDetachShader(program, fs);
//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     return program;
// }

// float vertices[] = {
//     // positions                    // colors
//     0.5f,-0.5f,0.0f,  1.0f,0.0f,0.0f,      // bottom right
//     -0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,   // bottom left
//     0.0f,0.5f,0.0f,  0.0f,0.0f,1.0f  // top 
// }; 

// void initTriangle(void)
// {
//     // Need to create and bind a vertex array object (VAO)
//     glGenVertexArrays(1, &VAO);
//     glBindVertexArray(VAO);

//     // Copy vertex array in a vertex buffer
//     GLuint vertexBuffer;
//     glGenBuffers(1, &vertexBuffer);
//     glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     // Set up vertex attribute pointers
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);

//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
//     glEnableVertexAttribArray(1);

//     // Create shader programs
//     const char* vertexShaderSource =
//         "#version 330 core\n"
//         "layout(location = 0) in vec3 aPos;\n"
//         "layout (location = 1) in vec3 aColor;\n"
//         "out vec3 ourColor;\n"
//         "void main() {\n"
//         "    gl_Position = vec4(aPos, 1.0);\n"
//         "    ourColor = aColor;\n"
//         "}\n";
//     const char* fragmentShaderSource =
//         "#version 330 core\n"
//         "out vec4 Fragcolor;\n"
//         "in vec3 ourColor;\n"
//         "void main() {\n"
//         "    Fragcolor = vec4(ourColor, 1.0f);\n"
//         "}\n";
    
//     shaderProgram = createShader(vertexShaderSource, fragmentShaderSource);
// }

// void renderTriangle(GLFWwindow *window)
// {
//     glUseProgram(shaderProgram);
//     glBindVertexArray(VAO);

//     glDrawArrays(GL_TRIANGLES, 0, 3);
// }

//===================================================================================================

float vertices[] = {
    // positions                    // colors
    0.5f,-0.5f,0.0f,  1.0f,0.0f,0.0f,      // bottom right
    -0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,   // bottom left
    0.0f,0.5f,0.0f,  0.0f,0.0f,1.0f  // top 
}; 

void initTriangle(void)
{
    // Need to create and bind a vertex array object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Copy vertex array in a vertex buffer
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // Create shader program
    shaderProgram = createShader("res/shaders/triangle.vs", "res/shaders/triangle.fs");
}

void renderTriangle(GLFWwindow *window)
{
    useShader(&shaderProgram);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

