#version 450 core
layout (location = 0) in vec2 texture_back_coord;
//layout (location = 1) in vec2 texture_front_coord;

uniform sampler2D texture_land;

void main() {
	gl_FragColor = texture(texture_land, texture_back_coord);
	//gl_FragColor = vec4(1,0,0,0);
}