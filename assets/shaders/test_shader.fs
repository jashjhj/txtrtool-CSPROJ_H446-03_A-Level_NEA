#version 330 core

out vec4 FragColor;
in vec2 out_uv;

uniform sampler2D myTexture;


void main()
{
    FragColor = texture(myTexture, out_uv);// * brightness;
}



/*
 // old test basic


#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f); // magenta
}



*/