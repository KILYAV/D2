#version 450 core
layout (location = 0) in ivec4 in_land_region;

layout (location = 0) out vec4 out_land_region;

layout (std140, binding = 0) uniform block_frame {
	vec2 frame_size;
	vec2 frame_pos;
	vec2 frame_pixel;
};

void main() {
	out_land_region = in_land_region * frame_pixel.xyxy;
}