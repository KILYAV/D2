#include "domain.h"

auto LoadBodyShade(const std::filesystem::path& direct) {
	auto bodies{ LoadVectorBitmap(direct / "body") };
	auto shades{ LoadVectorBitmap(direct / "shade") };

	const int count = bodies.size();
	if (count != shades.size())
		throw std::out_of_range{ "not match count bodies & shades" };

	std::vector<Bitmap> bitmaps;
	for (int index = 0; index < count; ++index) {
		auto [size, pos] { util::MaxSizePos(util::MaxRegion(bodies), util::MaxRegion(shades)) };
		bitmaps.emplace_back(size, pos).Insert(shades[index]).Insert(bodies[index]);
	}
	return bitmaps;
}
auto LoadHitStandAttack(const std::filesystem::path& direct) {
	std::map<std::string, std::vector<Bitmap>> bitmaps;
	bitmaps.emplace("hit", LoadBodyShade(direct / "hit"));
	bitmaps.emplace("stand", LoadBodyShade(direct / "stand"));
	bitmaps.emplace("attack", LoadBodyShade(direct / "attack"));
	return bitmaps;
}
auto LoadAversRevers(const std::filesystem::path& direct) {
	std::map<std::string, std::map<std::string, std::vector<Bitmap>>> bitmaps;
	bitmaps.emplace("avers", LoadHitStandAttack(direct / "avers"));
	bitmaps.emplace("revers", LoadHitStandAttack(direct / "revers"));
	return bitmaps;
}
std::map<std::string, std::map<std::string, std::map<std::string, std::vector<Bitmap>>>>
	LoadBatUnits(const std::filesystem::path& direct) {
	std::map<std::string, std::map<std::string, std::map<std::string, std::vector<Bitmap>>>> units;
	for (const auto& unit : std::filesystem::directory_iterator(direct)) {
		std::string label{ unit.path().filename().string() };
		units.emplace(label, LoadAversRevers(direct / unit));
	}
	return units;
}