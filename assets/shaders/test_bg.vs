#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVPos;
out vec2 UVPos;

uniform vec2 window_centre;
uniform vec2 window_size;

uniform vec2 offset;

uniform float square_scale;
uniform float window_size_ratio;

const float extra_size = 1.0f;

float lerp(float from, float to, float how_much){
	return from + (to-from)*how_much;
}

void main()
{
	vec2 new_pos = aPos.xy * 0.5;

	gl_Position = vec4(new_pos + offset, aPos.z, 1.0f);

	UVPos = aUVPos;
}


