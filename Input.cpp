#pragma once

/* ----------------------------------------------------------------
   name:           InputManager.cpp
   purpose:        input handling, glut callbacks
   version:	   LIBRARY CODE
   TODO:           nothing
   author:         katrin lang
   computer graphics
   htw berlin
   ------------------------------------------------------------- */

#include <string>

#include <vector>
// for pair
#include <utility>

#include "freeglut.h"

#include "Input.hpp"

// use this with care
// might cause name collisions
using namespace std;
using namespace glm;

template<class Application> Keyboard InputManager<Application>::keyboard;
template<class Application> Mouse InputManager<Application>::mouse;
// glut bitmap font 
// see setFont
template<class Application> GLvoid *InputManager<Application>::font= GLUT_BITMAP_HELVETICA_12;

template <class Application> void InputManager<Application>::registerCallbacks(void){

  Application::window->makeCurrent();
  
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glutMouseFunc(mouseButton);
  glutPassiveMotionFunc(mouseMoved);
  glutMotionFunc(mouseDragged);
  glutKeyboardFunc(keyPressed);
  glutKeyboardUpFunc(keyReleased);	
  glutSpecialFunc(specialKeyPressed);
  glutSpecialUpFunc(specialKeyReleased);
  glutIdleFunc(idle);

  if(!Application::menuEntries.empty())
    glutCreateMenu(Application::menu);
  for(unsigned int i= 0; i<Application::menuEntries.size(); i++) glutAddMenuEntry(Application::menuEntries[i].second.c_str(), Application::menuEntries[i].first);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  Application::mouse = &InputManager<Application>::mouse;
  Application::keyboard = &InputManager<Application>::keyboard;
}

template <class Application> void InputManager<Application>::startEventLoop(void){
  
  registerCallbacks();
  
  glutMainLoop();
}

template <class Application> void InputManager<Application>::display(void){
  Application::display();
}

// mouse pressed callback
template <class Application> void InputManager<Application>::mouseButton(int button, int state, int x, int y){
  
  retrieveModifiers();
  
  if (state == GLUT_UP){
    
    mouse.movement= ivec2(x, y)-mouse.position;
    mouse.position = ivec2(x, y);	
    Application::mouseReleased();
  }
  else if (state == GLUT_DOWN){

    mouse.movement= ivec2(0);
    mouse.position = ivec2(x, y);	

    mouse.button= button == GLUT_LEFT ? Mouse::LEFT : Mouse::RIGHT;
    
    Application::mousePressed();
  }
}

template <class Application> void InputManager<Application>::reshape(int width, int height){
  //  if(Application::window->isPrimary());
  Application::reshape();
}

template <class Application> void InputManager<Application>::mouseMoved(int x, int y){
  
  if(x == mouse.position.x && y == mouse.position.y) return;
  mouse.movement = ivec2(x, y) - mouse.position;
  mouse.position = ivec2(x, y);	
  retrieveModifiers();
  Application::mouseMoved();
}

template <class Application> void InputManager<Application>::mouseDragged(int x, int y){

  if (x == mouse.position.x && y == mouse.position.y) return;
  mouse.movement = ivec2(x, y) - mouse.position;
  mouse.position = ivec2(x, y);
 
  retrieveModifiers();
  Application::mouseDragged();
}

template <class Application> void InputManager<Application>::keyPressed(unsigned char key, int x, int y){

  retrieveModifiers();

  keyboard.pressed= true;
  keyboard.key= static_cast<typename Keyboard::Key>(key);

  mouse.position= uvec2(x, y);

  Application::keyPressed();
}

template <class Application> void InputManager<Application>::retrieveModifiers(void){

  keyboard.modifiers= glutGetModifiers();
}

template <class Application> void InputManager<Application>::specialKeyPressed(int key, int x, int y){
  
  retrieveModifiers();
  
  keyboard.pressed= true;
  keyboard.key= Keyboard::Key::SPECIAL;

  switch (key) {
      
  case GLUT_KEY_UP:
    keyboard.code= Keyboard::Code::UP;
    break;
  case GLUT_KEY_DOWN:
    keyboard.code= Keyboard::Code::DOWN;
    break;
  case GLUT_KEY_LEFT:
    keyboard.code= Keyboard::Code::LEFT;
    break;
  case GLUT_KEY_RIGHT:
    keyboard.code= Keyboard::Code::RIGHT;	
    break;
  default: return;
  }

  mouse.position = uvec2(x, y);	
  
  Application::keyPressed();
}

template <class Application> void InputManager<Application>::keyReleased(unsigned char key, int x, int y){

  keyboard.pressed= false;
  keyboard.key= static_cast<typename Keyboard::Key>(key);
  
  mouse.position = uvec2(x, y);	
  
  Application::keyReleased();
}

template <class Application> void InputManager<Application>::specialKeyReleased(int key, int x, int y) {

  keyboard.pressed = false;
  keyboard.key= Keyboard::Key::SPECIAL;

  switch (key) {
    
  case GLUT_KEY_UP:
    keyboard.code= Keyboard::Code::UP;
    break;
  case GLUT_KEY_DOWN:
    keyboard.code = Keyboard::Code::DOWN;
    break;
  case GLUT_KEY_LEFT:
    keyboard.code = Keyboard::Code::LEFT;
    break;
  case GLUT_KEY_RIGHT:
    keyboard.code = Keyboard::Code::RIGHT;
    break;
  default: break;
  }
  
  mouse.position= uvec2(x, y);
  
  Application::keyReleased();
}

template <class Application> void InputManager<Application>::idle(void) {
  Application::idle();
}

// select glut bitmap font
template <class Application> void InputManager<Application>::setFont(const string& name, const int size){  
  
  if(name.compare("helvetica") == 0){
    if(size == 10)
      font= GLUT_BITMAP_HELVETICA_10;
    else if(size == 12) 
      font= GLUT_BITMAP_HELVETICA_12;
    else if(size == 18)
      font= GLUT_BITMAP_HELVETICA_18;
  } 
  else if(name.compare("times") == 0){
    if(size == 10)
      font= GLUT_BITMAP_TIMES_ROMAN_10;
    if(size == 24)
      font= GLUT_BITMAP_TIMES_ROMAN_24;
  } 
  else if(name.compare("8x13") == 0){
    font= GLUT_BITMAP_8_BY_13;
  }
  else if(name.compare("9x15") == 0) {
    font= GLUT_BITMAP_9_BY_15;
  }
}

// draw a string in 2D
template <class Application> void InputManager<Application>::drawString(float x, float y, const string& s){
  glRasterPos2f(x, y);
  for (size_t i= 0; i<s.length(); i++)
    glutBitmapCharacter(font, s[i]);
}

// draw a string in 3D
template <class Application> void InputManager<Application>::drawString(float x, float y, float z, const string& s){
  glRasterPos3f(x, y, z);
  for (size_t i= 0; i<s.length(); i++)
    glutBitmapCharacter(font, s[i]);
}
