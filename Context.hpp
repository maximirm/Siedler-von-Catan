#pragma once

#include "Window.hpp"

/* ----------------------------------------------------------------
   name:           Context.hpp
   purpose:        OpenGL loading, context creation and initialization
   version:	   LIBRARY CODE
   TODO:           nothing
   author:         katrin lang
   computer graphics
   htw berlin
   ------------------------------------------------------------- */

// Primary Template
template<class...  Applications> class OpenGLContext {

    // recursion anchor
    static void init(Window* window) {}
};

template <class Application, class... Applications> class OpenGLContext<Application, Applications...> {

public:

    static void init(int* argc, char** argv);
  
    // recursion
    static void init(Window* window) {
        OpenGLContext<Application>::init(window);
        OpenGLContext<Applications...>::init(window);
    }
};
 
// The actual class for a (possibly shared) OpenGL Context that contains the respective window
template<class Application> class OpenGLContext<Application> {

public:

  enum Profile { CORE, COMPATIBILITY };
  
  static void init(int* argc, char** argv);
  
  static void init(Window* window);
  
  static Window* window;
};


#include "Context.cpp"

