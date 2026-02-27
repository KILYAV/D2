vec2 Point(const vec4 value, const int index) {
	vec2[4] point;
	point[0] = value.zy;
	point[1] = value.xy;
	point[2] = value.zw;
	point[3] = value.xw;
	return point[index];
}
ivec4 Region(ivec4 size_pos) {
	ivec4 result;
	result.x = ((size_pos.x + 1) >> 1) + size_pos.z;
	result.y = ((size_pos.y + 1) >> 1) + size_pos.w;
	result.z = result.x - size_pos.x;
	result.w = result.y - size_pos.y;
	return result;
}