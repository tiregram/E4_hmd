#version 330 core

uniform sampler2D warpTexture;
uniform vec2 LensCenter;
uniform vec2 ViewportScale;

uniform float WarpScale;

uniform vec4 HmdWarpParam;
uniform vec3 aberr;

in vec2 uv;
out vec3 color;


void main(){


vec2 loc = vec2(uv.x, uv.y);

vec2 r = loc * ViewportScale - LensCenter;

r/=WarpScale;

float r_mag = length(r);
vec2 r_displaced = r*(HmdWarpParam.w +
                      HmdWarpParam.z * r_mag +
                      HmdWarpParam.y * r_mag*r_mag +
                      HmdWarpParam.x * r_mag*r_mag*r_mag);

r_displaced *= WarpScale;

vec2 tc_r = (LensCenter + aberr.r * r_displaced)/ViewportScale;
vec2 tc_g = (LensCenter + aberr.g * r_displaced)/ViewportScale;
vec2 tc_b = (LensCenter + aberr.b * r_displaced)/ViewportScale;

float red = texture2D(warpTexture, tc_r).r;
float green = texture2D(warpTexture, tc_g).g;
float blue = texture2D(warpTexture, tc_b).b;

//gl_FragColor = ((tc_g.x < 0.0) || (tc_g.x > 1.0) || (tc_g.y < 0.0) || (tc_g.y > 1.0)) ? vec4(0.0, 0.0, 0.0, 1.0) : vec4(red, green, blue, 1.0);

color = ((tc_g.x < 0.0) || (tc_g.x > 1.0) || (tc_g.y < 0.0) || (tc_g.y > 1.0)) ? vec3(0.0, 0.0, 0.0) : vec3(red, green, blue);


//if(r[0] > 0.0)
//color = vec3(1.0,0.0,0.0);

//else
//gl_FragColor = vec4(0.0,1.0,1.0,1.0);


}