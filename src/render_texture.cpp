#include "render_texture.h"
#include <iostream>
#include "renderer.h"

RenderTexture::RenderTexture()
{
	this->ID = 0;
}

RenderTexture::RenderTexture(Sprite* sprite, char wrap_type)
{
	this->load(sprite, wrap_type);
}

RenderTexture::~RenderTexture()
{
	glGenTextures(1, &this->ID);
	this->use();
}


/// <param name="wrap_type">[c]lamp, [l]oop</param>
bool RenderTexture::load(Sprite* sprite, char wrap_type)
{

	glGenTextures(1, &this->ID);
	this->use();

	switch (wrap_type) {
	case 's'://stretch
	case 'c'://clamp
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // wrapping settings: clamp to edge
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;

	case 'l'://loop
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;

	default:
		std::printf("Incorrect wrapping type: `%c`", wrap_type);
	}
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // on scale down (texture area > pixel area)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // on scale up (texture area < pixel area)

	if (sprite->get_data() == nullptr) // ensures that there * is * data to create a texture from
	{
		std::cout << "ERROR: Render texture created based on sprite with missing data attribute" << std::endl;
		return 0;
	}

	// RGBA for potential transparency support.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite->get_width(), sprite->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite->get_data());

	return 1;
}

void RenderTexture::use()
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}
