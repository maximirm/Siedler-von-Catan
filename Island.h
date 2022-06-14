#pragma once
#include "glm/glm.hpp"
#include "Texture.hpp"

class Island

{
	
private:
	glm::vec3 position;

	Texture texture;

public:
	void draw(glm::mat4 modelMatrix);

	Island(std::string texture, glm::vec3 position);

};


