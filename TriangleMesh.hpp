#pragma once

/* ----------------------------------------------------------------
   name:           TriangleMesh.cpp
   purpose:        triangle mesh for OpenGL rendering
   version:	   SKELETON CODE
   TODO:           nothing (see TriangleMesh.cpp)
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include <string>
#include <vector>

#include "GLIncludes.hpp"

// OpenGL mathematics library
// http://glm.g-truc.net
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include "Material.h"

/*
 * Class for a simple triangle mesh represented as an indexed face set
 */
class TriangleMesh{

public:

  // default constructor
  TriangleMesh();

  // destructor
  ~TriangleMesh();

  // clockwise / counter-clockwise?
  enum PolygonWinding{
    CW,
    CCW
  };

  void setWinding(PolygonWinding winding);
  
  // load the mesh
  void load(const std::string& filename, bool unitize= true, bool center= true);

  // reload the mesh
  void reload();

  // draw the model
  void draw(void);
  
  // vertex attribute bindings
  // see https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/attributes.php
  static const GLuint attribPosition;
  static const GLuint attribNormal;
  static const GLuint attribColor;
  static const GLuint attribTexCoord;

protected:

  // load the mesh from an off file
  void loadOff(const std::string& filename);
 // load the mesh from an obj file
  void loadObj(const std::string& filename);
  
    // triangulate a polygon
  void triangulate(std::vector<GLuint> positionIndices, std::vector<GLuint> texCoordIndices, std::vector<GLuint> normalIndices);
  // bring to format opengl eats
  // (no shared normals / texture coordinates)
  void clean(void);
  // parse a material library	
  void parseMaterials(std::string filename);
  // find material
  unsigned int materialIndex(std::string name);
  // add material to group
  void addMaterial(std::string group, std::string material);

  typedef struct group{
    std::string name;
    unsigned long index;
    std::string material;
  } Group;
  
  // normalize to bounding sphere radius 1
  void unitize(void);
  // center model
  void center(void);
  // compute bounding sphere
  void computeBoundingSphere(void);
  // compute bounding box
  void computeBoundingBox(void);

  // compute texture coordinates
  void computeSphereUVs(void);  
  // compute the normals of the vertices
  void computeNormals(void);

  // release vbo memory on gpu
  void clearBuffers(void);

  // generate vertex buffer objects
  void uploadBuffers(void);

  std::string name;
  
  // Position of the vertices
  std::vector<glm::vec3> positions;
  // normals of the vertices
  std::vector<glm::vec3> normals, normalsRAW;
  // texture coordinates of the vertices
  std::vector<glm::vec2> texCoords, texCoordsRAW;
  // indices of the faces
  std::vector<glm::uvec3> faces;

  PolygonWinding winding;

  float boundingSphereRadius;
  // two opposite corners of bounding box 
  glm::vec3 boundingBoxMin;
  glm::vec3 boundingBoxMax;

  GLuint vao;
  
  // buffer ids
  GLuint vbo[3];
  GLuint ibo;

  std::vector<glm::vec2> texCoordIndices;
  std::vector<glm::vec3> normalIndices;
  std::vector<Group> groups;
  std::vector<Material> materials;
};
