#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVPos;
out vec2 UVPos;

uniform vec2 window_centre;
uniform vec2 window_size;

uniform vec2 image_scale;


float lerp(float from, float to, float how_much){
	return from + (to-from)*how_much;
}

void main()
{

	vec2 new_pos = (2.0f*window_centre) - vec2(1.0f,1.0f) + aPos.xy*window_size * 2.0f;
	//new_pos = aPos.xy;

	//gl_Position = vec4(aPos.xyz, 1.0f);
	gl_Position = vec4(new_pos, aPos.z, 1.0f);


	UVPos = (aUVPos - 0.5f) / image_scale + 0.5f;

}


