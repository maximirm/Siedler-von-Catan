/* ----------------------------------------------------------------
   name:           GLSL.hpp
   purpose:        cg1_ex3 OpenGL shading language wrapper
   version:	   LIBRARY CODE
   TODO:           nothing
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#pragma once 

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

#include "GLIncludes.hpp"

#include "glm/glm.hpp"

namespace glsl{

  class Shader{

  public:
  
    Shader();
    ~Shader();

    // load and compile vertex shader from file
    void loadVertexShader(const std::string& fileName);
    // load and compile geometry shader from file
    void loadGeometryShader(const std::string& fileName);
    // load and compile fragment shader from file
    void loadFragmentShader(const std::string& fileName);

    // add vertex shader directly from source
    void addVertexShader(const std::string& code);
    // add geometry shader from directly from source
    void addGeometryShader(const std::string& code);
    // add fragment shader from directly from source
    void addFragmentShader(const std::string& code);

    // compile vertex shader
    void compileVertexShader(void);
    // compile geometry shader
    void compileGeometryShader(void);
    // compile fragment shader
    void compileFragmentShader(void);

    void setGeometryShaderInOut(GLuint in, GLuint out, int count);
  
    // link Shader
    void link(void);

    // Bind the shader to the openGL pipeline
    void bind()const;
  
    // unbind the shader
    void unbind()const;

    // ------------------------------- set uniform ----------------------------------------
    // ---------------------- overloaded for all possible types ---------------------------
  
    // Set a float uniform variable
    void setUniform(const std::string& name, const float& value);
  
    // Set an integer uniform variable
    void setUniform(const std::string& name, const int& value);

    // Set an unsigned integer uniform variable
    void setUniform(const std::string& name, const unsigned int& value);

    // Set a 2 component float vector uniform parameter
    void setUniform(const std::string& name, const float& x, const float& y);
  
    // Set a 2 component integer vector uniform parameter
    void setUniform(const std::string& name, const int& x, const int& y);
  
    // Set a 2 component unsigned integer vector uniform parameter
    void setUniform(const std::string& name, const unsigned int& x, const unsigned int& y);

    // Set a 3 component float vector uniform parameter
    void setUniform(const std::string& name, const float& x, const float& y, const float& z);
  
    // Set a 3 component integer vector uniform parameter
    void setUniform(const std::string& name, const int& x, const int& y, const int& z);
   
    // Set a 3 component unsigned integer vector uniform parameter
    void setUniform(const std::string& name, const unsigned int& x, const unsigned int& y, const unsigned int& z);
 
    // Set a 4 component float vector uniform parameter
    void setUniform(const std::string& name, const float& x, const float& y, const float& z, const float& w);
  
    // Set a 4 component integer vector uniform parameter
    void setUniform(const std::string& name, const int& x, const int& y, const int& z, const int& w);
  
    // Set a 4 component unsigned integer vector uniform parameter
    void setUniform(const std::string& name, const unsigned int& x, const unsigned int& y, const unsigned int& z, const unsigned int& w);

    // Set a 2 component float vector uniform parameter
    void setUniform(const std::string& name, const glm::vec2& value);
  
    // Set a 2 component integer vector uniform parameter
    void setUniform(const std::string& name, const glm::ivec2& value);

    // Set a 2 component unsigned integer vector uniform parameter
    void setUniform(const std::string& name, const glm::uvec2& value);

    // Set a 3 component float vector uniform parameter
    void setUniform(const std::string& name, const glm::vec3& value);
  
    // Set a 3 component integer vector uniform parameter
    void setUniform(const std::string& name, const glm::ivec3& value);

    // Set a 3 component unsigned integer vector uniform parameter
    void setUniform(const std::string& name, const glm::uvec3& value);
    
    // Set a 4 component float vector uniform parameter
    void setUniform(const std::string& name, const glm::vec4& value);
  
    // Set a 4 component integer vector uniform parameter
    void setUniform(const std::string& name, const glm::ivec4& value);
  
    // Set a 4 component unsigned integer vector uniform parameter
    void setUniform(const std::string& name, const glm::uvec4& value);

    // Set a 2x2 matrix uniform parameter
    void setUniform(const std::string& name, const glm::mat2& value);

    // Set a 2x3 matrix uniform parameter
    void setUniform(const std::string& name, const glm::mat2x3& value);
  
    // Set a 3x3 matrix uniform parameter
    void setUniform(const std::string& name, const glm::mat3& value);
  
    // Set a 4x4 matrix uniform parameter
    void setUniform(const std::string& name, const glm::mat4& value);

    // bind an attribute to the same id used in glVertexAttrib* call
    void bindVertexAttrib(const std::string& name, const int index);
  
    // get vertex attribute binding to be used in glVertexAttrib* call
    int getVertexAttribBinding(const std::string& name);

    // only needs to be called when rendering to multiple target
    void bindOutput(const std::string& name, int id);

    // for debugging
    GLint getUniformBinding(const std::string& name);

    //transform feedback (goes into separate buffers)
    void activateFeedbackVaryings(const std::vector< std::string > varyings);
  
  private:

    // load a code string from a source file
    std::string load(const std::string& file);

    // Create a shader of type stype from a source
    GLuint compile(GLuint type, const std::vector<std::string>& code);
  
    // check for error
    bool hasErrors(GLuint shader);
  
    // Print the info log for a program if the status is not OK.
    bool printProgramLog(GLuint program);
  
    // Print the info log for a shader if the status is not OK.
    void printShaderLog(GLuint shader);
  
    // OpenGL handles to the vertex shaders
    std::vector<GLuint> vertexShaders;
    // OpenGL handles to the geometry shaders 
    std::vector<GLuint> geometryShaders;
    // OpenGL handles to the fragment shaders
    std::vector<GLuint> fragmentShaders;	
    // OpenGL handle to the whole shader
  
    GLuint program;

    // hash table for uniform bindings
    // since retrieving from shader each time is costly
    std::map<std::string, GLint> attributeBindings;

    std::vector<std::string> vertexCode;
    std::vector<std::string> geometryCode;
    std::vector<std::string> fragmentCode;
  };
};
