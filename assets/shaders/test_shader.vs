#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVPos;

out vec2 out_uv;

void main(){
	gl_Position = vec4(aPos, 1.0f);
	vec2 out_uv = aUVPos;
}



// nicer test - old code
/*
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVPos;
out vec2 UVPos;

uniform vec2 window_centre;
uniform vec2 window_size;


uniform float square_scale;
uniform float window_size_ratio;

const float extra_size = 1.0f;

float lerp(float from, float to, float how_much){
	return from + (to-from)*how_much;
}

void main()
{
	vec2 image_scale = vec2(window_size_ratio, 1.0f);
	vec2 out_uv = aUVPos;

	//image_scale *= square_scale;


	vec2 window_centre_gl = 2.0f*window_centre - vec2(1.0f,1.0f);

	vec2 new_pos = extra_size*image_scale*aPos.xy + window_centre_gl;




	vec2 old_pos = new_pos;

	
	if(new_pos.x > window_centre_gl.x + window_size.x){ // if too far to the right
		new_pos.x = window_centre_gl.x + window_size.x;
		out_uv.x =  lerp(aUVPos.x, extra_size-aUVPos.x, (old_pos.x - new_pos.x)/image_scale.x/extra_size);
	}
	else if(new_pos.x < window_centre_gl.x - window_size.x){ // too far to the left
		new_pos.x = window_centre_gl.x - window_size.x;
		out_uv.x =  lerp(aUVPos.x, extra_size-aUVPos.x, -(old_pos.x - new_pos.x)/image_scale.x/extra_size);
	}

	if(new_pos.y > window_centre_gl.y + window_size.y){ // up
		new_pos.y = window_centre_gl.y + window_size.y;
		out_uv.y =  lerp(aUVPos.y, extra_size-aUVPos.y, (old_pos.y - new_pos.y)/image_scale.y/extra_size);
	}
	else if(new_pos.y < window_centre_gl.y - window_size.y){ // down
		new_pos.y = window_centre_gl.y - window_size.y;
		out_uv.y =  lerp(aUVPos.y, extra_size-aUVPos.y, -(old_pos.y - new_pos.y)/image_scale.y/extra_size);
	}
	

	gl_Position = vec4(new_pos, aPos.z, 1.0f);

	UVPos = out_uv * square_scale;
}


*/