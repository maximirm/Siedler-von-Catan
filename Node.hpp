#pragma once

/* ----------------------------------------------------------------
   name:           Node.h
   purpose:        class declaration for a scenegraph node
   version:	   SKELETON CODE
   TODO:           nothing (see Node.cpp)
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include "glm/glm.hpp"

#include "Texture.hpp"
#include "TriangleMesh.hpp"
#include "GLSL.hpp"

#include "Material.h"
#include "LightSource.h"

// NOTE: each node in the scenegraph can have multiple 
//       children/siblings by pointing to its first child via 
//       the 'child' pointer and then linking up the children 
//       using the 'sibling' pointers (previous and next). 
//       in the following example diagram, N = node
//
//
//       the diagram/linking:
//
//                    N (root node of this example scenegraph)
//                   /
//               child
//                /
//               N - sibling - N - sibling - N
//              /             /
//          child         child        
//           /             /
//          N             N - sibling - N
// 
//
//       represents the following scenegraph/tree:
//
//
//                    N (root node of this example scenegraph)
//                   /|\
//                 /  |  \
//               N    N   N
//              /    /|
//            /    /  |
//           N    N   N
//

class Node{

public:

  // constructor
  // x,y,z: position of node center of mass relative to parent node center
  // length, height, and width of this body part
  // jointx, jointy, jointz: x,y,z offset of  rotation axis from center of body part
  // i.e. for rotation about a "shoulder" as opposed to the center of mass of a glutsolidcube
  // x,y,z: rotation (in degrees) of node relative to parent node rotation
  Node(TriangleMesh *mesh,
       Material material,
       glsl::Shader *shader,
       Texture *texture,
       float x, float y, float z, 
       float length, float height, float width,
       float jointx, float jointy, float jointz,
       float rotx, float roty, float rotz);

  // destructor
  ~Node();
  
  // sets the parent of this node
  void setParent(Node* parent);
  
protected:

  void getMatrices(void);
  
  // scenegraph has access to protected functions
  friend class SceneGraph;
  
  // transform an individual node
  // according to its position, 
     // rotation, and rotation center
     // and call draw() methods
    void render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 &modelMatrix, LightSource lightSource);
  
  // draw an individual node
  void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix, LightSource lightSource);

  // increment / decrement rotation
  void rotate(float x, float y, float z);

  // return parent / child / siblings
  Node *getNext();
  Node *getPrevious();
  Node *getParent();
  Node *getChild();

  // select / deselect node
  void select();
  void deselect();

  // reset rotation
  void reset();
  
  glm::vec3 position;   // x,y,z position of
                        // node center relative
                        // to parent node center
  
  glm::vec3 dimension;  // length, height,
                        // and width 
                        // of this body part
  
  glm::vec3 joint;      // x,y,z offset of 
         		// rotation axis
                   	// from center of body part
                        // i.e. for rotation about
                        // a "shoulder" as opposed
                        // to the center of mass
			// of a glutsolidcube

  glm::vec3 rotation;  // initial x,y,z rotation
                       // (in degrees) of node relative
                       // to parent node rotation
  
  glm::mat4 rotationMatrix;
  
  // is node selected?
  bool selected;
  
  // siblings
  Node* next;
  Node* previous;
  // parent / child node
  Node* parent;
  Node* child;

  TriangleMesh *mesh;
  Material material;
  glsl::Shader *shader;
  Texture *texture;
};
