#pragma once

#include "sprite.h"
#include "glad.h"

class RenderTexture {
public:

	RenderTexture();
	RenderTexture(Sprite*, char wrap_type = 's');
	~RenderTexture();


	bool load(Sprite*, char wrap_type = 's');

	void use();

private:
	unsigned int ID;

};