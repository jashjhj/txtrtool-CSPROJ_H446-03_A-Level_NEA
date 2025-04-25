#version 330 core

out vec4 FragColor;
in vec2 UVPos;

uniform float brightness = 1.0f;

uniform sampler2D myTexture;

void main()
{
    FragColor = texture(myTexture, UVPos) * brightness;
}

