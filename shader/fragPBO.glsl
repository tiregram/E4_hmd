#version 330 core


//UVS
in vec2 uv;

//outputColor
out vec4 color;


//texture to map to
uniform sampler2D renderedTexture;

void main(){

  color = texture( renderedTexture, uv);

}
