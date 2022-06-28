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
			    glm::uvec2(50, 50),
			    glm::uvec2(281, 395),
			    "Computer Graphics - Beleg");

 OpenGLConfiguration configLeft(config,
				glm::uvec2(0, 0),
				glm::uvec2(167, 114),
				"");

OpenGLConfiguration configRight(config,
				glm::uvec2(167, 0),
				glm::uvec2(114, 114),
				"");
OpenGLConfiguration configMain(config,
			       glm::uvec2(0, 114),
			       glm::uvec2(281, 281),
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
TriangleMesh Beleg::Main::islandMesh;
TriangleMesh Beleg::Main::cubeMesh;
TriangleMesh Beleg::Main::houseMesh;
Beleg::Island *Beleg::Main::centerIsland;
Beleg::Island* Beleg::Main::bottomLeftIsland;
Beleg::Island* Beleg::Main::bottomRightIsland;
Beleg::Island* Beleg::Main::leftIsland;
Beleg::Island* Beleg::Main::topLeftIsland;
Beleg::Island* Beleg::Main::topRightIsland;
Beleg::Island* Beleg::Main::rightIsland;
Beleg::Island* Beleg::Main::skyBox;
std::vector<Beleg::Island*> Beleg::Main::houses;

glsl::Shader Beleg::Main::diffuseShader, Beleg::Main::texturingShader;

// Left window
mat4 Beleg::Left::projectionMatrixLeft, Beleg::Left::viewMatrixLeft, Beleg::Left::modelMatrixLeft(1);
TriangleMesh Beleg::Left::cubeMeshLeft;
glsl::Shader Beleg::Left::diffuseShaderLeft, Beleg::Left::texturingShaderLeft;
Beleg::Island* Beleg::Left::topLeftObject;
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
  islandMesh.setWinding(TriangleMesh::CW);
  islandMesh.load("meshes/platform.obj");
  cubeMesh.load("meshes/cube.obj");
  houseMesh.load("meshes/Hauschen-Body.obj");


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

  centerIsland = new Island("./textures/grass.ppm", glm::vec3(0, 0, 0), &islandMesh);
  bottomLeftIsland = new Island("./textures/checker.ppm", glm::vec3(-1.2, 0, -0.7), &islandMesh);
  bottomRightIsland = new Island("textures/earthcyl2.ppm", glm::vec3(-1.2, 0, 0.7), &islandMesh);
  topLeftIsland = new Island("./textures/earthcyl2.ppm", glm::vec3(1.2, 0, -0.7), &islandMesh);
  topRightIsland = new Island("./textures/sand.ppm", glm::vec3(1.2, 0, 0.7), &islandMesh);
  rightIsland = new Island("./textures/cobblestone.ppm", glm::vec3(0, 0, 1.4), &islandMesh);
  leftIsland = new Island("./textures/sand.ppm", glm::vec3(0, 0, -1.4), &islandMesh);
  
  skyBox = new Island("./textures/sky.ppm", glm::vec3(0), &cubeMesh, false);

  houses.push_back(new Island("./textures/checker.ppm", glm::vec3(1, 0.8, 1), &houseMesh));
  //ML schnapp
}

// adjust to new window size
void Beleg::Main::reshape(){
    
  // viewport
  glViewport(0, 0, (GLsizei) window->width(), (GLsizei) window->height());

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
  skyBox->display(glm::scale(matrix, vec3(10)));
  for (auto i : houses) {
      i->display(glm::scale(modelMatrix, vec3(0.5)));
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
    cubeMeshLeft.load("meshes/quad.off",false);
    viewMatrixLeft = glm::lookAt(vec3(0,0,-1), vec3(0), vec3(0,1,0));

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

    topLeftObject = new Island("./textures/topleftpic.ppm", vec3(1,-2,0) , &cubeMeshLeft, true, true);


}

void Beleg::Left::reshape(void){
  
  // Set the viewport to be the entire window
  glViewport(0, 0, window->width(), window->height());

  window->redisplay();
}

// display texture on full screen quad
void Beleg::Left::display(void){

    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 matrix = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(1,0,0));

    topLeftObject->display( viewMatrixLeft);


    window->swapBuffers();




}

// Right Window

void Beleg::Right::init(void){

}

void Beleg::Right::reshape(void){
  
  // Set the viewport to be the entire window
  glViewport(0, 0, window->width(), window->height());

  window->redisplay();
}

// display texture on full screen quad
void Beleg::Right::display(void){

    glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
      Main::rotationMatrix = glm::rotate(Main::rotationMatrix, glm::radians(3.0f), vec3(0, 1, 0));
      Main::computeViewMatrix();
      Main::window->redisplay();
      break;
  case 'd':
      Main::rotationMatrix = glm::rotate(Main::rotationMatrix, glm::radians(-3.0f), vec3(0, 1, 0));
      Main::computeViewMatrix();
      Main::window->redisplay();
      break;
  case 'i':
      scaling = scaling * 0.9;
      Main::computeViewMatrix();
      Main::window->redisplay();
  case 'k':
      scaling = scaling * 1.1;
      Main::computeViewMatrix();
      Main::window->redisplay();
  default:
    break;

  }
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

vector< pair < int, string > > Beleg::Main::menuEntries{{Menu::QUIT, "quit"}};

// mouse menu control
void Beleg::Main::menu(int id){
  
  switch (id) {
  case Menu::QUIT: 
    exit(0);

  default:
    break;
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
