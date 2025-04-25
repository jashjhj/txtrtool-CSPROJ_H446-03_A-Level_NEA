#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVPos;
out vec2 UVPos;

uniform vec2 window_centre;
uniform vec2 window_size;


float lerp(float from, float to, float how_much){
	return from + (to-from)*how_much;
}

void main()
{

	vec2 new_pos = window_centre + aPos.xz*window_size;

	gl_Position = vec4(new_pos, aPos.z, 1.0f);
	vec2 out_uv = aUVPos;

}


