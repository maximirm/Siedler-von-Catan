#include <iostream>
#include "GLIncludes.hpp"
#include "GLError.hpp"


const char * getGLErrorString(GLenum err){

  switch (err)
    {
    case GL_NO_ERROR:          return "No error";
    case GL_INVALID_ENUM:      return "Invalid enum";
    case GL_INVALID_VALUE:     return "Invalid value";
    case GL_INVALID_OPERATION: return "Invalid operation";
    case GL_STACK_OVERFLOW:    return "Stack overflow";
    case GL_STACK_UNDERFLOW:   return "Stack underflow";
    case GL_OUT_OF_MEMORY:     return "Out of memory";
    default:                   return "Unknown error";
    }
}

void checkGLError(){

  while (true){

      const GLenum error = glGetError();
      if (GL_NO_ERROR == error) break;

      std::cout << "GL Error: " << getGLErrorString(error) << std::endl;
    }
}
