#version 330 core

out vec4 color;

in vec2 uv;

uniform sampler2D renderedTexture;

void main(){

  color = texture( renderedTexture, uv);

}
