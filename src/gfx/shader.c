#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

static char* readFile(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open shader file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size < 0) {
        fprintf(stderr, "Failed to read shader file: %s\n", path);
        fclose(file);
        return NULL;
    }

    // allocate memory for shader code - file size not known at compile time
    char* buffer = malloc((size_t)size + 1);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory for shader file: %s\n", path);
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(buffer, 1, (size_t)size, file);
    buffer[bytesRead] = '\0';
    fclose(file);
    return buffer;
}

static GLuint compileShader(const char* source, GLenum type)
{
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
        fprintf(stderr, "Failed to compile %s shader: %s\n",
                (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static GLuint linkShaderProgram(const char* vertexSource, const char* fragmentSource)
{
    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
    if (!vertexShader || !fragmentShader) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        int length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);
        fprintf(stderr, "Failed to link shader: %s\n", message);
        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}


Shader createShader(const char* vertexPath, const char* fragmentPath)
{
    Shader shader = {0};

    char* vertexCode = readFile(vertexPath);
    char* fragmentCode = readFile(fragmentPath);
    if (!vertexCode || !fragmentCode) {
        free(vertexCode);
        free(fragmentCode);
        return shader;
    }

    shader.id = linkShaderProgram(vertexCode, fragmentCode);
    if (!shader.id) {
        fprintf(stderr, "Failed to create shader from '%s' and '%s'\n",
                vertexPath, fragmentPath);
    }

    free(vertexCode);
    free(fragmentCode);

    return shader;
}


void useShader(const Shader* shader) {
    glUseProgram(shader->id);
}

void destroyShader(Shader* shader) {
    glDeleteProgram(shader->id);
    shader->id = 0;
}

// Assumes the shader is already bound via useShader()
void setShaderBool(const Shader* shader, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void setShaderInt(const Shader* shader, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void setShaderFloat(const Shader* shader, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}
