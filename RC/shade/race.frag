#version 450 core
layout (location = 0) in vec2 land_texture_coord;
layout (location = 1) in vec2 menu_texture_coord;

layout (binding = 0) uniform sampler2D texture_land;
layout (binding = 1) uniform sampler2D texture_menu;

bool IsCoord(const vec2 coord) {
	return 0 <= coord.x && coord.x <= 1 && 0 <= coord.y && coord.y <= 1;
}

void main() {
	gl_FragColor = vec4(1,0,0,0);
	/*
	if (IsCoord(land_texture_coord))
		gl_FragColor = vec4(0,1,0,0);// texture(texture_land, land_texture_coord);
	*/
	gl_FragColor = texture(texture_land, land_texture_coord);
}