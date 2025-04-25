#include "renderer.h"


#include <iostream>


unsigned int Rendering::Rect::vertex_buffer = 0;
unsigned int Rendering::Rect::VAO = 0;
unsigned int Rendering::Rect::EBO = 0;
bool Rendering::Rect::is_init = false;

void Rendering::render_viewport(SpriteViewport* viewport)
{
    if (viewport->get_sprite()->get_data() == nullptr) { // validation - guard clause to check that sprite is intiialised
        std::cout << "Attempting to render a viewport with an uninitialised Sprite: Sprite `" << viewport->get_sprite()->get_name() << "`\n";
        return;
    }

    render_rect(viewport->get_shader(), viewport->get_render_texture());

}





void Rendering::render_viewport_bg(SpriteViewport* viewport)
{
    // background textures are always intiialised - so no need to check.

    render_rect(viewport->get_bg_shader(), viewport->get_bg_render_texture());
}



void Rendering::render_rect(Shader* shader, RenderTexture* texture)
{

    if (!Rect::get_is_init()) { Rect::init(); }


    shader->use();
    texture->use();

    glBindVertexArray(Rect::get_VAO());


    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

// for some reason this is vital to rendering that window is passed through clear
void Rendering::clear(GLFWwindow* window, float color[4]) {

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}


float Rendering::pixels_to_portion(int pixels, char dimension)
{
    switch (dimension)
    {
    case 'w':
    case 'x':
        return (float)pixels / (float)GLOBALS::WIDTH;

    case 'h':
    case 'y':
        return (float)pixels / (float)GLOBALS::HEIGHT;

    default:
        return 0;
    }
}

int Rendering::portion_to_pixels(float portion, char dimension)
{
    switch (dimension)
    {
    case 'w':
    case 'x':
        return (int)(portion * (float)GLOBALS::WIDTH);

    case 'h':
    case 'y':
        return (int)(portion * (float)GLOBALS::HEIGHT);

    default:
        return 0;
    }
}



void Rendering::Rect::init() {
    
    if (is_init) { return; }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_order), indices_order, GL_STATIC_DRAW);

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // colours
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    is_init = true;
}

void Rendering::Rect::deinit()
{
    if (!is_init) { return; }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertex_buffer);

    is_init = false;
}



bool Rendering::Rect::get_is_init()
{
    return is_init;
}
unsigned int Rendering::Rect::get_vertex_buffer()
{
    return vertex_buffer;
}
unsigned int Rendering::Rect::get_EBO()
{
    return EBO;
}
unsigned int Rendering::Rect::get_VAO()
{
    return VAO;
}



