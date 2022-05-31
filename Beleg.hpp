#pragma once

/* ----------------------------------------------------------------
   name:           Control.hpp
   purpose:        robot control
   version:	   SKELETON CODE
   TODO:           add more callbacks (see Application.hpp)
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include <vector>
// for pair
#include <utility>

#include "glm/glm.hpp"

#include "Context.hpp"
#include "Application.hpp"
#include "Material.h"
#include "LightSource.h"

 OpenGLConfiguration config(glm::uvec2(2, 1),
			    OpenGLConfiguration::Profile::COMPATIBILITY,
			    (Window::DOUBLE | Window::DEPTH | Window::RGB | Window::MULTISAMPLE),
			    8,
			    glm::uvec2(50, 50),
			    glm::uvec2(512, 768),
			    "Computer Graphics - Beleg");

 OpenGLConfiguration configLeft(config,
				glm::uvec2(0, 0),
				glm::uvec2((config.size.x)/2, config.size.y/3),
				"");

OpenGLConfiguration configRight(config,
				glm::uvec2((config.size.x)/2, 0),
				glm::uvec2((config.size.x)/2, config.size.y/3),
				"");

OpenGLConfiguration configMain(config,
			       glm::uvec2(0, config.size.y/3),
			       glm::uvec2(config.size.x, 2*(config.size.y/3)),
			       "");

class Beleg : public OpenGLApplication<config>{

public:
  
  // initialization
  static void init();

  // display scene
  static void display(void);
  
  // called after window rehape
  static void reshape();
  
  // keyboard callback
  static void keyPressed();
  
  class Main : public OpenGLApplication<configMain>{
    
  public:
    
    // initialization
    static void init();
    
    // display scene
    static void display(void);
    
    // called after window rehape
    static void reshape();
    
    // called when menu item was selected          
    static void menu(int id);
    
    // keyboard callback
    static void keyPressed();
    
    // keyboard callback for special keys 
    static void specialKey();
    
    // menu entries
    static std::vector< std::pair< int, std::string > >  menuEntries;
    
  private:
    
    static void computeViewMatrix(void);
    static void computeProjectionMatrix(void);
    
    // field of view (in degrees)                                               
      static GLfloat fov;
    
    static float pan;
    
    // camera position                                                           
    static float cameraZ;
    
    // near and far plane                                                        
    static float nearPlane, farPlane;
    
    static glm::mat4 projectionMatrix;
    static glm::mat4 viewMatrix;
    static glm::mat4 modelMatrix;
    
    static LightSource lightSource;
    static Material material;
    
    struct Menu{
      enum Item{QUIT};
    };
  };
  
  class Left : public OpenGLApplication<configLeft>{
    
  public:
    
    // initialization
    static void init();
    
    // display scene
    static void display(void);
    
    // called after window rehape
    static void reshape();
    
    // keyboard callback
    static void keyPressed();
    
  };
  
  class Right : public OpenGLApplication<configRight>{
      
  public:
    
    // initialization
    static void init();
    
    // display scene
    static void display(void);
    
    // called after window rehape
    static void reshape();
    
    // keyboard callback
    static void keyPressed();
  };

  private:
  
   static void handleKeyboardInput(unsigned int key);
};
