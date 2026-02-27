#version 450 core
layout (location = 0) in ivec4 unit_size_pos;
layout (location = 1) in ivec2 unit_offset;

layout (location = 0) out vec4 unit_region;
layout (location = 1) out vec4 mark_region;

layout(std140) uniform buffer_battle {
	vec4 land_region;
	vec4 mark_small_region;
	vec4 mark_large_region;
};
layout(std140) uniform buffer_frame {
	vec2 frame_size;
	vec2 frame_pixel;
	vec2 frame_center;
};

void main() {
	gl_Position = land_region * frame_pixel.xyxy * 2;
}