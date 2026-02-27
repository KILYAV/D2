#version 450 core
layout (location = 0) in ivec4 land_size_pos;

layout (location = 0) out vec4 land_region;

layout (std140, binding = 0) uniform block_frame {
	vec2 frame_size;
	vec2 frame_pos;
	vec2 frame_pixel;
};

ivec4 Region(ivec4 size_pos) {
	ivec4 result;
	result.x = ((size_pos.x + 1) >> 1) + size_pos.z;
	result.y = ((size_pos.y + 1) >> 1) + size_pos.w;
	result.z = result.x - size_pos.x;
	result.w = result.y - size_pos.y;
	return result;
}

void main() {
	land_region = Region(land_size_pos) * frame_pixel.xyxy;
}