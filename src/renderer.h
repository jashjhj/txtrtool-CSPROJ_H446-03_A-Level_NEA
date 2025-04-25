#pragma once

#include "glad.h"
#include "glfw3.h"

#include "shader.h"
#include "viewport.h"
#include "sprite.h"
#include "texture.h"
#include "globals.h"
#include "ui_elements.h"


namespace Rendering {
	void render_viewport(SpriteViewport*);
	//void render_viewport(TextureViewport*);

	void render_viewport_bg(SpriteViewport*);



	void render_rect(Shader*, RenderTexture*); // use this, going forward

	void clear(GLFWwindow*, float color[4]); // for some reason this is vital to rendering that window is passed through clear

	float pixels_to_portion(int pixels, char width_or_height); // 'w' or 'h'
	int portion_to_pixels(float portion, char width_or_height);





	class Rect {
	public:
		static void init();
		static void deinit();

		static bool get_is_init();

		static unsigned int get_vertex_buffer();
		static unsigned int get_EBO();
		static unsigned int get_VAO();

	private:
		static unsigned int vertex_buffer;
		static unsigned int EBO;
		static unsigned int VAO;
		static bool is_init;
	};

	// consts
	const float vertex_data[20] = {
		// positions       // uv
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // bottom right
	   -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom left
	   -0.5f,  0.5f, 0.0f, 0.0f, 0.0f  // top left 
	};
	const unsigned int indices_order[6] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
}