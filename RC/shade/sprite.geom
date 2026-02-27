#version 450 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

//layout (location = 0) in vec4 texture_back_region[];

layout (location = 0) out vec2 texture_back_coord;

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
		gl_Position   = vec4(Point(gl_in[0].gl_Position, index), 0, 1);
		texture_back_coord = Point(vec4(1,1,0,0), index);
		EmitVertex();
	}
	EndPrimitive();
}