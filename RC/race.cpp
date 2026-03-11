#include "domain.h"

std::map<std::string, Bitmap> Land(const std::filesystem::path& direct) {
	std::map<std::string, Bitmap> source;
	source.emplace("dwarf", LoadBitmap(direct / "dwarf.png").MirrorXX());
	source.emplace("undead", LoadBitmap(direct / "undead.png").MirrorXX());
	source.emplace("heretic", LoadBitmap(direct / "heretic.png").MirrorXX());
	source.emplace("imperial", LoadBitmap(direct / "imperial.png").MirrorXX());
	return source;
}
std::map<std::string, Bitmap> Menu(const std::filesystem::path& direct) {
	std::map<std::string, Bitmap> source;
	source.emplace("menu", LoadBitmap(direct / "menu.png").MirrorXX());
	return source;
}
std::map<std::string, std::vector<char>> LoadRace(const std::filesystem::path& direct) {
	auto land{ Land(direct / "land") };
	auto menu{ Menu(direct / "menu") };

	std::map<std::string, std::vector<char>> race;
	race.emplace("land", store::Resource{}.StoreResource(land));
	race.emplace("menu", store::Resource{}.StoreResource(menu));

	return race;
}