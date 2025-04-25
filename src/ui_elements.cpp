#include "ui_elements.h"
#include "viewport.h"
#include "renderer.h"


unsigned int UIElement::ID_counter = 1;

UIElement::UIElement()
{
	this->ID = ID_counter;
	ID_counter++;

	this->window_title = "UIElement";
	this->tooltip = "Uninitialised tooltip";
}

void UIElement::render(bool enforce_square) // also manages inputs
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoBackground // comment this out if it must be visible
		;

	
	ImGui::SetNextWindowPos(ImVec2(Rendering::portion_to_pixels(this->position_x, 'w'), Rendering::portion_to_pixels(this->position_y, 'h')));

	ImVec2 window_size;
	if (enforce_square) {
		window_size = ImVec2(Rendering::portion_to_pixels(this->size_x, 'w') * GLOBALS::HEIGHT / GLOBALS::WIDTH, Rendering::portion_to_pixels(this->size_y, 'h'));
	}
	else {
		window_size = ImVec2(Rendering::portion_to_pixels(this->size_x, 'w'), Rendering::portion_to_pixels(this->size_y, 'h'));
	}
	ImGui::SetNextWindowSize(window_size);

	char* title = (char*)malloc(48);
	strncpy(title, window_title, 32);           // lines are fine, its been malloc`d
	strcat(title, std::to_string(ID).c_str()); // puts together title name

	ImGui::Begin(title, nullptr, window_flags);


	if (this->is_hovered != ImGui::IsWindowHovered()) { // if changed hover state
		this->is_hovered = ImGui::IsWindowHovered();
		switch (is_hovered)
		{
		case true:
			this->on_hover();
			break;

		case false:
			this->on_unhover();
			break;
		}
	}

	if (is_hovered && ImGui::IsMouseDown(0))
	{
		if (is_clicked == false) { // if just clicked
			this->is_clicked = true;
			this->on_click();
		}
	}
	else if (!ImGui::IsMouseDown(0)) {
		if (is_clicked == true) { // if just unclicked
			this->is_clicked = false;
			if (is_hovered) {
				this->on_unclick();
			}else {
				this->on_cancel_click(); // if mouse taken off button before unclicking
			}
		}
	}


	//tooltip
	if (is_hovered) { ImGui::SetTooltip(this->tooltip); }

	ImGui::End();
}

void UIElement::set_position(float x, float y)
{
	position_x = x;
	position_y = y;
}

void UIElement::set_size(float x, float y)
{
	size_x = x;
	size_y = y;
}

void UIElement::on_hover() {} // virtual fns
void UIElement::on_unhover() {}
void UIElement::on_click() {}
void UIElement::on_unclick() {}
void UIElement::on_cancel_click() {}





// ---- BUTTON

Widget::Widget()
	:shader_bg("assets/shaders/widget_bg.vs", "assets/shaders/widget_bg.fs")
{
	UIElement::window_title = "Widget";

	this->widget_sprite.set_name("Widget Sprite");
	this->widget_sprite.load_from_file("assets/images/missing.bmp");

	texture_bg.load(&this->widget_sprite, 'c'); // clamp texture to edge to give a border


	shader_bg.set_float("brightness", 1.0f);
	shader_bg.set_2d_float("image_scale", 1.0f, 1.0f);

}

void Widget::render()
{
	UIElement::render(true);

	float square_size_x = Rendering::pixels_to_portion(Rendering::portion_to_pixels(size_y, 'y'), 'x');

	shader_bg.set_2d_float("window_centre", position_x + square_size_x * 0.5f, 1.0f - position_y - size_y * 0.5f);
	shader_bg.set_2d_float("window_size", square_size_x, size_y);



	Rendering::render_rect(&this->shader_bg, &this->texture_bg);
}

void Widget::set_sprite(Sprite spr)
{
	this->widget_sprite = spr;
	texture_bg.load(&this->widget_sprite, 'c');
}


void Widget::on_hover()
{
	shader_bg.set_float("brightness", 0.9f);
}

void Widget::on_unhover()
{
	shader_bg.set_float("brightness", 1.0f);
}

void Widget::on_click()
{
	shader_bg.set_2d_float("image_scale", 0.8f, 0.8f);
}

void Widget::on_unclick()
{
	shader_bg.set_2d_float("image_scale", 1.0f, 1.0f);
}

void Widget::on_cancel_click()
{
	shader_bg.set_2d_float("image_scale", 1.0f, 1.0f);
}