#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD  // avoid clangd/MinGW _mm_prefetch false positive 
#include "stb_image.h"

#include <glad/gl.h>
#include <stdio.h>

#include "texture.h"


unsigned int loadTexture(const char* path, GLenum wrapS, GLenum wrapT) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Texture wrapping: GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS); // s = x axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT); // t = y axis

    // For clamp to border colour
    // float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Texture filtering - nearest neighbour or linear interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // min - scaling down
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);                // mag - scaling up
    // Using mipmaps - smaller resolution textures for distant objects
    // mipmap only for min filter not mag filter

    // Load and generate the texture using image data
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);  // flip so the texture is not upside down
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 4) {
            format = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        // Automatically generate all the required mipmaps for the texture
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture: %s\n", path);
    }

    stbi_image_free(data);
    return texture;
}