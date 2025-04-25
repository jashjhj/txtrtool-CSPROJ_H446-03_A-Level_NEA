#include "sprite.h"

#include <string>
#include <tchar.h>
#include <cstdio>
#include <filesystem>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


Sprite::Sprite(char* name, unsigned int width, unsigned int height) // Constructor
{
	std::strcpy(this->name, name);
	this->extension = static_cast<EXTENSION>(CONSTS::DEFAULT_EXPORT_EXTENSION);

	this->width = width;
	this->height = height;

	if (width == 0 || height == 0) {
		this->data = nullptr;
	}
	else {
		init_data();
	}

	sprite_type = Sprite::SPRITETYPE::UNSET;

	this->is_mono = false;
}

Sprite::~Sprite()
{
	//clear_data(); for some reason this crashes it
}





bool Sprite::load_from_file(char* filepath) {
	char* dot_index_ptr = std::strrchr(filepath, '.'); // validation that filepath is valid.
	if (dot_index_ptr == nullptr) { printf("Attempted to load file without extension @ `%s`.", filepath); return false; }

	int dot_index = *dot_index_ptr; // string rear character - get index of ending `.`
	char file_extension[8];
	std::strncpy(file_extension, filepath + dot_index, 8); // copies up to 8 chars from filename following the final `.`

	this->clear_data(); // resets pointer for new data

	int width = -1, height = -1, channels = -1;
	this->data = (char*)stbi_load(filepath, &width, &height, &channels, 4); // Loads file  --  4 to enforce 4 channels

	printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

	if (width < 0 || height < 0 || channels < 0) { printf("Failed to load."); return false; }

	this->set_width(width);
	this->set_height(height);

	return true;
}

void Sprite::generate(int new_width, int new_height, char fill_colour[4])
{
	if (new_width != -1) { this->width = new_width; }
	if (new_height != -1) { this->height = new_height; }

	if (width == 0 || height == 0) {
		this->data = nullptr;
	}
	else {
		init_data();
	}

	if (fill_colour == nullptr) {
		fill_colour = (char*)malloc(4);

		fill_colour[0] = 0x00; fill_colour[1] = 0xff; fill_colour[2] = 0xff; fill_colour[3] = 0xff;
	}
	
	for (int i = 0; i < this->get_area(); i++) { // fills in the memory.
		int* position = (int*)this->get_data() + i*4;
		position = (int*)fill_colour;
	}
}

void Sprite::export_as(char* filename) {

	char extension_string[8];
	strcpy(extension_string, get_extension_string(this->extension));


	char location[256]; // f out location
	location[0] = 0; // initialise null-terminated string

	if (filename[0] != 0) { // if file location is not current directory,
		// ensure that the location exists - if not, create it.
		std::filesystem::create_directories(filename);

		std::strcpy(location, filename);
		std::strcat(location, "/");
	}
	std::strcat(location, this->name);
	std::strcat(location, ".");
	std::strcat(location, extension_string);




	// use a library to save it.
	std::cout << this->extension && 0b11;

	switch (this->extension && 0b11) {
	case this->EXTENSION::BMP:
		stbi_write_bmp(location, this->width, this->height, 4, this->data);
		printf("Successfully exported file @ `%s`.", location);
		break;
	case EXTENSION::PNG:
		stbi_write_png(location, this->width, this->height, 4, this->data, this->width * 4);
		printf("Successfully exported file @ `%s`.", location);
		break;
	case EXTENSION::JPG:
		stbi_write_jpg(location, this->width, this->height, 4, this->data, 85);
		printf("Successfully exported file @ `%s`.", location);
		break;
	default:
		printf("Failed to export file - Unknown filetype");
	}

}


char* Sprite::prepare_for_export(char* export_location)
{
	char* last_dot = strrchr(export_location, '.');
	char* last_slash = (char*) __max((unsigned long long int)strrchr(export_location, '\\'), (unsigned long long int)strrchr(export_location, '/')); // retruns actual final \ or /
	if (last_dot == nullptr || last_slash == nullptr) { printf("invalid filepath in prepare_for_export: `%s` -Contains no . or /", export_location); return nullptr; }
	if ((unsigned long long int)last_slash > (unsigned long long int) last_dot) { printf("invalid filepath in prepare_for_export: `%s` -Contains / after .", export_location); return nullptr; }

	char extension[8];
	char name[32];
	strcpy(extension, last_dot+1);
	strncpy(name, last_slash + 1, last_dot - last_slash - 1);
	strnset(name + (last_dot - last_slash - 1), '\00', 1);

	this->extension = (get_extension_from_string(extension));
	std::cout << this->extension;
	this->set_name(name);

	memset(last_slash, 0x00, strlen(last_slash));

	return export_location;
}


void Sprite::clear_data() {
	if (this->data != nullptr) { // must prevent double-freeing
		free(this->data);
		this->data = nullptr;
	}
}

/// <summary>
/// Uses width, height to clear the correct area of data
/// </summary>
void Sprite::init_data(){
	if (this->data != nullptr) {
		clear_data();
	}
	this->data = (char*)malloc(width * height * 4); // *4 bpp
}

char* Sprite::get_extension_string(int e)
{
	char* extension_string = (char*)malloc(5);
	extension_string[0] = 0x00;
	switch (e)
	{
	case Sprite::EXTENSION::BMP:
		strcpy(extension_string, this->Sprite_Extension_Strings[0]);
		break;
	case Sprite::EXTENSION::PNG:
		strcpy(extension_string, this->Sprite_Extension_Strings[1]);
		break;
	case Sprite::EXTENSION::JPG:
		strcpy(extension_string, this->Sprite_Extension_Strings[2]);
		break;
	}
	
	return extension_string;
}

int Sprite::get_extension_from_string(char* str)
{
	for (int i = 0; i < 3; i++) {
		if (strcmp(str, this->Sprite_Extension_Strings[i]) == 0) {

			return i;

		}
	}
	return 0;
}











char* Sprite::get_name()
{
	return this->name;
}

void Sprite::set_name(char* new_name)
{
	strncpy(this->name, new_name, 63);
	strnset(this->name + strlen(new_name), '\00', 1);
}

int Sprite::get_extension()
{
	return (this->extension);
}

void Sprite::set_extension(int extension)
{
	this->extension = extension;
}

char* Sprite::get_data()
{
	return this->data;
}

char Sprite::get_average_pixel(unsigned int index)
{
	index = index * 4;
	if (is_mono) { return(this->data[index]); } // if its mono this is easier
	return (char)((
		  (int)this->data[index + 0]
		+ (int)this->data[index + 1]
		+ (int)this->data[index + 2]
		//+ (int)this->data[index + 3]
		)/3
		);
}

float Sprite::get_average_pixel_f(unsigned int index)
{
	index = index * 4;
	if (is_mono) { return((float)this->data[index]); } // if its mono this is easier
	return ((
		(float)this->data[index + 0]
		+ (float)this->data[index + 1]
		+ (float)this->data[index + 2]
		//+ (float)this->data[index + 3]
		) / 4.0f
		);
}

unsigned int Sprite::get_height()
{
	return this->height;
}

void Sprite::set_height(unsigned int height)
{
	this->height = height;
}

unsigned int Sprite::get_width()
{
	return this->width;
}

void Sprite::set_width(unsigned int width)
{
	this->width = width;
}


unsigned long int Sprite::get_area()
{
	return this->width * this->height;
}

Sprite::SPRITETYPE Sprite::get_sprite_type()
{
	return this->sprite_type;
}

void Sprite::set_sprite_type(SPRITETYPE type)
{
	this->sprite_type = type;

	switch (type){ // sets is_mono
	case Sprite::SPRITETYPE::HEIGHT:
		this->is_mono = true;
		break;

	case Sprite::SPRITETYPE::METALNESS:
		this->is_mono = true;
		break;

	case Sprite::SPRITETYPE::ROUGHNESS:
		this->is_mono = true;
		break;

	case Sprite::SPRITETYPE::OTHER_MONO:
		this->is_mono = true;
		break;

	default:
		this->is_mono = false;
	}
}

bool Sprite::get_is_mono()
{
	return this->is_mono;
}

