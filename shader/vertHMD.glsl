#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

out vec2 uv;

void main(void)
{
	//gl_TexCoord[0] = gl_MultiTexCoord0;
  //gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

  gl_Position.xyz = vertexPosition_modelspace;
  gl_Position.w = 1.0;

  uv = vertexUV;
}