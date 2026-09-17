#include "triangle.h"
#include "shader.h"
#include "texture.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

GLuint vertexBuffer;
GLuint EBO;
static Shader shaderProgram;
static unsigned int texture1;
static unsigned int texture2;
static float mixValue = 0.2f;
static GLuint VAO;

// float vertices[] = {
//     // positions                    // colors
//     0.5f,-0.5f,0.0f,  1.0f,0.0f,0.0f,      // bottom right
//     -0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,   // bottom left
//     0.0f,0.5f,0.0f,  0.0f,0.0f,1.0f  // top 
// };

static float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left
};

static unsigned int indices[] = {
    0, 1, 3,  // first triangle
    1, 2, 3   // second triangle
};

void initTriangle(void)
{
    // Need to create and bind a vertex array object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Copy vertex array in a vertex buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Copy indices in an element buffer object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Create shader program
    shaderProgram = createShader("res/shaders/triangle.vert", "res/shaders/triangle.frag");
    if (!shaderProgram.id) {
        fprintf(stderr, "Failed to create triangle shader\n");
        exit(EXIT_FAILURE);
    }

    // Set texture units with shader class
    useShader(&shaderProgram);
    setShaderInt(&shaderProgram, "texture1", 0);
    setShaderInt(&shaderProgram, "texture2", 1);

    // Load texture
    texture1 = loadTexture("res/textures/wall.jpg", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture2 = loadTexture("res/textures/awesomeface.png", GL_REPEAT, GL_REPEAT);
}

void adjustMixValue(float delta)
{
    mixValue += delta;
    if (mixValue >= 1.0f) {
        mixValue = 1.0f;  }
    if (mixValue <= 0.0f) {
        mixValue = 0.0f;  }
}

void renderTriangle(void)
{
    // Set shader program
    useShader(&shaderProgram);

    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Texture units to use more than one texture in a shader
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, texture1); // bind the texture to the texture unit

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Set mix value in the shader
    setShaderFloat(&shaderProgram, "mixValue", mixValue);

    // Bind vertex array object
    glBindVertexArray(VAO);

    // Draw the triangles
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void destroyTriangle(void)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &EBO);
    destroyShader(&shaderProgram);
}
