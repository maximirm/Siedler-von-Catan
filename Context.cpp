#pragma once

/* ----------------------------------------------------------------
   name:           Context.cpp
   purpose:        OpenGL loading, context creation and initialization
   version:	   LIBRARY CODE
   author:         katrin lang
                   htw berlin
   ------------------------------------------------------------- */

#include "Context.hpp"
#include "GLIncludes.hpp"
#include "Application.hpp"

#include "freeglut.h"

#include "Window.hpp"

template<class Application> class Window* OpenGLContext<Application>::window = NULL;

template <class Application, class... Applications> void OpenGLContext<Application, Applications...>::init(int* argc, char** argv) {
  
  OpenGLContext<Application>::init(argc, argv);
  
  glutSetOption(GLUT_RENDERING_CONTEXT, GLUT_USE_CURRENT_CONTEXT);
  
  OpenGLContext<Applications...>::init(OpenGLContext<Application>::window);
}

template <class Application> void OpenGLContext<Application>::init(int *argc, char** argv){
  
  // create window with glut
  glutInit(argc, argv);
  
  glutInitContextVersion(Application::config.version[0], Application::config.version[1]);

  glutInitContextProfile(Application::config.profile==OpenGLConfiguration::CORE ? GLUT_CORE_PROFILE : GLUT_COMPATIBILITY_PROFILE);  

  glutSetOption(GLUT_MULTISAMPLE, Application::config.samples);
  
  glutInitDisplayMode(Application::config.flags);
  
  window = new Window(Application::config.title.c_str(), Application::config.position, Application::config.size);
  
  glewExperimental= GL_TRUE;
  
  if (glewInit() != GLEW_OK) {
    std::cerr << "Error initializing GLEW." << std::endl;
    exit(0);
  }

  Application::window = window;

  window->makeCurrent();
  
  Application::init();
}

template <class Application>  void OpenGLContext<Application >::init(Window* parent) {

  parent->makeCurrent();
  
  glutSetOption(GLUT_RENDERING_CONTEXT, GLUT_USE_CURRENT_CONTEXT);
  
  window= new Window(parent, Application::config.title.c_str(), Application::config.position, Application::config.size);
  
  Application::window= window;
  
  window->makeCurrent();
  
  Application::init();
}
