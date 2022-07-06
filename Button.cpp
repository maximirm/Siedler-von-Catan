#include "Beleg.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;
using namespace std;

Beleg::Button::Button(glm::vec3 position, TriangleMesh *meshPointer, Texture* pressedTexture, Texture* defaultTexture, glm::vec2 size) {
	this->position = position;
	this->mesh = meshPointer;
	this->pressed = false;
    this->pressedTexture = pressedTexture;
    this->defaultTexture = defaultTexture;
    this->size = size;
}

void Beleg::Button::display(glm::mat4 modelViewProjectionMatrix) {

    modelViewProjectionMatrix = glm::translate(modelViewProjectionMatrix, this->position);
    modelViewProjectionMatrix = glm::scale(modelViewProjectionMatrix, glm::vec3(size.x, size.y, 1));
    Main::texturingShader.bind();
    Main::texturingShader.setUniform("modelViewProjectionMatrix", modelViewProjectionMatrix);
    Main::texturingShader.setUniform("lighting", false);
    Main::texturingShader.setUniform("texturing", true);
    Main::texturingShader.setUniform("texture", texture.id());
    Main::texturingShader.setUniform("lightPosition", inverse(modelViewProjectionMatrix) * Main::lightSource.position);
    if (pressed) {
        this->pressedTexture->bind();
    }
    else {
        this->defaultTexture->bind();
    }
    this->mesh->draw();
    this->pressedTexture->unbind();
    this->defaultTexture->unbind();
    Main::texturingShader.unbind();
}

void Beleg::Button::toggle() {
    if (this->pressed == true) {
        this->pressed = false;
    }
    else {
        this->pressed = true;
  
    }
}

bool Beleg::Button::checkPosition(glm::vec2 position2D) {
    if (position2D.x > position.x - (size.x / 2) && 
        position2D.x < position.x + (size.x / 2) && 
        position2D.y > position.y - (size.y / 2) && 
        position2D.y < position.y + (size.y / 2)) {
        return true;
    }
    else
    {
        return false;
    }
    
}

void Beleg::Button::setTexture(Texture* texture) {
    this->defaultTexture = texture;
    this->pressedTexture = texture;
}
