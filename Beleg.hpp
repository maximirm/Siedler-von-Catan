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
#include "Input.hpp"

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

  static float cameraZ;
  static float cameraX;
  static float cameraY;

  

public:
    class Main;
  
  class Island{

      friend class Beleg::Main;


     

  private:
    glm::vec3 position;
    Texture texture;
    TriangleMesh* islandMesh;
    TriangleMesh* cubeMesh;
    bool texturing;
    bool lighting;
    TriangleMesh* cubeMeshLeft;


  public:
    Island(std::string texture, glm::vec3 position, TriangleMesh *meshPointer, bool lighting=true, bool texturing = true);
    void display(glm::mat4 modelMatrix);
    void setTexture(Texture* texture);
    void changeLighting();
    
    
  };

  class Button {
      friend class Beleg::Main;
      
  private:
      glm::vec3 position;
      glm::vec2 size;
      Texture* pressedTexture;
      Texture* defaultTexture;
      Texture texture;
      TriangleMesh* mesh;
      


  public:

      Button(glm::vec3 position, TriangleMesh* meshpointer, Texture* pressedTexture, Texture* defaultTexture, glm::vec2 size);
      void toggle();
      void display(glm::mat4 modelMatrix);
      bool checkPosition(glm::vec2 position2D);
      void setTexture(Texture* pressedTexture, Texture* defaultTexture);
      bool pressed;


  };

 


public:
  
  class Main : public OpenGLApplication<configMain>{

    friend class Beleg::Island;
    friend class Beleg;
    
  public:
    
    // initialization
    static void init();
    
    // display scene
    static void display(void);
    
    // called after window rehape
    static void reshape();
    
    // called when menu item was selected          
    static void menu(int id);

    static void mousePressed();
    
    // keyboard callback
    static void keyPressed();
    
    // keyboard callback for special keys 
    static void specialKey();

    //this is where the rolling happens
    static void rollDice(Button *dice);

    static void setSelectedDecoration();

    static void markSelectedDecoration();
    
    // menu entries
    static std::vector< std::pair< int, std::string > >  menuEntries;

    static Mouse* mouse;
    
  private:
    
    static void computeViewMatrix(void);
    static void computeProjectionMatrix(void);
    
    // field of view (in degrees)                                               
      static GLfloat fov;
    
    static float pan;
    
    // camera position maybe not in main                                                          
    //static float cameraZ;
    //static float cameraX;
    //static float cameraY;
    
    // near and far plane                                                        
    static float nearPlane, farPlane;
    
    static glm::mat4 projectionMatrix;
    static glm::mat4 viewMatrix;
    static glm::mat4 modelMatrix;
    static glm::mat4 rotationMatrix;
    
    static LightSource lightSource;
    static Material material;
    
    struct Menu{
      enum Item{QUIT};
    };
    
    // ML schnipp
    static TriangleMesh islandMesh;
    static TriangleMesh cubeMesh;
    static TriangleMesh rectangleMesh;
    static TriangleMesh houseMesh;
    static TriangleMesh startMesh;
    static TriangleMesh streetMesh1;
    static TriangleMesh streetMesh2;
    static TriangleMesh streetMesh3;
    static TriangleMesh siedlungMesh;
    static TriangleMesh knightMesh;
    static TriangleMesh quadMesh;
    static glsl::Shader diffuseShader, texturingShader;
    static Island *centerIsland, *bottomLeftIsland, *bottomRightIsland, *leftIsland, *rightIsland, *topLeftIsland, *topRightIsland;
    static Island* skyBox;
    static std::vector <Beleg::Island*> decorations;
    //everything dice
    static Button* dice1;
    static Button* dice2;
    static Button* dice3;
    static Button* dice4;
    static Button* dice5;
    static Button* dice6;
    static Texture diceTexture1;
    static Texture diceTexture2;
    static Texture diceTexture3;
    static Texture diceTexture4;
    static Texture diceTexture5;
    static Texture diceTexture6;
    static Texture dicePressedTexture1;
    static Texture dicePressedTexture2;
    static Texture dicePressedTexture3;
    static Texture dicePressedTexture4;
    static Texture dicePressedTexture5;
    static Texture dicePressedTexture6;
    static Island* selectedDeco;
    static int decoCounter;

    
   
    
    // ML schnapp
  };
  
  class Left : public OpenGLApplication<configLeft>{
    
  public:
    
      friend class Beleg::Main;
      friend class Beleg::Left;

      static LightSource lightSourceLeft;
      static glm::mat4 projectionMatrixLeft;
      static glm::mat4 viewMatrixLeft;
      static glm::mat4 modelMatrixLeft;
      static TriangleMesh cubeMeshLeft;
      static Button* topLeftObject;
      static TriangleMesh *leftWindowCube;
      static glsl::Shader diffuseShaderLeft, texturingShaderLeft;
      static Texture texture;

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

      friend class Beleg::Main;
      friend class Beleg::Right;

      static LightSource lightSourceRight;
      static glm::mat4 projectionMatrixRight;
      static glm::mat4 viewMatrixRight;
      static glm::mat4 modelMatrixRight;
      static TriangleMesh buttonMeshRight;
      static std::vector <Beleg::Button*>  topRightObjects;
      static TriangleMesh *rightWindowCube;
      static glsl::Shader diffuseShaderRight, texturingShaderRight;
      static Texture pressedTexture;
      static Texture defaultTexture;
      static Mouse* mouse;


    // initialization
    static void init();
    
    // display scene
    static void display(void);
    
    // called after window rehape
    static void reshape();
    
    // keyboard callback
    static void keyPressed();

    static void mousePressed();
  };

  private:
  
   static void handleKeyboardInput(unsigned int key);  
};
