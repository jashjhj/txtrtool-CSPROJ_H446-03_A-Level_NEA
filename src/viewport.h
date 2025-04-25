#pragma once

#include "sprite.h"
#include "texture.h"
#include "imgui.h"
//#include "renderer.h"
#include "shader.h";
#include "render_texture.h"

class Viewport {
public:

	Viewport(float pos_x = 0.1f, float pos_y = 0.1f, float size_x = 0.1f, float size_y = 0.1f);
	~Viewport();

	void render(); // use renderer.render_viewport(window, this)

	void set_position(float x, float y);
	void set_window_size(float x, float y);

	float get_pos_x();
	float get_pos_y();
	float get_size_x();
	float get_size_y();
	float get_image_pos_x();
	float get_image_pos_y();

	void set_image_scale(float);
	float get_image_scale();

	void reset_sprite(); // depreciated, hard-sets scale = 0.5

protected: // for inheritance

	float image_position_x = 0;
	float image_position_y = 0;
	float image_scale = 1;

private:

	bool is_mouse_dragging = false;
	ImVec2 dragging_delta;

	float position_x;
	float position_y;

	float size_x;
	float size_y;

	unsigned int ID;
	static unsigned int ID_counter;

};

class SpriteViewport : public Viewport {
public:

	SpriteViewport(float pos_x = 0.1f, float pos_y = 0.1f, float size_x = 0.1f, float size_y = 0.1f);
	~SpriteViewport();


	void render();



	Shader* get_shader();
	RenderTexture* get_render_texture();


	Shader* get_bg_shader();
	RenderTexture* get_bg_render_texture();

	void set_sprite(Sprite);
	Sprite* get_sprite();


	Sprite sprite;
private:



	Shader shader;
	RenderTexture render_texture;

	Sprite bg_sprite;
	Shader bg_shader; // background shader & texture.
	RenderTexture bg_texture;

};

/*

class TextureViewport : Viewport{
public:

	TextureViewport(int pos_x = 10, int pos_y = 10, int size_x = 10, int size_y = 10);
	~TextureViewport();

	Texture texture;

};

*/