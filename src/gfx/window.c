#include "window.h"
#include "triangle.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

// 0 = Start windowed, 1 = Start fullscreen
#define START_FULLSCREEN 0

// Global window
static GLFWwindow *window;
static int windowed_x, windowed_y, windowed_w, windowed_h;


static void setFullscreen(GLFWwindow *win, int enable)
{
    if (enable) {
        glfwGetWindowPos(win, &windowed_x, &windowed_y);
        glfwGetWindowSize(win, &windowed_w, &windowed_h);

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(win, monitor, 0, 0,
                             mode->width, mode->height, mode->refreshRate);
    } else {
        glfwSetWindowMonitor(win, NULL, windowed_x, windowed_y,
                             windowed_w, windowed_h, 0);
    }
}

static void toggleFullscreen(GLFWwindow *win)
{
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
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (key == GLFW_KEY_F) {
        toggleFullscreen(window);
    }
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
    window = glfwCreateWindow(2000, 1200, "Minecraft", NULL, NULL);
    if (!window)
    {
        // Window or OpenGL context creation failed
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Get window position and size
    glfwGetWindowPos(window, &windowed_x, &windowed_y);
    glfwGetWindowSize(window, &windowed_w, &windowed_h);
    
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

    if (START_FULLSCREEN) {
        setFullscreen(window, 1);
    }
    
}

// Main application loop
void mainLoop(void)
{
    // Initialise
    initTriangle();

    while (!glfwWindowShouldClose(window)) {
        // Process input/window events first
        glfwPollEvents();

        //do stuff here using fresh input

        //render - draw to back buffer
        renderTriangle(window);

        // Show frame just drawn
        glfwSwapBuffers(window);

    }//end while

    destroyWindow();
}
