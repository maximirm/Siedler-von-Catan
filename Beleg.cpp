/* ---------------------------------------------------------------
   name:           Beleg.cpp
   purpose:        Beleg Template
   version:	   STARTER CODE
   author:         katrin lang
                   htw berlin
   ------------------------------------------------------------- */

#include "GLIncludes.hpp"

#include <iostream> 

#include "Beleg.hpp"
#include "Context.hpp"
#include "Input.hpp"

#include "LightSource.h"
#include "Material.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// use this with care
// might cause name collisions
using namespace glm;
using namespace std;

// field of view
GLfloat Beleg::Main::fov= 45.0;
GLfloat Beleg::Main::cameraZ= 3;

mat4 Beleg::Main::projectionMatrix, Beleg::Main::viewMatrix, Beleg::Main::modelMatrix(1);

LightSource Beleg::Main::lightSource={
    // position
  glm::vec4(0, 2, 3, 1),
  // ambient color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  // diffuse color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  // specular color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
};

void Beleg::Main::init(){
  
}

// adjust to new window size
void Beleg::Main::reshape(){
    
  // viewport
  glViewport(0, 0, (GLsizei) window->width(), (GLsizei) window->height());

  computeViewMatrix();
  computeProjectionMatrix();

  window->redisplay();
}

void Beleg::Main::computeViewMatrix(void){

  viewMatrix= glm::lookAt(vec3(0,0,cameraZ), vec3(0), vec3(0,1,0));
}

void Beleg::Main::computeProjectionMatrix(void){

  // reshaped window aspect ratio
  float aspect= (float) window->width() / (float) window->height();
  
  // compute near and far plane
  float nearPlane=cameraZ/10.0f; 
  float farPlane= cameraZ*10.0f;
  
  projectionMatrix= glm::perspective(radians(fov), aspect, nearPlane, farPlane);
}

// this is where the drawing happens
void Beleg::Main::display(void){

  glClearColor(0.0,0.0,0.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // display back buffer
  window->swapBuffers();
}

// Root Window

void Beleg::init(void){

  // set background color to black
  glPolygonMode(GL_FRONT, GL_FILL);
    
  // enable depth test (z-buffer)
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  // enable antialiasing
  glEnable(GL_MULTISAMPLE);
}

void Beleg::reshape(void){
  
  // Set the viewport to be the entire window
  glViewport(0, 0, window->width(), window->height());
  
  Left::reshape();
  Right::reshape();
  Main::reshape();
  
  window->redisplay();
}

// display texture on full screen quad
void Beleg::display(void){

  glClearColor(0.5, 0.5, 0.5, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  window->swapBuffers();
}

// Left Window

void Beleg::Left::init(void){

}

void Beleg::Left::reshape(void){
  
  // Set the viewport to be the entire window
  glViewport(0, 0, window->width(), window->height());

  window->redisplay();
}

// display texture on full screen quad
void Beleg::Left::display(void){

  glClearColor(0.3, 0.3, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  window->swapBuffers();
}

// Right Window

void Beleg::Right::init(void){

}

void Beleg::Right::reshape(void){
  
  // Set the viewport to be the entire window
  glViewport(0, 0, window->width(), window->height());

  window->redisplay();
}

// display texture on full screen quad
void Beleg::Right::display(void){

    glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    window->swapBuffers();
}


// keyboard control
void Beleg::keyPressed(){
  handleKeyboardInput(keyboard->key);
}
void Beleg::Main::keyPressed(){
  handleKeyboardInput(keyboard->key);
}
void Beleg::Left::keyPressed(){
  handleKeyboardInput(keyboard->key);
}
void Beleg::Right::keyPressed(){
  handleKeyboardInput(keyboard->key);
}

void Beleg::handleKeyboardInput(unsigned int key){

  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(key){
    
  case 'q':
  case 'Q':
    exit(0);

  default:
    break;
  }
}

// keyboard callback for special keys
// (arrow keys for node selection)
void Beleg::Main::specialKey(){

  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(keyboard->code) {

  case Keyboard::Code::UP:
    
    break;
  case Keyboard::Code::DOWN:
    
    break;
  case Keyboard::Code::LEFT:
    
    break;
  case Keyboard::Code::RIGHT:
    
    break;
  default:
    break;
  }
}

vector< pair < int, string > > Beleg::Main::menuEntries{{Menu::QUIT, "quit"}};

// mouse menu control
void Beleg::Main::menu(int id){
  
  switch (id) {
  case Menu::QUIT: 
    exit(0);

  default:
    break;
  }
}

int main(int argc, char** argv){

  // initialize OpenGL context
  OpenGLContext<Beleg, Beleg::Main, Beleg::Left, Beleg::Right>::init(&argc, argv);

  // some diagnostic output
  std::cout << "GPU: " << glGetString(GL_RENDERER) << ", OpenGL version: " << glGetString(GL_VERSION) << ", GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
  //start event loop
  InputManager<Beleg, Beleg::Main, Beleg::Left, Beleg::Right>::startEventLoop();
  
  return 0;
}
