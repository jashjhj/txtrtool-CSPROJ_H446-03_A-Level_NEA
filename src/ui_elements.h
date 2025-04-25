#pragma once
#include "viewport.h"

class UIElement {
public:
	UIElement();


	void render(bool enforce_square = false);


	void set_position(float x, float y);
	void set_size(float x, float y);


protected:

	virtual void on_hover();
	virtual void on_unhover();
	virtual void on_click();
	virtual void on_unclick();
	virtual void on_cancel_click();

	bool is_hovered = false;
	bool is_clicked = false;




	float position_x = 0;
	float position_y = 0;

	float size_x = 0;
	float size_y = 0;

	char* window_title;
	char* tooltip;

private:

	unsigned int ID;
	static unsigned int ID_counter;
};


class Widget : public UIElement {
public:
	Widget();

	void render();


protected:
	void on_hover();
	void on_unhover();
	void on_click();
	void on_unclick();
	void on_cancel_click();

	void set_sprite(Sprite);


private:

	Sprite widget_sprite;

	Shader shader_bg; // background shader & loaded texture
	RenderTexture texture_bg;

};