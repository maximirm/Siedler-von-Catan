#include "freeglut.h"

template <OpenGLConfiguration &c> Window *OpenGLApplication<c>::window= NULL;
template <OpenGLConfiguration &c> Mouse *OpenGLApplication<c>::mouse= NULL;
template <OpenGLConfiguration &c> Keyboard *OpenGLApplication<c>::keyboard= NULL;


template <OpenGLConfiguration &c> std::vector< std::pair<unsigned int, std::string> > OpenGLApplication<c>::menuEntries;

template <OpenGLConfiguration &c> OpenGLConfiguration &OpenGLApplication<c>::config(c);

template <OpenGLConfiguration &c> void OpenGLApplication<c>::init(void){

}

// all callbacks that are not overriden deregister themselves

template <OpenGLConfiguration &c> void OpenGLApplication<c>::display(void){
  glutDisplayFunc(NULL);
}

template <OpenGLConfiguration &c> void OpenGLApplication<c>::reshape(void){
  glutReshapeFunc(NULL);
}

template <OpenGLConfiguration &c> void OpenGLApplication<c>::mousePressed(void){
  glutMouseFunc(NULL);
}

template <OpenGLConfiguration &c> void OpenGLApplication<c>::mouseMoved(void){
  glutPassiveMotionFunc(NULL);
}

template <OpenGLConfiguration &c> void OpenGLApplication<c>::mouseDragged(void){
   glutMotionFunc(NULL);
}

template <OpenGLConfiguration &c> void OpenGLApplication<c>::mouseReleased(void){

}

template <OpenGLConfiguration &c> void OpenGLApplication<c>::keyPressed(void){
  glutKeyboardFunc(NULL);
  glutSpecialFunc(NULL);
}

template <OpenGLConfiguration &c> void OpenGLApplication<c>::keyReleased(void){
  glutKeyboardUpFunc(NULL);
  glutSpecialUpFunc(NULL);
}

template <OpenGLConfiguration &c> void OpenGLApplication<c>::idle(void){
 glutIdleFunc(NULL);
}

template <OpenGLConfiguration &c> void OpenGLApplication<c>::menu(int){}
