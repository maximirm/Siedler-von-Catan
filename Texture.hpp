/* ----------------------------------------------------------------
   name:           Texture.hpp
   purpose:        cg_ex4 texturing tutorial 
   version:	   SKELETON CODE
   TODO:           nothing (see Texture.cpp)
   author:         katrin lang
                   computer graphics
                   htw berlin
   ------------------------------------------------------------- */

#pragma once

#include "GLIncludes.hpp"

#include <vector>
#include <string>

#include "glm/glm.hpp"

class Texture{

public:

  // constructors
  Texture();
  Texture(int width, int height);

  // destructor
  ~Texture();

  // load Texture from file
  void load(const std::string& filename);

  // set texture filter
  void setMinFilter(GLuint min);
  void setMagFilter(GLuint mag);

  // set wrapping mode
  void setWrapS(GLuint wrap);
  void setWrapT(GLuint wrap);
  // set both S and T
  void setWrap(GLuint wrap);

  // bind/unbind texture
  void bind();
  void unbind();

  // return textureID 
  // which can be passed to shader 
  // as uniform
  GLuint id(void);

  // generate OpenGL texture
  void generateTexture();

protected:

  // Texture data
  std::vector<glm::vec4> data;
  // dimensions
  int width;
  int height;

  GLuint textureID;

  //texturing parameters
  GLuint wrapS;
  GLuint wrapT;
  GLuint mag;
  GLuint min;

  // parse ppm format
  void loadPPM(const std::string& filename);
};
