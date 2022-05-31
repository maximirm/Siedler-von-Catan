#include "Config.hpp"

OpenGLConfiguration::OpenGLConfiguration(glm::uvec2 version, enum OpenGLConfiguration::Profile profile, unsigned int flags, unsigned int samples, glm::uvec2 position, glm::uvec2 size, std::string title)
  : version(version), profile(profile), flags(flags), samples(samples), position(position), size(size), title(title){}

OpenGLConfiguration::OpenGLConfiguration(OpenGLConfiguration &other, glm::uvec2 position, glm::uvec2 size, std::string title)
  : version(other.version), profile(other.profile), flags(other.flags), samples(other.samples), position(position), size(size), title(title){}
