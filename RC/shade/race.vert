#version 450 core
layout (location = 0) in ivec4 in_land_region;
layout (location = 1) in ivec4 in_menu_region;

layout (location = 0) out vec4 out_land_region;
layout (location = 1) out vec4 out_menu_region;

layout (std140, binding = 0) uniform block_frame {
	vec2 frame_size;
	vec2 frame_pos;
	vec2 frame_pixel;
};

vec2 Ratio(const vec4 region) {
	return vec2(1. / (region.x - region.z), 1. / (region.y - region.w));
}
vec4 Coord(const vec4 region, const vec2 ratio) {
	vec4 coord;
	coord.x = 1. + (1. - region.x) * .5 * ratio.x;
	coord.y = 1. + (1. - region.y) * .5 * ratio.y;
	coord.z = 0. - (1. + region.z) * .5 * ratio.x;
	coord.w = 0. - (1. + region.w) * .5 * ratio.y;
	return coord;
}

void main() {
	vec4 land_region = in_land_region * frame_pixel.xyxy;
	vec4 menu_region = in_menu_region * frame_pixel.xyxy;
	out_land_region = Coord(land_region, Ratio(land_region));
	out_menu_region = Coord(menu_region, Ratio(menu_region));
}