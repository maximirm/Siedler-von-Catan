#ifdef __cplusplus
#pragma once
struct LightSource{
  //position in view space
  glm::vec4 position;
  // ambient color
  glm::vec4 ambient;
  // diffuse color
  glm::vec4 diffuse;
  // specular color
  glm::vec4 specular;
};
#else
struct LightSource{
  //position in view space
  vec4 position;
  // ambient color
  vec4 ambient;
  // diffuse color
  vec4 diffuse;
  // specular color
  vec4 specular;
};
#endif
