/* ----------------------------------------------------------------
   name:           Node.cpp
   purpose:        implementation for a scenegraph node
   version:	   STARTER CODE
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include <iostream>

#include "Node.hpp"
#include "Log.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/string_cast.hpp"
//#include "glm/gtc/type_ptr.hpp"

// use this with care
// might cause name collisions
using namespace glm;
using namespace std;

// constructor
// x,y,z: position of node center of mass relative to parent node center
// length, height, and width of this body part
// jointx, jointy, jointz: x,y,z offset of  rotation axis from center of body part
// i.e. for rotation about a "shoulder" as opposed to the center of mass of a cube
// x,y,z: rotation (in degrees) of node relative to parent node rotation
// radius: radius of virtual trackball
Node::Node(TriangleMesh *mesh,
	   Material material,
	   glsl::Shader *shader,
	   Texture *texture,
	   float x, float y, float z, 
	   float length, float height, float width,
	   float jointx, float jointy, float jointz,
	   float rotx, float roty, float rotz){
    
  selected= false;

  position= vec3(x,y,z);
    
  dimension= vec3(length, height, width);
    
  joint= vec3(jointx, jointy, jointz);
    
  rotation= vec3(rotx, roty, rotz);

  parent= NULL;
  child= NULL;
  previous= NULL;
  next= NULL;
  
  rotationMatrix= mat4(1);

  this->mesh= mesh;
  this->material= material;
  this->shader= shader;
  this->texture= texture;
  
  reset();
}

// destructor
Node::~Node(){
}

// sets the parent of this node
// and adds this node 
// to the list of the parent's children
void Node::setParent(Node* parent){
  
  this->parent= parent;
  if(parent->child==NULL){
    parent->child= this;
  }
  else{
    Node *sibling= parent->child;
    while(sibling->next != NULL) sibling= sibling->next;
    sibling->next= this;
    this->previous= sibling;
  }
}

// transform an individual node
// according to its position, 
// rotation, and rotation center
// and call draw()
  void Node::render(mat4 projectionMatrix, mat4 viewMatrix, mat4 &modelMatrix, LightSource lightSource){

  // note the order of the operations:
  // the transformations are applied in "reverse" order
  // of glRotate/glTranslate calls to the node

  // translate to final position
  // ML schnipp
  modelMatrix*= glm::translate(position);
  // ML schnapp
  
  // translate node center to joint position
  // ML schnipp
  modelMatrix*= glm::translate(joint);
  // ML schnapp
  
  // apply this node's rotation to transformation
  // ML schnipp
  modelMatrix*= rotationMatrix;
  // ML schnapp

  // ML schnipp
  modelMatrix*= glm::translate(-joint);
  // ML schnapp
  
  draw(projectionMatrix, viewMatrix, modelMatrix, lightSource);
}

// draw an individual node
void Node::draw(mat4 projectionMatrix, mat4 viewMatrix, mat4 modelMatrix, LightSource lightSource){

  // ML schnipp
  modelMatrix*= glm::scale(dimension);
  // ML schnapp
  
  shader->bind();
  shader->setUniform("transformation", projectionMatrix*viewMatrix*modelMatrix);
  shader->setUniform("lightPosition", inverse(modelMatrix)*lightSource.position);
  shader->setUniform("texture", texture->id());
  if(selected)
    shader->setUniform("color", vec4(1.0f, 0.3f, 0.3f, 1.0f));
  else
    shader->setUniform("color", material.diffuse);

  texture->bind();
  mesh->draw();
  texture->unbind();
  
  shader->unbind();
}

// increment / decrement rotation
void Node::rotate(float x, float y, float z){

  // preMultiply
  rotationMatrix*= glm::rotate(radians(x), vec3(1,0,0));
  rotationMatrix*= glm::rotate(radians(y), vec3(0,1,0));
  rotationMatrix*= glm::rotate(radians(z), vec3(0,0,1));
}

// getter for next pointer
Node *Node::getNext(){
  return next;
}

// getter for previous pointer
Node *Node::getPrevious(){
  return previous;
}

// getter for parent pointer
Node *Node::getParent(){
  return parent;
}

// getter for child pointer
Node *Node::getChild(){
  return child;
}

// select node
void Node::select(){
  selected= true;
}

// deselect node
void Node::deselect(){
  selected= false;
}

// resets an individual node
// to the initial rotation 
// specified in constructor
// XXX: NEEDS TO BE IMPLEMENTED
void Node::reset(){

  rotationMatrix= mat4(1);
  rotationMatrix*= glm::rotate(radians(rotation.x), vec3(1.0f,0.0f,0.0f));
  rotationMatrix*= glm::rotate(radians(rotation.y), vec3(0.0f,1.0f,0.0f));
  rotationMatrix*= glm::rotate(radians(rotation.z), vec3(0.0f,0.0f,1.0f));
}
