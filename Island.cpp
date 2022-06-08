#include "Beleg.hpp"

using namespace glm;
using namespace std;

// TODO load texture
Beleg::Island::Island(string texture){

}

//TODO: bind texture and use texturing shader instead of diffuse shader
void Beleg::Island::display(glm::mat4 modelMatrix) {
	
  Main::diffuseShader.bind();
  Main::diffuseShader.setUniform("transformation", Main::projectionMatrix * Main::viewMatrix * modelMatrix);
  Main::diffuseShader.setUniform("color", vec3(1, 1, 1));
  Main::diffuseShader.setUniform("lightPosition", inverse(modelMatrix) * Main::lightSource.position);
  Main::mesh.draw();
  Main::diffuseShader.unbind();
}
