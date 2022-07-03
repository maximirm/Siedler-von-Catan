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
#include <unordered_map>
#include <iterator> 
#include <iostream>
#include <algorithm>

#include "gl.h"

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

  // draw the model
  void draw(void);

  void draw(unsigned int);
  
  std::pair<glm::vec3, glm::vec3> getBoundingBox(void);
  float getBoundingSphereRadius(void);

  Material getMaterial(std::string name);
  
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

public:
  
  struct Range{

    Range(unsigned int begin){
      this->begin= begin;
      end= std::numeric_limits<unsigned long>::max();
    }

    unsigned long begin;
    unsigned long end;
  };

  struct Group;

  struct Segment{
     
    std::string material;
    std::string texture;

    std::vector<glm::uvec3> elements;
    std::vector<Range> ranges;

    void draw();

    ~Segment();
    
  private:

    friend class Group;
    friend class TriangleMesh;

    GLuint ibo;
    GLuint vao;
    
    Segment(unsigned long index, std::string material, std::string texture){
      this->material= material;
      this->texture= texture;
      begin(index);
    }

    void begin(unsigned long index){
      Range range(index);
      ranges.push_back(range);
    }

    void end(unsigned long index){
      if(ranges.back().end>index) ranges.back().end= index;
    }

    void computeElements(const std::vector<glm::uvec3> &triangles){
      for(int i=0; i<ranges.size(); i++){
	for(int j= ranges[i].begin; j<ranges[i].end; j++){
	  elements.push_back(triangles[j]);
	}
      }
    }

    void uploadIndexBuffer();
    void createVAO(unsigned int vbob[]);
  };
  
  struct Group{
    
    std::string name;

    std::vector<Segment>::iterator begin(){ return segments.begin(); };
    std::vector<Segment>::iterator end(){ return segments.end(); };

  private:

    friend class TriangleMesh;
    
    std::vector<Segment> segments;
    std::vector<Segment>::iterator current;
    
    Group(unsigned long index, std::string name, std::string material, std::string texture){
      this->name= name;
      segments.push_back(Segment(index, material, texture));
      current= segments.begin();
    }
    
    void begin(unsigned long index){
      current->begin(index);
    }
    
    void end(unsigned long index){
      current->end(index);
    }
    
    void addSegment(unsigned long index, std::string material, std::string texture){

      if((segments.size()==1 && current->ranges.front().begin == index) || current->ranges.back().end < index){
      	current->material= material;
	current->texture= texture;
      }
      else{
	current->end(index);
	current= find_if(segments.begin(), segments.end(), [&] (Segment s) {return s.material==material && s.texture==texture;});
	if(current == segments.end()){
	  segments.push_back(Segment(index, material, texture));
	  current= --segments.end();
	}
	else{
	  current->begin(index);
	}
      }
    }
    
    void computeElements(const std::vector<glm::uvec3> &triangles){
      for(int i= 0; i<segments.size(); i++) segments[i].computeElements(triangles);
    }
  };

  std::vector<Group>::iterator begin(){ return groups.begin(); };
  std::vector<Group>::iterator end(){ return groups.end(); };

protected:
  
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
  void uploadVertexBuffers(void);

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
  
  // buffer ids
  GLuint vbo[3];

  std::vector<glm::uvec3> texCoordIndices;
  std::vector<glm::uvec3> normalIndices;
  std::vector<Group> groups;
  std::unordered_map<std::string, Material> materials;
};
