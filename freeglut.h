// in order to avoid "gl included before glew" error
#include "gl.h"

#ifdef __APPLE__
#include <GL/freeglut.h>
#elif _WIN32
#include "GL\freeglut.h"
#else
#include "GL/freeglut.h"
#endif
