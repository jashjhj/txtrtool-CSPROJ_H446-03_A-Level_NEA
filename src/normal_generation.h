#pragma once

#include "sprite.h"

namespace Normalmap {

	enum EDGE_MODE
	{
		LOOP = 0, EXTEND = 1
	};

	Sprite generate(Sprite* reference, float steepness, unsigned char delta_threshold, bool is_half_resolution = false, EDGE_MODE = EDGE_MODE::EXTEND, bool is_dx_colors = false);

	float get_gradient(unsigned char, unsigned char, unsigned char);

	float get_double_gradient(unsigned char, unsigned char, unsigned char, unsigned char);

	void get_neighbours(Sprite* reference, int i, Normalmap::EDGE_MODE edge_mode, unsigned char* before_x, unsigned char* after_x, unsigned char* before_y, unsigned char* after_y);

	float get_angle(float gradient, float steepness, unsigned char delta_threshold);

	char* get_normal(float angle_x, float angle_z);

}