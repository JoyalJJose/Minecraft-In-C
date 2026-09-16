#include "window.h"
#include "rectangle.h"
#include "triangle.h"
#include "camera.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

// 0 = Start windowed, 1 = Start fullscreen
#define START_FULLSCREEN 1

// Global window
static GLFWwindow *window;
static int windowX, windowY;
static int windowWidth = 2048;
static int windowHeight = 1152;


static void setFullscreen(GLFWwindow *win, int enable) {
    if (enable) {
        glfwGetWindowPos(win, &windowX, &windowY);
        glfwGetWindowSize(win, &windowWidth, &windowHeight);

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(win, monitor, 0, 0,
                             mode->width, mode->height, mode->refreshRate);
    } else {
        glfwSetWindowMonitor(win, NULL, windowX, windowY,
                             windowWidth, windowHeight, 0);
    }
}

static void toggleFullscreen(GLFWwindow *win) {
    setFullscreen(win, glfwGetWindowMonitor(win) == NULL);
}


// Receive key press and release events
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;

    if (action != GLFW_PRESS) {
        return;
    }

    // ESC - close window, F - toggle fullscreen
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (key == GLFW_KEY_F) {
        toggleFullscreen(window);
    }
}

static void processInput(void) {
    // float speed = 5.0f * dt;
    float speed = 1.0f;

    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     camera.position += camera.front * speed;

    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     camera.position -= camera.front * speed;

    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     camera.position -= camera.right * speed;

    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     camera.position += camera.right * speed;

    // if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    //     camera.position.y += speed;

    // if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    //     camera.position.y -= speed;
}


// If a GLFW function fails, an error is reported to the error callback
static void error_callback(int error, const char* description)
{
    (void)error;
    fprintf(stderr, "Error: %s\n", description);
}

// Callback function for window size changes
static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    (void)window;
    // Update the GL viewport — don't overwrite windowWidth/windowHeight
    glViewport(0, 0, width, height);
} 

// Destroy window and context
static void destroyWindow(void) {
    // Destroy window & context - no more events delivered and handle invalid
    glfwDestroyWindow(window);
    // Terminate - destroy windows and release resources
    glfwTerminate();
}

// // Initialise ...
// static void init(void)
// {
//     // do init stuff e.g. vbo, vao, shader, etc.
// }


// Create and setup window and context
void createWindow(void)
{
    // Set callback function before initialising
    glfwSetErrorCallback(error_callback);

    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    // Set minimum OpenGL version - 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window and its OpenGL context - returns a handle to the created combined window and context object
    window = glfwCreateWindow(windowWidth, windowHeight, "Minecraft", NULL, NULL);
    if (!window)
    {
        // Window or OpenGL context creation failed
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // Initialise extension loader library
    if (!gladLoadGL(glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        destroyWindow();
        exit(EXIT_FAILURE);
    }

    // Set framebuffer size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Set key callback function
    glfwSetKeyCallback(window, key_callback);

    // Enable vsync - buffer swap synchronised with monitor refresh rate
    glfwSwapInterval(1);

    if (START_FULLSCREEN) {setFullscreen(window, 1);}
    
}

// Main application loop
void renderLoop(void)
{
    // Initialise
    // initRectangle();
    initTriangle();

    while (!glfwWindowShouldClose(window)) {
        // Process input/window events first
        glfwPollEvents();
        processInput();

        //do stuff here using fresh input
        //game physics & state updates

        //set sky colour - 78A7FF
        glClearColor(0.47f, 0.65f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //render - draw to back buffer
        // renderRectangle(window);
        renderTriangle(window);

        // Show frame just drawn
        glfwSwapBuffers(window);

    }//end while

    // //deallocate resources
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &vertexBuffer);
    // glDeleteBuffers(1, &EBO);
    // glDeleteProgram(shaderProgram);

    destroyWindow();
}
