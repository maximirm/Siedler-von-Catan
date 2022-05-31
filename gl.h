#pragma once

#ifdef __APPLE__
#include <GL/glew.h>
#include <OpenGL/gl.h>
#elif _WIN32
#include <GL\glew.h>
#include <GL\GL.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif
