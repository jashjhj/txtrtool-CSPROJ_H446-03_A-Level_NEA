#include "texture.h"


Texture::Texture(char* name)
{
	std::strcpy(this->name, name);

	//this->sprites[8] = nullptr; // this line causes data corruption as it cannot be cleanly freed -- NO WONDER 8th index is not a thing ...

	for (int i = 0; i < CONSTS::SPRITES_PER_TEXTURE; i++) {
		this->sprites[i] = nullptr;
	}
}

Texture::~Texture()
{
}

int Texture::add_sprite(Sprite* new_sprite)
{
	for (int i = 0; i < CONSTS::SPRITES_PER_TEXTURE; i++) {
		if (this->sprites[i] == 0x0){ // then it's empty, and can be filled
			sprites[i] = new_sprite;
			return i;
		}
	}
	return -1; // failed to find empty slot;
}

bool Texture::remove_sprite(int index)
{

	if (index >= 0 && index < CONSTS::SPRITES_PER_TEXTURE) {
		if (sprites[index] == nullptr) {
			return false;
		}
		this->sprites[index] = nullptr;
		return true;
	}
	return false;
	
}

Sprite* Texture::get_sprite(int index)
{
	if (index >= 0 && index < CONSTS::SPRITES_PER_TEXTURE) {
		return this->sprites[index];
	}
	else { return nullptr; }
}

int Texture::get_index_by_ref(Sprite* reference)
{
	for (int i = 0; i < CONSTS::SPRITES_PER_TEXTURE; i++) {
		if (this->sprites[i] == reference) { // then it's empty, and can be filled
			return i;
		}
	}
	return -1; // failed to find empty slot;
}

void Texture::export_as(char* filename)
{
	for (int i = 0; i < CONSTS::SPRITES_PER_TEXTURE; i++) {
		if (this->sprites[i] != nullptr) { // then it's a thing - so save it
			sprites[i]->export_as(filename);
		}
	}
}


std::string Texture::get_name()
{
	return this->name;
}

void Texture::set_name(char*)
{
	std::strcpy(this->name, name);
}

