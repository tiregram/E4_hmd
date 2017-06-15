#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 uvPosition_modelspace;

out vec2 uv;
out vec3 Position_worldspace;

// Values that stay constant for the whole mesh
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat4 MVP;



void main(){


  
    gl_Position = MVP * vec4(vertexPosition_modelspace,1);
//  gl_Position = P * V * M * vec4(vertexPosition_modelspace,1);
//  gl_Position = (vec4(vertexPosition_modelspace,1));
//  gl_Position.w = 1.0;
	
	
	// UV of the vertex. No special space for this one.
	uv = uvPosition_modelspace;




}
