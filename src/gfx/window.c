#include "window.h"
#include "triangle3.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

// Global window
static GLFWwindow* window;


// Receive key press and release events
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// If a GLFW function fails, an error is reported to the error callback
static void error_callback(int error, const char* description)
{
    (void)error;
    fprintf(stderr, "Error: %s\n", description);
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

    // Force GLFW to use X11.
    // This makes GLFW create the OpenGL context through GLX instead of attempting the EGL path.
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

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
    window = glfwCreateWindow(640, 480, "Minecraft", NULL, NULL);
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

    // Set key callback function
    glfwSetKeyCallback(window, key_callback);

    // Enable vsync - buffer swap synchronised with monitor refresh rate
    glfwSwapInterval(1);

}


// Main application loop
void mainLoop(void)
{
    // Initialise
    initTriangle3();

    while (!glfwWindowShouldClose(window)) {
        // Process input/window events first
        glfwPollEvents();

        //do stuff here using fresh input

        //render draw to back buffer
        renderTriangle3(window);

        // Show frame just drawn
        glfwSwapBuffers(window);

    }//end while

    destroyWindow();
}


