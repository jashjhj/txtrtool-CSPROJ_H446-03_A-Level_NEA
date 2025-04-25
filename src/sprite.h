#pragma once

#include <string>
#include "globals.h"

#define SPRITE_EXTENSIONS



class Sprite
{
public:


	static enum EXTENSION
	{
		BMP = 0, PNG = 1, JPG = 2, SIZE_OF_ENUM // SIZE_OF_ENUM must be last
	};
	inline const static char* Sprite_Extension_Strings[3] = { "bmp", "png", "jpg" };
	static_assert(sizeof(Sprite_Extension_Strings) / sizeof(char*) == (int)EXTENSION::SIZE_OF_ENUM, "Extension enum sizes incompatible."); // these MUST be same length


	enum class SPRITETYPE
	{
		UNSET, DIFFUSION, NORMAL, HEIGHT, ROUGHNESS, METALNESS, OTHER_MONO, OTHER_MULTI
	};



	Sprite(char* = "Unnamed Sprite", unsigned int = 0, unsigned int = 0);
	~Sprite();

	bool load_from_file(char*);

	// Yellow by default
	void generate(int width = -1, int height = -1, char fill_colour[4] = nullptr);

	char* get_name();
	void set_name(char*);
	int get_extension();
	void set_extension(int);

	char* get_data();
	char get_average_pixel(unsigned int index);
	float get_average_pixel_f(unsigned int index);

	unsigned int get_height();
	void set_height(unsigned int);
	unsigned int get_width();
	void set_width(unsigned int);

	unsigned long int get_area();

	SPRITETYPE get_sprite_type();
	void set_sprite_type(SPRITETYPE);

	bool get_is_mono();


	/// <param name="filepath"></param>
	void export_as(char*);

	/// <summary>
	/// WARNING - modifies input char*
	/// </summary>
	char* prepare_for_export(char*);


	char* get_extension_string(int );
	int get_extension_from_string(char*); // potentilly unnecessary?

private:

	void clear_data();
	void init_data();

	char name[64];
	int extension = 0;




	char* data; // 32bpp enforced

	unsigned int height;
	unsigned int width;

	SPRITETYPE sprite_type;

	bool is_mono;


};

