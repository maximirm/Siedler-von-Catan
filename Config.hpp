#pragma once

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include <string>

struct OpenGLConfiguration{
    
  glm::uvec2 version;
  enum Profile{CORE, COMPATIBILITY} profile;
  unsigned int flags;
  unsigned int samples;
  glm::uvec2 position;
  glm::uvec2 size;
  std::string title;
  
  OpenGLConfiguration(glm::uvec2 version, Profile profile, unsigned int flags, unsigned int samples, glm::uvec2 position, glm::uvec2 size, std::string title);
  OpenGLConfiguration(OpenGLConfiguration &other, glm::uvec2 position, glm::uvec2 size, std::string title);
};
