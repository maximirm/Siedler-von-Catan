/* ----------------------------------------------------------------
   name:           Window.cpp
   purpose:        GLUT (sub-) window implementation
   version:	   LIBRARY CODE
   TODO:           nothing
   author:         katrin lang
		   computer graphics
		   tu berlin
   ------------------------------------------------------------- */

#include "freeglut.h"

#include "Window.hpp"

using namespace std;
using namespace glm;

const unsigned int Window::RGBA= GLUT_RGBA, Window::RGB= GLUT_RGB, Window::DOUBLE=GLUT_DOUBLE, Window::DEPTH=  GLUT_DEPTH, Window::MULTISAMPLE= GLUT_MULTISAMPLE;

// main window
Window::Window(string title, uvec2 position, uvec2 size) :

  title(title){
  
  glutInitWindowPosition(position.x, position.y);
  glutInitWindowSize(size.x, size.y);
  id= glutCreateWindow(title.c_str());
  assert(id);
}

Window::Window(Window *parent, string title, uvec2 position, uvec2 size) :

  title(title){
  id= glutCreateSubWindow(parent->id, position.x, position.y, size.x, size.y);
  assert(id);
}

// get width
unsigned int Window::width(){
  if(!isCurrent()) makeCurrent();
  return glutGet(GLUT_WINDOW_WIDTH);
}

// get height
unsigned int Window::height(){
  if(!isCurrent()) makeCurrent();
  return glutGet(GLUT_WINDOW_HEIGHT);
}

// get size
uvec2 Window::size(){
  if(!isCurrent()) makeCurrent();
  return vec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// make window current
void Window::makeCurrent(void){

  glutSetWindow(id);
}

// is current?
bool Window::isCurrent(void) const{

  return glutGetWindow()==(int)id;
}

string Window::getTitle(void) const{

  return title;
}

// TODO: restore current window
void Window::redisplay(void){
  
  if(!isCurrent()) makeCurrent();
  glutPostRedisplay();
}

void Window::swapBuffers(void){

  glutSwapBuffers();
}

// reshape window
void Window::reshape(int width, int height){
  
  if(!isCurrent()) makeCurrent();
  glutReshapeWindow(width, height);
}

// reposition window
void Window::reposition(int x, int y){
  
  if(!isCurrent()) makeCurrent();
  glutPositionWindow(x, y);
}
