#pragma once

#include "sprite.h"
#include <string>

#include "globals.h";

class Texture {
public:

	Texture(char* = "Unnamed Texture");
	~Texture();

	/// <summary>
	/// Adds a sprite to the internal list of sprites. Returns the index in which it was added, or -1 if it failed.
	/// </summary>
	int add_sprite(Sprite*);
	bool remove_sprite(int);

	Sprite* get_sprite(int);

	/// <summary>
	/// Sprite pointer must be equal to that which was added by
	/// </summary>
	int get_index_by_ref(Sprite*);

	/// <param name="filepath"></param>
	void export_as(char*);



	std::string get_name();
	void set_name(char*);


private:

	char name[64];

	Sprite* sprites[CONSTS::SPRITES_PER_TEXTURE];


};

// Ill probably have to change the Sprite pointers to actual sprites, as I might store them here. we'll see.