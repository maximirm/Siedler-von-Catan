#ifdef __cplusplus
#pragma once
struct Material{
  std::string name;
  // ambient color
  glm::vec4 ambient;
  // diffuse color
  glm::vec4 diffuse;
  // specular color
  glm::vec4 specular;
  // shininess
  float shininess;
};

#else
struct Material{
  // ambient color
  vec4 ambient;
  // diffuse color
  vec4 diffuse;
  // specular color
  vec4 specular;
  // shininess
  float shininess;
};
#endif
