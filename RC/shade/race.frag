#version 450 core
layout (location = 0) in vec2 land_texture_coord;

/*layout (binding = 0)*/ uniform sampler2D texture_land;

void main() {
	gl_FragColor = texture(texture_land, land_texture_coord);
	//gl_FragColor = vec4(1,0,0,0);
}