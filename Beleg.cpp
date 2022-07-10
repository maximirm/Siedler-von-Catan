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
#include "Texture.hpp"

#include "LightSource.h"
#include "Material.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// use this with care
// might cause name collisions
using namespace glm;
using namespace std;



 OpenGLConfiguration config(glm::uvec2(2, 1),
			    OpenGLConfiguration::Profile::COMPATIBILITY,
			    (Window::DOUBLE | Window::DEPTH | Window::RGB | Window::MULTISAMPLE),
			    8,
			    glm::uvec2(100, 100),
			    glm::uvec2(562, 790),
			    "Computer Graphics - Beleg");

 OpenGLConfiguration configLeft(config,
				glm::uvec2(0, 0),
				glm::uvec2(334, 228),
				"");

OpenGLConfiguration configRight(config,
				glm::uvec2(334, 0),
				glm::uvec2(228, 228),
				"");
OpenGLConfiguration configMain(config,
			       glm::uvec2(0, 228),
			       glm::uvec2(562, 562),
			       "");


// field of view
GLfloat Beleg::Main::fov= 45.0;
GLfloat Beleg::cameraZ= 6;
GLfloat Beleg::cameraX = 0;
GLfloat Beleg::cameraY = 1;
static float scaling = 1;

mat4 Beleg::Main::projectionMatrix, Beleg::Main::viewMatrix, Beleg::Main::modelMatrix(1);
mat4 Beleg::Main::rotationMatrix = glm::mat4(1);

// ML schnipp
//meshes
TriangleMesh Beleg::Main::islandMesh;
TriangleMesh Beleg::Main::cubeMesh;
TriangleMesh Beleg::Main::houseMesh;
TriangleMesh Beleg::Main::startMesh;
TriangleMesh Beleg::Main::streetMesh1;
TriangleMesh Beleg::Main::streetMesh2;
TriangleMesh Beleg::Main::streetMesh3;
TriangleMesh Beleg::Main::siedlungMesh;
TriangleMesh Beleg::Main::knightMesh;
TriangleMesh Beleg::Main::quadMesh;

//Objects/Islands
Beleg::Island *Beleg::Main::centerIsland;
Beleg::Island* Beleg::Main::bottomLeftIsland;
Beleg::Island* Beleg::Main::bottomRightIsland;
Beleg::Island* Beleg::Main::leftIsland;
Beleg::Island* Beleg::Main::topLeftIsland;
Beleg::Island* Beleg::Main::topRightIsland;
Beleg::Island* Beleg::Main::rightIsland;
Beleg::Island* Beleg::Main::skyBox;
Beleg::Island* Beleg::Main::selectedDeco;
int Beleg::Main::decoCounter;
std::vector<Beleg::Island*> Beleg::Main::decorations;
//dice buttons
Beleg::Button* Beleg::Main::dice1;
Beleg::Button* Beleg::Main::dice2;
Beleg::Button* Beleg::Main::dice3;
Beleg::Button* Beleg::Main::dice4;
Beleg::Button* Beleg::Main::dice5;
Beleg::Button* Beleg::Main::dice6;

//dice texture
Texture Beleg::Main::diceTexture1;
Texture Beleg::Main::diceTexture2;
Texture Beleg::Main::diceTexture3;
Texture Beleg::Main::diceTexture4;
Texture Beleg::Main::diceTexture5;
Texture Beleg::Main::diceTexture6;
Texture Beleg::Main::dicePressedTexture1;
Texture Beleg::Main::dicePressedTexture2;
Texture Beleg::Main::dicePressedTexture3;
Texture Beleg::Main::dicePressedTexture4;
Texture Beleg::Main::dicePressedTexture5;
Texture Beleg::Main::dicePressedTexture6;

Mouse* Beleg::Main::mouse;
Mouse* Beleg::Right::mouse;


glsl::Shader Beleg::Main::diffuseShader, Beleg::Main::texturingShader;

// Left window
mat4 Beleg::Left::projectionMatrixLeft, Beleg::Left::viewMatrixLeft, Beleg::Left::modelMatrixLeft(1);
TriangleMesh Beleg::Left::cubeMeshLeft;
glsl::Shader Beleg::Left::diffuseShaderLeft, Beleg::Left::texturingShaderLeft;
Beleg::Button* Beleg::Left::topLeftObject;
Texture Beleg::Left::texture;
LightSource Beleg::Left::lightSourceLeft={
        // position
        glm::vec4(0, 0, 1, 0),
        // ambient color
        glm::vec4(1.0, 1.0, 1.0, 1.0),
        // diffuse color
        glm::vec4(1.0, 1.0, 1.0, 1.0),
        // specular color
        glm::vec4(1.0, 1.0, 1.0, 1.0),
};

// Right window
mat4 Beleg::Right::projectionMatrixRight, Beleg::Right::viewMatrixRight, Beleg::Right::modelMatrixRight(1);
TriangleMesh Beleg::Right::buttonMeshRight;
glsl::Shader Beleg::Right::diffuseShaderRight, Beleg::Right::texturingShaderRight;
std::vector<Beleg::Button*> Beleg::Right::topRightObjects;
Texture Beleg::Right::pressedTexture;
Texture Beleg::Right::defaultTexture;
LightSource Beleg::Right::lightSourceRight={
        // position
        glm::vec4(0, 0, 1, 0),
        // ambient color
        glm::vec4(1.0, 1.0, 1.0, 1.0),
        // diffuse color
        glm::vec4(1.0, 1.0, 1.0, 1.0),
        // specular color
        glm::vec4(1.0, 1.0, 1.0, 1.0),
};

// ML schnapp

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
  
  // ML schnipp
    //load ze meshes
  islandMesh.setWinding(TriangleMesh::CW);
  islandMesh.load("meshes/Plattform.obj");
  cubeMesh.load("meshes/cube.obj");
  houseMesh.load("meshes/Haus.obj");
  startMesh.load("meshes/Start.obj");
  streetMesh1.load("meshes/Strasse1.obj");
  streetMesh2.load("meshes/Strasse2.obj");
  streetMesh3.load("meshes/Strasse3.obj");
  siedlungMesh.load("meshes/Siedlung.obj");
  knightMesh.load("meshes/Ritter.obj");
  quadMesh.load("meshes/quad.obj");

  //load all dice texture
  diceTexture1.load("textures/wuerfel_1.ppm");
  diceTexture2.load("textures/wuerfel_2.ppm");
  diceTexture3.load("textures/wuerfel_3.ppm");
  diceTexture4.load("textures/wuerfel_4.ppm");
  diceTexture5.load("textures/wuerfel_5.ppm");
  diceTexture6.load("textures/wuerfel_6.ppm");
  dicePressedTexture1.load("textures/w1.ppm");
  dicePressedTexture2.load("textures/w2.ppm");
  dicePressedTexture3.load("textures/w3.ppm");
  dicePressedTexture4.load("textures/w4.ppm");
  dicePressedTexture5.load("textures/w5.ppm");
  dicePressedTexture6.load("textures/w6.ppm");


  const std::string version= "#version 120\n";


  diffuseShader.addVertexShader(version);
  diffuseShader.loadVertexShader("shaders/diffuse.vert");
  diffuseShader.compileVertexShader();
  diffuseShader.addFragmentShader(version);
  diffuseShader.loadFragmentShader("shaders/diffuse.frag");
  diffuseShader.compileFragmentShader();
  diffuseShader.bindVertexAttrib("position", TriangleMesh::attribPosition);
  diffuseShader.bindVertexAttrib("normal", TriangleMesh::attribNormal);
  diffuseShader.link();


  texturingShader.loadVertexShader("shaders/texturing.vert");
  texturingShader.compileVertexShader();
  texturingShader.loadFragmentShader("shaders/texturing.frag");
  texturingShader.compileFragmentShader();
  texturingShader.bindVertexAttrib("position", TriangleMesh::attribPosition);
  texturingShader.bindVertexAttrib("normal", TriangleMesh::attribNormal);
  texturingShader.bindVertexAttrib("texCoord", TriangleMesh::attribTexCoord);

  
  texturingShader.link();

  //create the objects
  centerIsland = new Island("./textures/mitte.ppm", glm::vec3(0, 0, 0), &islandMesh);
  bottomLeftIsland = new Island("./textures/wald.ppm", glm::vec3(-0.85, 0, 1.5), &islandMesh);
  bottomRightIsland = new Island("textures/lehm.ppm", glm::vec3(0.85, 0, 1.5), &islandMesh);
  topLeftIsland = new Island("./textures/getreide.ppm", glm::vec3(-0.85, 0, -1.5), &islandMesh);
  topRightIsland = new Island("./textures/erz.ppm", glm::vec3(0.85, 0, -1.5), &islandMesh);
  rightIsland = new Island("./textures/wuste.ppm", glm::vec3(1.7, 0, 0), &islandMesh);
  leftIsland = new Island("./textures/wiese.ppm", glm::vec3(-1.7, 0, 0), &islandMesh);  

  skyBox = new Island("./textures/box.ppm", glm::vec3(0), &cubeMesh, false);
  dice1 = new Button(glm::vec3(60, 40, 0), &quadMesh, &dicePressedTexture1, &diceTexture1, glm::vec2(50, 50));
  dice2 = new Button(glm::vec3(150, 40, 0), &quadMesh, &dicePressedTexture1, &diceTexture1, glm::vec2(50, 50));
  dice3 = new Button(glm::vec3(240, 40, 0), &quadMesh, &dicePressedTexture1, &diceTexture1, glm::vec2(50, 50));
  dice4 = new Button(glm::vec3(330, 40, 0), &quadMesh, &dicePressedTexture1, &diceTexture1, glm::vec2(50, 50));
  dice5 = new Button(glm::vec3(420, 40, 0), &quadMesh, &dicePressedTexture1, &diceTexture1, glm::vec2(50, 50));
  dice6 = new Button(glm::vec3(510, 40, 0), &quadMesh, &dicePressedTexture1, &diceTexture1, glm::vec2(50, 50));

  //decorate the island with vector of objects
  
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(0.5, 1.4, 0), &knightMesh));  
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-0.5, 1.4, 0), &knightMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-2.975, 1.4, -5.25), &knightMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(2.975, 1.4, -5.25), &knightMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-2.975, 1.4, 5.25), &knightMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(2.975, 1.4, 5.25), &knightMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-5.5, 1.4, 0), &knightMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(5, 1.4, 0), &knightMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(6, 1.4, 0), &knightMesh));

  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(4.25, 0.75, -2.35), &startMesh));  
  
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(0, 0.7, -4.75), &streetMesh1));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(0, 0.7, 5), &streetMesh1));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-5.5, 0.7, 5), &streetMesh1));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-5.5, 0.7, -4.75), &streetMesh1));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(5.5, 0.7, 5), &streetMesh1));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(8.25, 0.7, 0.25), &streetMesh1));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-8.25, 0.7, 0.25), &streetMesh1));

  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-1.5, 0.7, -2.25), &streetMesh2));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-7, 0.7, -2.25), &streetMesh2));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-1.5, 0.7, 7.25), &streetMesh2));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(4, 0.7, 7.25), &streetMesh2));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(7, 0.7, 2.25), &streetMesh2));

  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(1.5, 0.7, -2.25), &streetMesh3));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-4, 0.7, -2.25), &streetMesh3));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(1.5, 0.7, 7.25), &streetMesh3));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-4, 0.7, 7.25), &streetMesh3));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-7, 0.7, 2.25), &streetMesh3));

  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(0, 1, -6.25), &siedlungMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-5.5, 1, -6.25), &siedlungMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(0, 1, 3.5), &siedlungMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-5.5, 1, 3.5), &siedlungMesh));

  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(2.75, 0.85, -1.25), &houseMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-2.75, 0.85, -1.25), &houseMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(8, 0.85, -1.25), &houseMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-8, 0.85, -1.25), &houseMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(2.75, 0.85, 8), &houseMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(-2.75, 0.85, 8), &houseMesh));
  decorations.push_back(new Island("./textures/holz.ppm", glm::vec3(5.5, 0.85, 3.5), &houseMesh));

  decoCounter = 0;
  selectedDeco = decorations[decoCounter];
  //ML schnapp
}

// adjust to new window size
void Beleg::Main::reshape(){
    
  // viewport
  

  computeViewMatrix();
  computeProjectionMatrix();

  window->redisplay();
}

void Beleg::Main::computeViewMatrix(void){

    vec3 newPosition = vec3(rotationMatrix * vec4(cameraX, cameraY, cameraZ, 1));

  viewMatrix= glm::lookAt(newPosition * scaling, vec3(0), vec3(0,1,0));
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

  glViewport(0, 0, (GLsizei)window->width(), (GLsizei)window->height());
  glClearColor(0.0,0.0,0.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glm::mat4 matrix = glm::rotate(glm::mat4(1), glm::radians(00.0f), glm::vec3(1,0,0));
  centerIsland->display(matrix);
  bottomLeftIsland->display(matrix);
  bottomRightIsland->display(matrix);
  topLeftIsland->display(matrix);
  topRightIsland->display(matrix);
  leftIsland->display(matrix);
  rightIsland->display(matrix);
  skyBox->display(glm::scale(matrix, vec3(20)));

  float nearPlane = cameraZ / 10.0f;
  float farPlane = cameraZ * 10.0f;
  glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 0, 1) * scaling, vec3(0), vec3(0, 1, 0));
  glm::mat4 projectionMatrix = glm::ortho(0.0f, 562.0f, 562.0f, 0.0f, nearPlane, farPlane);

  dice1->display(projectionMatrix * viewMatrix);
  dice2->display(projectionMatrix * viewMatrix);
  dice3->display(projectionMatrix * viewMatrix);
  dice4->display(projectionMatrix * viewMatrix);
  dice5->display(projectionMatrix * viewMatrix);
  dice6->display(projectionMatrix * viewMatrix);
  for (auto i : decorations) {
      i->display(glm::scale(modelMatrix, vec3(0.3)));
  }
  


  // ML schnipp
  diffuseShader.bind();
  diffuseShader.setUniform("transformation", projectionMatrix*viewMatrix*modelMatrix);
  diffuseShader.setUniform("color", vec3(1,1,1));
  diffuseShader.setUniform("lightPosition", inverse(modelMatrix)*lightSource.position);
  //mesh.draw();
  diffuseShader.unbind();
  // ML schnapp
  
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

    

    const std::string version= "#version 120\n";
    cubeMeshLeft.load("meshes/quad.obj", false);
    texture.load("textures/topleftpic.ppm");

    diffuseShaderLeft.addVertexShader(version);
    diffuseShaderLeft.loadVertexShader("shaders/diffuse.vert");
    diffuseShaderLeft.compileVertexShader();
    diffuseShaderLeft.addFragmentShader(version);
    diffuseShaderLeft.loadFragmentShader("shaders/diffuse.frag");
    diffuseShaderLeft.compileFragmentShader();
    diffuseShaderLeft.bindVertexAttrib("position", TriangleMesh::attribPosition);
    diffuseShaderLeft.bindVertexAttrib("normal", TriangleMesh::attribNormal);
    diffuseShaderLeft.link();

    texturingShaderLeft.loadVertexShader("shaders/texturing.vert");
    texturingShaderLeft.compileVertexShader();
    texturingShaderLeft.loadFragmentShader("shaders/texturing.frag");
    texturingShaderLeft.compileFragmentShader();
    texturingShaderLeft.bindVertexAttrib("position", TriangleMesh::attribPosition);
    texturingShaderLeft.bindVertexAttrib("normal", TriangleMesh::attribNormal);
    texturingShaderLeft.bindVertexAttrib("texCoord", TriangleMesh::attribTexCoord);
    texturingShaderLeft.link();

    topLeftObject = new Button(glm::vec3(0), &cubeMeshLeft, &texture, &texture, glm::vec2(1,1));

}

void Beleg::Left::reshape(void){
  
  // Set the viewport to be the entire window
  

  window->redisplay();
}

// display texture on full screen quad
void Beleg::Left::display(void){

  glViewport(0, 0, window->width(), window->height());
  glClearColor(0.3, 0.3, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    
    topLeftObject->display(glm::mat4(1));

    window->swapBuffers();



}

// Right Window

void Beleg::Right::init(void){

    buttonMeshRight.load("meshes/quad.obj");
    pressedTexture.load("textures/checked.ppm");
    defaultTexture.load("textures/unchecked.ppm");


    texturingShaderRight.loadVertexShader("shaders/texturing.vert");
    texturingShaderRight.compileVertexShader();
    texturingShaderRight.loadFragmentShader("shaders/texturing.frag");
    texturingShaderRight.compileFragmentShader();
    texturingShaderRight.bindVertexAttrib("position", TriangleMesh::attribPosition);
    texturingShaderRight.bindVertexAttrib("normal", TriangleMesh::attribNormal);
    texturingShaderRight.bindVertexAttrib("texCoord", TriangleMesh::attribTexCoord);


    texturingShaderRight.link();

    //create the objects
    //top row
    topRightObjects.push_back(new Button(glm::vec3(30, 150,0), &buttonMeshRight, &pressedTexture, &defaultTexture, glm::vec2(20, 20)));
    topRightObjects.push_back(new Button(glm::vec3(70, 150, 0), &buttonMeshRight, &pressedTexture, &defaultTexture, glm::vec2(20, 20)));
    topRightObjects.push_back(new Button(glm::vec3(110, 150, 0), &buttonMeshRight, &pressedTexture, &defaultTexture, glm::vec2(20, 20)));
    topRightObjects.push_back(new Button(glm::vec3(150, 150, 0), &buttonMeshRight, &pressedTexture, &defaultTexture, glm::vec2(20, 20)));
    topRightObjects.push_back(new Button(glm::vec3(190, 150, 0), &buttonMeshRight, &pressedTexture, &defaultTexture, glm::vec2(20, 20)));
    //bottom row
    topRightObjects.push_back(new Button(glm::vec3(30, 200, 0), &buttonMeshRight, &pressedTexture, &defaultTexture, glm::vec2(20, 20)));
    topRightObjects.push_back(new Button(glm::vec3(70, 200, 0), &buttonMeshRight, &pressedTexture, &defaultTexture, glm::vec2(20, 20)));
    topRightObjects.push_back(new Button(glm::vec3(110, 200, 0), &buttonMeshRight, &pressedTexture, &defaultTexture, glm::vec2(20, 20)));
    topRightObjects.push_back(new Button(glm::vec3(150, 200, 0), &buttonMeshRight, &pressedTexture, &defaultTexture, glm::vec2(20, 20)));
    topRightObjects.push_back(new Button(glm::vec3(190, 200, 0), &buttonMeshRight, &pressedTexture, &defaultTexture, glm::vec2(20, 20)));
    //top right
    topRightObjects.push_back(new Button(glm::vec3(190, 100, 0), &buttonMeshRight, &pressedTexture, &defaultTexture, glm::vec2(20, 20)));
    topRightObjects.push_back(new Button(glm::vec3(190, 50, 0), &buttonMeshRight, &pressedTexture, &defaultTexture, glm::vec2(20, 20)));

}

void Beleg::Right::reshape(void){
  
  // Set the viewport to be the entire window
  

  window->redisplay();
}

// display texture on full screen quad
void Beleg::Right::display(void){

  glViewport(0, 0, window->width(), window->height());
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float nearPlane=cameraZ/10.0f;
    float farPlane= cameraZ*10.0f;
    glm::mat4 viewMatrix= glm::lookAt(glm::vec3(0,0,1) * scaling, vec3(0), vec3(0,1,0));
    glm::mat4 projectionMatrix = glm::ortho(0.0f, 228.0f, 228.0f, 0.0f, nearPlane, farPlane );
    for (auto i : topRightObjects) {
        i->display(projectionMatrix * viewMatrix);
    }

    


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
  // x,y and z axes with keypressed
   
  switch(key){
    
  case 'q':
  case 'Q':
    exit(0);
  case 'w':
      Main::rotationMatrix = glm::rotate(Main::rotationMatrix, glm::radians(3.0f), vec3(1, 0, 0));
      Main::computeViewMatrix();
      Main::window->redisplay();
      break;
  case 's':
      Main::rotationMatrix = glm::rotate(Main::rotationMatrix, glm::radians(-3.0f), vec3(1, 0, 0));
      Main::computeViewMatrix();
      Main::window->redisplay();
      break;
  case 'a':
      Main::rotationMatrix = glm::rotate(glm::mat4(1), glm::radians(3.0f), vec3(0, 1, 0)) * Beleg::Main::rotationMatrix;
      Main::computeViewMatrix();
      Main::window->redisplay();
      break;
  case 'd':
      Main::rotationMatrix = glm::rotate(glm::mat4(1), glm::radians(-3.0f), vec3(0, 1, 0)) * Beleg::Main::rotationMatrix;
      Main::computeViewMatrix();
      Main::window->redisplay();
      break;
  case 'i':
      if (scaling > 0.3) {
          scaling = scaling * 0.9;
      }
      Main::computeViewMatrix();
      Main::window->redisplay();
      break;
  case 'k':
      if (scaling < 1.9) {
          scaling = scaling * 1.1;
      }
      Main::computeViewMatrix();
      Main::window->redisplay();
      break;
  case 'r':
      if (!Beleg::Main::dice1->pressed) {
          Beleg::Main::rollDice(Beleg::Main::dice1);
      }
      if (!Beleg::Main::dice2->pressed) {
          Beleg::Main::rollDice(Beleg::Main::dice2);
      }
      if (!Beleg::Main::dice3->pressed) {
          Beleg::Main::rollDice(Beleg::Main::dice3);
      }
      if (!Beleg::Main::dice4->pressed) {
          Beleg::Main::rollDice(Beleg::Main::dice4);
      }
      if (!Beleg::Main::dice5->pressed) {
          Beleg::Main::rollDice(Beleg::Main::dice5);
      }
      if (!Beleg::Main::dice6->pressed) {
          Beleg::Main::rollDice(Beleg::Main::dice6);
      }
      Main::window->redisplay();
      break;
  case 'u':
      if(Beleg::Main::decoCounter < Beleg::Main::decorations.size())
      Main::decoCounter += 1;
      Main::setSelectedDecoration();
      break;
  case 'j':
      if (Main::decoCounter > 0) {
          Main::decoCounter -= 1;
          Main::setSelectedDecoration();
      }
      break;
  case 'g':
      Main::markSelectedDecoration();
  default:
    break;

  }
}

//prototype for dice rolling
void Beleg::Main::rollDice(Button *dice) {
   

    //generate random number between 6 and 1
    int diceSide;
    diceSide = rand() % 6 + 1;

    switch (diceSide) {
    case 1:
        dice->setTexture(&dicePressedTexture1 ,&diceTexture1);
        break;
    case 2:
        dice->setTexture(&dicePressedTexture2, &diceTexture2);
        break;
    case 3:
        dice->setTexture(&dicePressedTexture3, &diceTexture3);
        break;
    case 4:
        dice->setTexture(&dicePressedTexture4,&diceTexture4);
        break;
    case 5:
        dice->setTexture(&dicePressedTexture5,&diceTexture5);
        break;
    case 6:
        dice->setTexture(&dicePressedTexture6, &diceTexture6);
        break;
    default:
        break;
    }

}

void Beleg::Main::setSelectedDecoration() {
    Beleg::Main::selectedDeco = Beleg::Main::decorations[decoCounter];
}

void Beleg::Main::markSelectedDecoration() {
    Beleg::Main::selectedDeco->setTexture(&diceTexture1);
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

vector< pair < int, string > > Beleg::Main::menuEntries{ {Menu::QUIT, "quit"} };

// mouse menu control
void Beleg::Main::menu(int id){
  
  switch (id) {
  case Menu::QUIT: 
    exit(0);

  default:
    break;
  }
}

void Beleg::Main::mousePressed() {
    vec2 mousePosition = mouse->position;

    if (dice1->checkPosition(mousePosition)) {
        dice1->toggle();
        Main::window->redisplay();
    }
    if (dice2->checkPosition(mousePosition)) {
        dice2->toggle();
        Main::window->redisplay();
    }
    if (dice3->checkPosition(mousePosition)) {
        dice3->toggle();
        Main::window->redisplay();
    }
    if (dice4->checkPosition(mousePosition)) {
        dice4->toggle();
        Main::window->redisplay();
    }
    if (dice5->checkPosition(mousePosition)) {
        dice5->toggle();
        Main::window->redisplay();
    }
    if (dice6->checkPosition(mousePosition)) {
        dice6->toggle();
        Main::window->redisplay();
    }
}

void Beleg::Right::mousePressed() {
    for (auto checkBox : topRightObjects) {
        if (checkBox->checkPosition(mouse->position)) {
            checkBox->toggle();
            Right::window->redisplay();
        }
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
