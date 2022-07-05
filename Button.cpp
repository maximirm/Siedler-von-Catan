#include "Beleg.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;
using namespace std;

Beleg::Button::Button(glm::vec3 position, TriangleMesh *meshPointer, std::string pressedTexture, std::string defaultTexture) {
	this->position = position;
	this->mesh = meshPointer;
	this->pressed = false;
    this->pressedTexture = pressedTexture;
    this->defaultTexture = defaultTexture;
    this->texture.load(this->defaultTexture);
}

void Beleg::Button::display(glm::mat4 modelMatrix) {

    //modelMatrix = glm::translate(modelMatrix, this->position);
    Main::texturingShader.bind();
    Main::texturingShader.setUniform("modelViewProjectionMatrix", Main::projectionMatrix * Main::viewMatrix * modelMatrix);
    Main::texturingShader.setUniform("lighting", false);
    Main::texturingShader.setUniform("texturing", true);
    Main::texturingShader.setUniform("texture", texture.id());
    Main::texturingShader.setUniform("lightPosition", inverse(modelMatrix) * Main::lightSource.position);
    this->texture.bind();
    this->mesh->draw();
    this->texture.unbind();
    Main::texturingShader.unbind();
}

void Beleg::Button::toggle() {
    if (this->pressed == true) {
        this->pressed = false;
        this->texture.load(this->defaultTexture);
    }
    else
    {
        this->pressed = true;
        this->texture.load(this->pressedTexture);
    }
}

bool Beleg::Button::checkPosition(glm::vec2 position2D) {
    return false;
}
