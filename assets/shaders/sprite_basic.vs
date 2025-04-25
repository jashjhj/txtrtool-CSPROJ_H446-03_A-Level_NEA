#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVPos;
out vec2 UVPos;

uniform vec2 window_centre;
uniform vec2 window_size;
uniform vec2 image_position;

uniform float sprite_size_ratio;
uniform float scale;



float lerp(float from, float to, float how_much){
	return from + (to-from)*how_much;
}

void main()
{
	vec2 image_scale = vec2(1, sprite_size_ratio);
	image_scale *= scale;


	vec2 window_centre_gl = 2.0f*window_centre - vec2(1.0f,1.0f);
	vec2 image_centre_gl = window_centre_gl + image_position * 2.0f;

	vec2 new_pos = image_scale*aPos.xy + image_centre_gl;

	vec2 out_uv = aUVPos;


	vec2 old_pos = new_pos;

	if(new_pos.x > window_centre_gl.x + window_size.x){ // if too far to the right
		new_pos.x = window_centre_gl.x + window_size.x;
		out_uv.x =  lerp(aUVPos.x, 1.0f-aUVPos.x, (old_pos.x - new_pos.x)/image_scale.x);
	}
	else if(new_pos.x < window_centre_gl.x - window_size.x){ // too far to the left
		new_pos.x = window_centre_gl.x - window_size.x;
		out_uv.x =  lerp(aUVPos.x, 1.0f-aUVPos.x, -(old_pos.x - new_pos.x)/image_scale.x);
	}

	if(new_pos.y > window_centre_gl.y + window_size.y){ // up
		new_pos.y = window_centre_gl.y + window_size.y;
		out_uv.y =  lerp(aUVPos.y, 1.0f-aUVPos.y, (old_pos.y - new_pos.y)/image_scale.y);
	}
	else if(new_pos.y < window_centre_gl.y - window_size.y){ // down
		new_pos.y = window_centre_gl.y - window_size.y;
		out_uv.y =  lerp(aUVPos.y, 1.0f-aUVPos.y, -(old_pos.y - new_pos.y)/image_scale.y);
	}

	gl_Position = vec4(new_pos, aPos.z + 0.1f, 1.0);

	UVPos = out_uv;
}


