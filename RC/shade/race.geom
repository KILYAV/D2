#version 450 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (location = 0) in vec4 land_region[];
layout (location = 1) in vec4 menu_region[];

layout (location = 0) out vec2 land_texture_coord;
layout (location = 1) out vec2 menu_texture_coord;

vec2 Point(const vec4 value, const int index) {
	vec2[4] point;
	point[0] = value.zy;
	point[1] = value.xy;
	point[2] = value.zw;
	point[3] = value.xw;
	return point[index];
}

void main() {
	for (int index = 0; index < 4; ++index) {
		gl_Position = vec4(Point(vec4(+1,+1,-1,-1), index), 0, 1);
		land_texture_coord = Point(land_region[0], index);
		menu_texture_coord = Point(menu_region[0], index);
		EmitVertex();
	}
	EndPrimitive();
}