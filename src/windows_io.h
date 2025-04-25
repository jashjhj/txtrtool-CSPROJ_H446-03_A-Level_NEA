#pragma once
#include "sprite.h"
#include <string>


std::string open_image_file();

std::string pick_folder();

std::string save_image_file();

Sprite load_sprite_from_filesystem();

bool save_sprite_to_filesystem(Sprite*);