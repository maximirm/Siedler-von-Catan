#pragma once

/* ----------------------------------------------------------------
   name:           InputManager
   purpose:        input handling, glut callbacks
   version:	   LIBRARY CODE
   TODO:           nothing
   author:         katrin lang
                   computer graphics
                   htw berlin
   ------------------------------------------------------------- */

/* --------------------------------------------------------------- 
   implemented glut callbacks:
   display            - display scene
   reshape            - reshape window
   menu               - right mouse button menu
   mouseButton        - mouse pressed / released
   mouseMoved         - mouse moved
   mouseDragged       - mouse moved while mouse button pressed
   keyPressed         - keyboard callback
   idle               - called each frame without input
   timer              - called when timer expires
   ---------------------------------------------------------------- */

#include <string>

#include <iostream> 

#include <vector>
// for pair
#include <utility>

#include "glm/glm.hpp"

#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Context.hpp"

using namespace std;

// Primary template
template<class... Applications> class InputManager{

public:
  
  // recursion anchor
  static void registerCallbacks(void){}
};

template <class Application, class... Applications> class InputManager<Application, Applications...> {

public:

	static void startEventLoop(void) {

		InputManager<Applications...>::registerCallbacks();
		InputManager<Application>::startEventLoop();
	}
  
  	// recursion
	static void registerCallbacks(void) {

		InputManager<Application>::registerCallbacks();
		InputManager<Applications...>::registerCallbacks();
	}

};

// actual implementation of InputManager
template <class Application> class InputManager<Application>{

public:

  static void startEventLoop(void);

  // select glut bitmap font
  static void setFont(const std::string& name, const int size);
  static void drawString(float x, float y, const std::string& s);
  static void drawString(float x, float y, float z, const std::string& s);

  static void registerCallbacks(void);

  // reshape callback
  static void reshape(int width, int height);

  // display callback
  static void display();
  
  // mouse callbacks
  static void mouseButton(int button, int state, int x, int y);
  static void mouseMoved(int x, int y);
  static void mouseDragged(int x, int y);

  static void menu(int item);
  
  // keyboard callbacks
  static void keyPressed(unsigned char key, int x, int y);
  static void keyReleased(unsigned char key, int x, int y);
  static void specialKeyPressed(int key, int x, int y);
  static void specialKeyReleased(int key, int x, int y);

  static void retrieveModifiers(void);

  // idle callback
  static void idle(void);
  
  static Keyboard keyboard;
  static Mouse mouse;
  static GLvoid *font;
};

#include "Input.cpp"
