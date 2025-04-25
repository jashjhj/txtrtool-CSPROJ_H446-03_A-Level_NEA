#pragma once
#include "ui_elements.h"
#include "normal_generation.h"

class Button_Reset_Viewport : public Widget{
public:
	Button_Reset_Viewport(Viewport* attached_viewport);

private:
	void on_unclick();

	Viewport* attached_viewport;
};


class Button_Load_FS_Sprite : public Widget {
public:
	Button_Load_FS_Sprite(Sprite* attached_sprite);

	bool render();

private:
	void on_unclick();

	bool is_changed = 0;

	Sprite* attached_sprite;
};


class Button_Save_Sprite : public Widget {
public:
	Button_Save_Sprite(SpriteViewport* attached_sprite);

	bool render(Sprite loaded_sprite);

private:
	void on_unclick();

	bool is_modal_open = false;
	bool is_string_valid = false;
	bool is_done_modal_open = false;


	inline static const unsigned int BUFFER_SIZE = 256;
	char save_loc_string[BUFFER_SIZE];

	SpriteViewport* attached_sprite_vp;
};



namespace imgui_windows {
	bool customisation(float* sharpness, unsigned char* delta_threshold, Normalmap::EDGE_MODE* edge_mode, bool* is_half_resolution, bool* is_dx_colours );

	void help();
}