#include "Beleg.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;
using namespace std;

// TODO load texture

Beleg::Island::Island(std::string texture, glm::vec3 position) {
    this->texture.load(texture);
    this->position = position;
}


//TODO: bind texture and use texturing shader instead of diffuse shader
void Beleg::Island::display(glm::mat4 modelMatrix) {
	 
    modelMatrix = glm::translate(modelMatrix, this->position);
    Main::texturingShader.bind();
    Main::texturingShader.setUniform("modelViewProjectionMatrix", Main::projectionMatrix * Main::viewMatrix * modelMatrix);
    Main::texturingShader.setUniform("lighting", true);
    Main::texturingShader.setUniform("texturing", true);
    Main::texturingShader.setUniform("texture", texture.id());
    Main::texturingShader.setUniform("lightPosition", inverse(modelMatrix) * Main::lightSource.position);
    this->texture.bind();
    Main::mesh.draw();
    this->texture.unbind();
    Main::texturingShader.unbind();
}
