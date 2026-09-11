#include <GLFW/glfw3.h>
#include <stdio.h>


void error_callback(int error, const char* description);


int main(void)
{
    // Set callback function before initialising
    glfwSetErrorCallback(error_callback);

    // Force GLFW to use X11.
    // This makes GLFW create the OpenGL context through GLX instead of attempting the EGL path.
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    };

    // Request an OpenGL 3.3 context - change later?
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        // Window or OpenGL context creation failed
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    
    // Main application loop


    // Terminate - destroy windows and release resources
    glfwTerminate();

}//end main


// If a GLFW function fails, an error is reported to the error callback
void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
