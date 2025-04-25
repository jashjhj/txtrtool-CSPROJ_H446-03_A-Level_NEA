
#include "ui_element_instances.h"
#include <string>

Button_Reset_Viewport::Button_Reset_Viewport(Viewport* viewport)
{
	this->attached_viewport = viewport;

	UIElement::window_title = "Button_Reset_Viewport";
	Sprite reset_viewport_sprite("Button_Reset_Viewport Sprite");
	reset_viewport_sprite.load_from_file("assets/images/widgets/reset_position.bmp");
	Widget::set_sprite(reset_viewport_sprite);


	Widget::tooltip = "Resets the sprite's location and scale in the Viewport.";

}

void Button_Reset_Viewport::on_unclick()
{
	Widget::on_unclick();

	// command
	attached_viewport->reset_sprite();
}


#include "windows_io.h"

Button_Load_FS_Sprite::Button_Load_FS_Sprite(Sprite* sprite)
{
	this->attached_sprite = sprite;

	UIElement::window_title = "Button_Load_FS_Sprite";
	Sprite load_fs_sprite("Button_Load_FS_Sprite Sprite");
	load_fs_sprite.load_from_file("assets/images/widgets/load.bmp");
	Widget::set_sprite(load_fs_sprite);


	Widget::tooltip = "Load a sprite from your filesystem.";
}

bool Button_Load_FS_Sprite::render()
{
	Widget::render();

	if (is_changed) { is_changed = false; return true; }
	return false;
}

void Button_Load_FS_Sprite::on_unclick()
{
	Widget::on_unclick();

	Sprite sprite = load_sprite_from_filesystem();
	if (sprite.get_data() == nullptr) {
		return;
	}
	*this->attached_sprite = sprite; // sets new sprite
	this->is_changed = true;
}






Button_Save_Sprite::Button_Save_Sprite(SpriteViewport* sprite_vp)
{
	this->attached_sprite_vp = sprite_vp;

	std::cout << (long long unsigned int)this->attached_sprite_vp;

	UIElement::window_title = "Button_Save_Sprite";
	Sprite save_sprite("Button_Save_Sprite Sprite");
	save_sprite.load_from_file("assets/images/widgets/save.bmp");
	Widget::set_sprite(save_sprite);

	Widget::tooltip = "Save the normal-map sprite to your filesystem";

	strnset(save_loc_string, '?', 3); // initialises the string
	strset(save_loc_string + 3, '\00');
}

bool Button_Save_Sprite::render(Sprite loaded_sprite)
{
	Widget::render();



	if (is_modal_open) { ImGui::OpenPopup("Save Menu"); }


	if (ImGui::BeginPopupModal("Save Menu", &is_modal_open, ImGuiChildFlags_AlwaysAutoResize)) {
		static int extension_num = 0;



		if (ImGui::InputText(" = Location", save_loc_string, BUFFER_SIZE, ImGuiInputTextFlags_ElideLeft)) {
			char* final_dot = strrchr(save_loc_string, '.');

			if (final_dot != nullptr) { // check extension against known ones
				is_string_valid = true; 
				if (strcmp(final_dot, ".bmp") == 0) { extension_num = 0; }
				else if (strcmp(final_dot, ".png") == 0) { extension_num = 1; }
				else if (strcmp(final_dot, ".jpg") == 0) { extension_num = 2; }
				else if (strcmp(final_dot, ".jpeg") == 0) { extension_num = 2; }
				else { is_string_valid = false; }
			}
			else {
				is_string_valid = false;
			}
		}

		if (ImGui::Button("Choose save Location..")) {
			char save_location_temp[256];
			strcpy(save_location_temp, save_image_file().data());
			if (save_location_temp[0] != 0x00) { strcpy(save_loc_string, save_location_temp); } // ensures that a location is picked.



			char* final_dot = strrchr(save_loc_string, '.');

			if (final_dot != nullptr) { // check extension against known ones
				is_string_valid = true;
				if (strcmp(final_dot, ".bmp") == 0) { extension_num = 0; }
				else if (strcmp(final_dot, ".png") == 0) { extension_num = 1; }
				else if (strcmp(final_dot, ".jpg") == 0) { extension_num = 2; }
				else if (strcmp(final_dot, ".jpeg") == 0) { extension_num = 2; }
				else { is_string_valid = false; }
			}
			else {
				is_string_valid = false;
			}
		}

		ImGui::Separator();

		// dropdown to set extension
		if (ImGui::Combo("Extension", &extension_num, "BMP\0PNG\0JPG")) {

			char extension[5] = ".bmp";
			switch (extension_num){ // sets appropriate extension string based on dropdown
			case 1:
				strcpy(extension, ".png");
				break;
			case 2:
				strcpy(extension, ".jpg");
				break;
			}

			char* final_dot = strrchr(save_loc_string, '.'); // writes extension to the string
			if (final_dot == nullptr
				|| (strlen(final_dot) > 5)) { // guard clasue for if there is a . early on
				strcpy(save_loc_string + strlen(save_loc_string), extension);
			}
			else {
				strcpy(final_dot, extension);
			}

			is_string_valid = true;
		} 

		if (is_string_valid) {
			if (ImGui::Button("Export")) { // Export!!

				std::cout << loaded_sprite.get_extension();
				char* export_location = loaded_sprite.prepare_for_export(this->save_loc_string);
				std::cout << loaded_sprite.get_extension();

				if (export_location != nullptr) { 
					loaded_sprite.export_as(export_location);
					this->is_done_modal_open = true;
				}
				else { this->is_string_valid = false; }

					

			}
		}
		else {
			ImGui::Text("Cannot Export - Extension Invalid. Please use the drop-down to select an extension."); // info if invalid
		}

		// second window - completed save
		if (is_done_modal_open) { ImGui::OpenPopup("File Saved!"); }

		if (ImGui::BeginPopupModal("File Saved!", NULL, ImGuiChildFlags_AlwaysAutoResize)) {
			if (ImGui::Button("=   OK   =")) { is_done_modal_open = false; is_modal_open = false; }
			ImGui::EndPopup();
		}

		ImGui::EndPopup();
	}

	return false;
}



void Button_Save_Sprite::on_unclick()
{
	Widget::on_unclick();

	is_modal_open = true;
	is_string_valid = false;


}











bool imgui_windows::customisation(float* sharpness, unsigned char* delta_threshold, Normalmap::EDGE_MODE* edge_mode, bool* is_half_resolution, bool* is_dx_colours)
{
	bool is_anything_changed = false;


	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGui::Begin("Customisation", nullptr, window_flags);


	// Sharpness

	ImGuiSliderFlags cannot_manual = ImGuiSliderFlags_NoInput; // ImGuiSliderFlags_Logarithmic;

	ImGui::Text("Multiplier for gradients.");
	is_anything_changed += ImGui::SliderFloat("Sharpness", sharpness, -4.0f, 4.0f, "%.3f");
	ImGui::Separator();

	// Delta Threshold


	ImGui::Text("Threshold for maximum gradient.");
	int delta_threshold_temp = (int)*delta_threshold;
	is_anything_changed += ImGui::SliderInt("Delta Threshold", &delta_threshold_temp, 0, 255);
	*delta_threshold = (unsigned char)delta_threshold_temp;
	ImGui::Separator();

	// EDGE MODE

	ImGui::Text("What to do on the edge of the image:");
	int edge_mode_temp = (*edge_mode == Normalmap::EDGE_MODE::LOOP) ? 0 : 1;
	is_anything_changed += ImGui::SliderInt("Edge Mode", &edge_mode_temp, 0, 1, edge_mode_temp?"CLAMP":"LOOP", cannot_manual);
	*edge_mode = (edge_mode_temp == 0) ? Normalmap::EDGE_MODE::LOOP : Normalmap::EDGE_MODE::EXTEND;
	ImGui::Separator();

	// Half res

	ImGui::Text("Generate sprite at Half resolution?");
	int is_half_resolution_temp = *is_half_resolution;
	is_anything_changed += ImGui::SliderInt("Half resolution", &is_half_resolution_temp, 0, 1, (*is_half_resolution? "True" : "False"), cannot_manual);
	*is_half_resolution = is_half_resolution_temp % 2;
	ImGui::Separator();

	// DirectX Colours

	ImGui::Text("Use data encoding for DirectX Applications");
	int is_dx_colours_temp = *is_dx_colours;
	is_anything_changed += ImGui::SliderInt("DirectX Colours", &is_dx_colours_temp, 0, 1, (*is_dx_colours ? "True" : "False"), cannot_manual);
	*is_dx_colours = is_dx_colours_temp % 2;
	ImGui::Separator();

	static bool generate_only_on_press = 0;
	ImGui::Text("Generate automatically, or only on button press?");
	ImGui::SliderInt("Generation style", (int*)&generate_only_on_press, 0, 1, (generate_only_on_press ? "Manual" : "Automatic"), cannot_manual);

	if (generate_only_on_press) { // is_anything_changed override
		ImGui::Text("Use this is things become unresponsive\non automatic generation.");
		is_anything_changed = ImGui::Button("Generate!");
	}

	ImGui::End();
	return is_anything_changed;
}

void imgui_windows::help()
{
	ImGui::Begin("Help");

	ImGui::Text("Click the Load-File button to load a height-map.");
	ImGui::Text("Use the settings window to alter the generation.");
	ImGui::Text("Click the Export button to save your normal-map.");

	ImGui::End();
}
