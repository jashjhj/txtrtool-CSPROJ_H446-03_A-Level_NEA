#include "normal_generation.h"
#include <iostream>;
#include <cmath>



Sprite Normalmap::generate(Sprite* reference, float steepness, unsigned char delta_threshold, bool is_half_resolution, EDGE_MODE edge_mode, bool is_dx_colors)
{
	if (delta_threshold == 0) { delta_threshold = 1; }


	Sprite output = Sprite("Normalmap gen output");


	if (reference->get_data() == nullptr) // validation - ensure sprite sent is safe and set up
	{
		printf("Invalid sprite passed to Normalmap::generate(): `%s` - Data uninitialised", reference->get_name());
		output.set_name("Uninitialised: Reference data uninitialised.");
		return output;
	};


	if (is_half_resolution) {
		if (reference->get_width() % 2 || reference->get_height() % 2) { // another guard clause - checks that resolution is divisble appropriately.
			printf("Invalid sprite passed to Normalmap::generate(): `%s` - is_half_resolution TRUE, resolution not divisible by 2", reference->get_name());
			output.set_name("Uninitialised: Reference data invalid: resolution not divisble by 2.");
			return output;
		}
		output.generate(reference->get_width() / 2, reference->get_height() / 2);
	}
	else { // 1x res
		output.generate(reference->get_width(), reference->get_height());
	}


	char* data_pointer = output.get_data();

	for (unsigned long int i = 0; i < output.get_area(); i++) {

		// 1. find relative gradients
		unsigned int pixel_pos_x = i % output.get_width();
		unsigned int pixel_pos_y = i / output.get_width();


		float vertical_grad, horizontal_grad;

		if(is_half_resolution){

			unsigned int local_index = pixel_pos_y * output.get_width() * 2 + pixel_pos_x * 2;


			vertical_grad = get_double_gradient(
				reference->get_average_pixel(local_index), reference->get_average_pixel(local_index + 1),
				reference->get_average_pixel(local_index + reference->get_width()), reference->get_average_pixel(local_index + reference->get_width() + 1)
			);
			horizontal_grad = get_double_gradient(
				reference->get_average_pixel(local_index), reference->get_average_pixel(local_index + reference->get_width()),
				reference->get_average_pixel(local_index + 1), reference->get_average_pixel(local_index + reference->get_width() + 1)
			);
		}
		// this one is tricky - lots of edge cases
		else 
		{
			unsigned char current = reference->get_average_pixel(i);
			unsigned char before_x = 0, after_x = 0, before_y = 0, after_y = 0;
			
			get_neighbours(reference, i, edge_mode, &before_x, &after_x, &before_y, &after_y);
			
			horizontal_grad = get_gradient(before_x, current, after_x);
			vertical_grad = get_gradient(before_y, current, after_y);
		}

		char normal_vector[3];
		memcpy(normal_vector, get_normal(get_angle(horizontal_grad, steepness, delta_threshold), get_angle(vertical_grad, steepness, delta_threshold)), 3);
		//normal_vector = { get_normal(get_angle(horizontal_grad, steepness, delta_threshold), get_angle(vertical_grad, steepness, delta_threshold)) };
		


		switch (is_dx_colors) { // sets pixel data.
		case true:
			// Z-is-up
			// difference is that green is negative.
			data_pointer[i * 4 + 0] = 255 - normal_vector[0];
			data_pointer[i * 4 + 1] = 255 - normal_vector[2];
			data_pointer[i * 4 + 2] = normal_vector[1];
			data_pointer[i * 4 + 3] = 255;

			break;
		case false:
			// Z-is-up
			data_pointer[i * 4 + 0] = 255 - normal_vector[0];
			data_pointer[i * 4 + 1] = normal_vector[2];
			data_pointer[i * 4 + 2] = normal_vector[1];
			data_pointer[i * 4 + 3] = 255;

			break;
		}

	}

	return output;
}

void Normalmap::get_neighbours(Sprite* reference, int i, Normalmap::EDGE_MODE edge_mode, unsigned char* before_x, unsigned char* after_x, unsigned char* before_y, unsigned char* after_y) {
	
	unsigned char current = reference->get_average_pixel(i);

	unsigned int pixel_pos_x = i % reference->get_width();
	unsigned int pixel_pos_y = i / reference->get_width();


	if (pixel_pos_x == 0) {
		*after_x = reference->get_average_pixel(i + 1);

		switch (edge_mode)
		{
		case Normalmap::EDGE_MODE::LOOP:
			*before_x = reference->get_average_pixel(pixel_pos_y + reference->get_width() - 1);
			break;
		case Normalmap::EDGE_MODE::EXTEND:
			*before_x = current;
			break;
		}
	}
	else if (pixel_pos_x == reference->get_width() - 1) {
		*before_x = reference->get_average_pixel(i - 1);

		switch (edge_mode)
		{
		case Normalmap::EDGE_MODE::LOOP:
			*after_x = reference->get_average_pixel(pixel_pos_y * reference->get_width());
			break;
		case Normalmap::EDGE_MODE::EXTEND:
			*after_x = current;
			break;
		}
	}
	else {
		*before_x = reference->get_average_pixel(i - 1);
		*after_x = reference->get_average_pixel(i + 1);
	}

	// and again for Y

	if (pixel_pos_y == 0) {
		*after_y = reference->get_average_pixel(i + reference->get_width());

		switch (edge_mode)
		{
		case Normalmap::EDGE_MODE::LOOP:
			*before_y = reference->get_average_pixel(pixel_pos_x + (reference->get_height() - 1) * reference->get_width());
			break;
		case Normalmap::EDGE_MODE::EXTEND:
			*before_y = current;
			break;
		}
	}
	else if (pixel_pos_y == reference->get_height() - 1) {
		*before_y = reference->get_average_pixel(i - reference->get_width());

		switch (edge_mode)
		{
		case Normalmap::EDGE_MODE::LOOP:
			*after_y = reference->get_average_pixel(pixel_pos_x);
			break;
		case Normalmap::EDGE_MODE::EXTEND:
			*after_y = current;
			break;
		}
	}
	else {
		*before_y = reference->get_average_pixel(i - reference->get_width());
		*after_y = reference->get_average_pixel(i + reference->get_width());
	}

}

float Normalmap::get_gradient(unsigned char before, unsigned char current, unsigned char after) {
	float left_delta = (float)before - (float)current;
	float right_delta = (float)after - (float)current;
	left_delta = fmin(left_delta, 0.0f);
	right_delta = fmin(right_delta, 0.0f);
	float left_right_delta = right_delta - left_delta;

	return left_right_delta / (127.5f * 2.0f);

	/*
	float left_average = ((float)current + (float)before) / 2.0f;
	float right_average = ((float)current + (float)after) / 2.0f;
	return (right_average - left_average) / 255.0f * 2.0f;
	*/
}


// gradient is in the range of -1 -> +1
float Normalmap::get_angle(float gradient, float steepness, unsigned char delta_threshold) {

	// maps gradient through delta threshold

	float considered_gradient = gradient / ((float)delta_threshold / 255.0f);

	considered_gradient = copysign(fmin(copysign(considered_gradient, 1.0f), 1.0f), gradient); // limits between -1 and +1 again

	considered_gradient *= steepness; // applies steepness


	return atan(considered_gradient); // calculates angle
}

char* Normalmap::get_normal(float angle_x, float angle_z)
{
	float normal_vector[3] = {0, 0, 0};
	normal_vector[0] = sin(angle_x) * cos(angle_z);
	normal_vector[1] = cos(angle_x) * cos(angle_z);
	normal_vector[2] = cos(angle_x) * sin(angle_z);

	//normal_vector[0] = 0.5f;
	//normal_vector[1] = -1.0f;
	//normal_vector[2] = 0.1f;

	char normal_map_encoded[3];

	for (int i = 0; i < 3; i++) { // translates float from -1->+1 to 0->255 char
		normal_map_encoded[i] = (char)((normal_vector[i] + 1.0f) * 255.0f / 2.0f);
		//std::cout << normal_vector[i] << " => " << normal_map_encoded[i] << "; ";
	}


	return normal_map_encoded;
}



// Old get_gradient function
/*
	// gets lowest pixels, finds gradient between them.
	float distance = 1.0f;

	float left = 255.0f;
	float right = 255.0f;

	if (before < after) {       left = before; // this is not my proudest code. It simply checks each case.
		if (current < after)    { right = current;} // i worked out each case manually. I cannot think of a cleaner way. - It's only 3 elements & 3 cases.
		else                    { right = after; distance = 2.0f;}
	}
	else {						right = after;
		if (current > after)    { left = before; distance = 2.0f;}
		else				    { left = current;}
	}

	return((right - left) / 255.0f / distance);
*/


float Normalmap::get_double_gradient(unsigned char a1, unsigned char a2, unsigned char b1, unsigned char b2)
{
	float first_average = ((float)a1 + (float)a2) / 2.0f / 255.0f;
	float second_average = ((float)b1 + (float)b2) / 2.0f / 255.0f;

	return second_average - first_average;
}
