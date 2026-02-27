#include "domain.h"


std::map<std::string, Bitmap> LoadRace(const std::filesystem::path& direct) {
	std::map<std::string, Bitmap> source;
	source.emplace("dwarf", LoadBitmap(direct / "dwarf.png"));
	source.emplace("undead", LoadBitmap(direct / "undead.png"));
	source.emplace("heretic", LoadBitmap(direct / "heretic.png"));
	source.emplace("imperial", LoadBitmap(direct / "imperial.png"));
	return source;
}