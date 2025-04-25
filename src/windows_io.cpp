#include "windows_io.h"


#include "portable-file-dialogs.h"


std::string open_image_file() {
	
	auto file_result = pfd::open_file("Open Image File", ".",
		{ "Image Files", "*.bmp *.png *.jpg *.jpeg" }
	);

	auto selection = file_result.result();

	if (!selection.empty()) {
		return selection[0];
	}
	else { return ""; }

}

std::string pick_folder() {
	auto folder_result = pfd::select_folder("Choose Folder", ".");

	return folder_result.result();
}

std::string save_image_file() {
	auto file_result = pfd::save_file("Save Image File", ".",
		{ "Image Files", "*.bmp *.png *.jpg *.jpeg" }
	);

	return file_result.result();
}



Sprite load_sprite_from_filesystem()
{
	Sprite new_sprite;
	std::string file_loc = open_image_file();
	if (file_loc.data() == "") { // validation - ensure there is a file present
		printf("Failed to load sprite - at file dialogue.");
		return new_sprite;
	}

	char* last_slash = strrchr(file_loc.data(), '\\');
	char* last_dot = strrchr(file_loc.data(), '.');

	// if there is no slash / no dot / slash after dot
	if (last_slash == nullptr || last_dot == nullptr || last_slash > last_dot) {
		printf("Failed to load sprite - at file dialogue parsing");
		return new_sprite;
	}

	char* name = (char*)malloc(last_dot - last_slash); // sets name
	strncpy(name, last_slash + 1, last_dot - last_slash - 1);
	strcat(name, "\00");

	new_sprite.set_name(name);

	free(name);

	new_sprite.set_extension(new_sprite.get_extension_from_string(last_dot + 1));

	new_sprite.load_from_file(file_loc.data());

	return new_sprite;
}



bool save_sprite_to_filesystem(Sprite* output) {

	return true;
}

