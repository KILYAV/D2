#pragma once
#include <string>

#include "unit.h"
#include "domain.h"
#include "texture.h"

#undef small


namespace battle {
	constexpr std::string_view land = "land";
	constexpr std::string_view mark = "mark";
}
struct Battle //:
	//private Buffer
{
public:
	inline static constexpr size_t count_max_unit = 16;
	const Texture texture_land;
	const std::vector<Texture> textures_mark;
	const expr::map<group::column::list,
		expr::map<group::row::list, glm::ivec2>> offsets;
private:
	std::vector<std::unique_ptr<Unit>> alive;
	std::vector<std::unique_ptr<Unit>> dead;

	struct Resource {
		std::map<std::string_view, std::vector<Bitmap>> bitmaps_land;
		std::vector<Bitmap> bitmaps_mark;
		Resource();
	} static const resource;
public:
	Battle(std::string_view race);

	Battle& AddUnit(std::unique_ptr<Unit> unit);
	Battle& Run();
private:
	void CursorPosCallBack(double xpos, double ypos);
	void MouseCallBack(int button, int action, int mods);
};
#ifdef LEON_OPENGL_IMPLEMENTATION
	const Battle::Resource Battle::resource;
#endif
