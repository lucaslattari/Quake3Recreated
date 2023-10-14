#pragma once

#include <cmath>
#include <vector>

#include "vector.h"
#include "GL_Utils.h"

void drawAxes(GLuint shaderProgram);
void drawTriangle(GLuint shaderProgram, Vec3f v1, Vec3f v2, Vec3f v3);
void drawSphere(GLuint shaderProgram, float radius, Vec3f p);