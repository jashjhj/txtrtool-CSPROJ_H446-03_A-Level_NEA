#include "viewport.h"

#include "renderer.h" // the include must be here to let renderer.h load viewport.h first
#include <iostream>

unsigned int Viewport::ID_counter = 1;


Viewport::Viewport(float pos_x, float pos_y, float size_x, float size_y)
{
	set_position(pos_x, pos_y);
	set_window_size(size_x, size_y);
	

	ID = ID_counter;
	ID_counter++; // increments for all
}

Viewport::~Viewport()
{
}

void Viewport::render() // plus controls
{
	//draw background.


	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;

	ImGui::SetNextWindowPos(ImVec2(Rendering::portion_to_pixels(this->position_x, 'w'), Rendering::portion_to_pixels(this->position_y, 'h')));
	ImGui::SetNextWindowSize(ImVec2(Rendering::portion_to_pixels(this->size_x, 'w'), Rendering::portion_to_pixels(this->size_y, 'h')));

	char title[20] = "Viewport##";
	strcat(title, std::to_string(ID).c_str());

	ImGui::Begin(title, nullptr, window_flags);


	bool is_hovered = ImGui::IsWindowHovered();
	//ImGui::Checkbox("IsItemHovered ", &is_hovered); // cosmetic -- debug
	if (is_hovered && ImGui::IsMouseClicked(0)) { this->is_mouse_dragging = true; dragging_delta = ImVec2(0, 0); }; // if you start dragging

	if (is_mouse_dragging) {
		if (!ImGui::IsMouseDown(0)) {
			is_mouse_dragging = false;
		}
		else {// if mosue currently being dragged
			ImVec2 drag = ImGui::GetMouseDragDelta(0, -1.0f);
			this->image_position_x += Rendering::pixels_to_portion(drag.x - dragging_delta.x, 'w');
			this->image_position_y += -Rendering::pixels_to_portion(drag.y - dragging_delta.y, 'h'); // flipped for opengl

			dragging_delta = drag;
		}
	}

	if (is_hovered) { // mosue wheel - zoom in and out controls
		int mouse_wheel_input = ImGui::GetIO().MouseWheel;

		if (mouse_wheel_input != 0) { // if scrolled

			float zoom_factor = pow(CONSTS::ZOOM_SENSITIVITY, (float)mouse_wheel_input);

			this->image_scale *= zoom_factor; // performs nice, friendly exponential zoom proportional to scroll rate.


			// maths to displace the image intuitively
			ImVec2 mouse_pos = ImGui::GetMousePos();
			float mouse_pos_x = Rendering::pixels_to_portion(mouse_pos.x, 'w');
			float mouse_pos_y = Rendering::pixels_to_portion(mouse_pos.y, 'h');
			float mouse_distance_x = mouse_pos_x - (position_x + size_x * 0.5f);
			float mouse_distance_y = - (mouse_pos_y - (position_y + size_y * 0.5f));


			this->image_position_x += (image_position_x - mouse_distance_x) * -(1 - zoom_factor);
			this->image_position_y += (image_position_y - mouse_distance_y) * -(1 - zoom_factor);
		}

	}


	/*ImGui::InputFloat("x_pos", &this->position_x, 0.2, 0.05); // for debug - window controls
	ImGui::InputFloat("y_pos", &this->position_y, 0.2, 0.05);
	ImGui::InputFloat("x_scale", &this->size_x, 0.2, 0.05);
	ImGui::InputFloat("y_scale", &this->size_y, 0.2, 0.05);*/

	
	ImGui::End();
}

void Viewport::set_position(float x, float y)
{
	this->position_x = x;
	this->position_y = y;
}

void Viewport::set_window_size(float x, float y)
{
	this->size_x = x;
	this->size_y = y;
}

float Viewport::get_pos_x()
{
	return position_x;
}
float Viewport::get_pos_y()
{
	return position_y;
}
float Viewport::get_size_x()
{
	return size_x;
}
float Viewport::get_size_y()
{
	return size_y;
}
float Viewport::get_image_pos_x()
{
	return image_position_x;
}
float Viewport::get_image_pos_y()
{
	return image_position_y;
}
void Viewport::set_image_scale(float scale)
{
	this->image_scale = scale;
}
float Viewport::get_image_scale()
{
	return this->image_scale;
}

void Viewport::reset_sprite()
{
	this->image_position_x = 0;
	this->image_position_y = 0;
	this->image_scale = this->size_x * 1.5f;

}






SpriteViewport::SpriteViewport(float pos_x, float pos_y, float size_x, float size_y)
																		  : shader("assets/shaders/sprite_basic.vs", "assets/shaders/sprite_basic.fs"),
																			bg_shader("assets/shaders/sprite_background.vs", "assets/shaders/sprite_background.fs"),
																			Viewport(pos_x, pos_y, size_x, size_y)
{
	this->sprite.set_name("Unnamed Viewport Sprite");

	this->bg_sprite.set_name("Background");
	this->bg_sprite.load_from_file("assets/images/background.bmp");
	this->bg_texture.load(&this->bg_sprite, 'l');

	this->reset_sprite();
}

SpriteViewport::~SpriteViewport()
{
}



void SpriteViewport::render()
{
	//background render
	bg_shader.set_2d_float("window_centre", get_pos_x() + get_size_x() * 0.5f, 1.0f - get_pos_y() - get_size_y() * 0.5f);
	bg_shader.set_2d_float("window_size", get_size_x(), get_size_y());
	bg_shader.set_float("square_scale", 20.0f);
	bg_shader.set_float("window_size_ratio", (float)GLOBALS::HEIGHT / (float)GLOBALS::WIDTH);

	Rendering::render_viewport_bg(this);

	// front sprite render
	shader.set_2d_float("window_centre", get_pos_x() + get_size_x()*0.5f, 1.0f - get_pos_y() - get_size_y() * 0.5f);
	shader.set_2d_float("window_size", get_size_x(), get_size_y());
	shader.set_2d_float("image_position", get_image_pos_x(), get_image_pos_y());

	float sprite_width = Rendering::pixels_to_portion(this->sprite.get_width(), 'w');
	float sprite_height = Rendering::pixels_to_portion(this->sprite.get_height(), 'h');

	shader.set_float("sprite_size_ratio", sprite_height / sprite_width);
	shader.set_float("scale", get_image_scale());


	Rendering::render_viewport(this);


	__super::render(); // does imgui work for input
}





Shader* SpriteViewport::get_shader()
{
	return &this->shader;
}
RenderTexture* SpriteViewport::get_render_texture()
{
	return &this->render_texture;
}

Shader* SpriteViewport::get_bg_shader()
{
	return &this->bg_shader;
}
RenderTexture* SpriteViewport::get_bg_render_texture()
{
	return &this->bg_texture;
}

void SpriteViewport::set_sprite(Sprite spr)
{
	this->sprite = spr;
	this->render_texture = RenderTexture(&spr);
}

Sprite* SpriteViewport::get_sprite()
{
	return &this->sprite;
}

/*
TextureViewport::TextureViewport()
{
	this->texture.set_name("Unnamed Viewport Texture");
}

TextureViewport::~TextureViewport()
{
}
*/