#ifndef TRIANGLE_H
#define TRIANGLE_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void initTriangle(void);
void renderTriangle(void);
void destroyTriangle(void);
void adjustMixValue(float delta);

#endif
