#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>

typedef struct Shader {
    GLuint id;
} Shader;

Shader createShader(const char* vertexPath, const char* fragmentPath);
void useShader(const Shader* shader);
void destroyShader(Shader* shader);

//for when we have uniforms
// void setShaderBool(const Shader* shader, const char* name, int value);
// void setShaderInt(const Shader* shader, const char* name, int value);
// void setShaderFloat(const Shader* shader, const char* name, float value);

#endif
