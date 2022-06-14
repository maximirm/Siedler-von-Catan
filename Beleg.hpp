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
#include "TriangleMesh.hpp"
#include "Texture.hpp"
#include "LightSource.h"
#include "GLSL.hpp"

extern OpenGLConfiguration config;
extern OpenGLConfiguration configLeft;
extern OpenGLConfiguration configRight;
extern OpenGLConfiguration configMain;

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

private:
  
  class Island{

  private:
    glm::vec3 position;
    Texture texture;
    
  public:
    Island(std::string texture);
    void display(glm::mat4 modelMatrix);
  };

public:
  
  class Main : public OpenGLApplication<configMain>{

    friend class Beleg::Island;
    
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
    
    // ML schnipp
    static TriangleMesh mesh;
    static glsl::Shader diffuseShader, texturingShader;
    
    // ML schnapp
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
