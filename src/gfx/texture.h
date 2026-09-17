#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/gl.h>

unsigned int loadTexture(const char* path, GLenum wrapS, GLenum wrapT);

#endif
