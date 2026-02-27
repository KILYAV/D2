#include "domain.h"

auto LoadMark(const std::filesystem::path& direct) {
	std::vector<Bitmap> bitmaps{ LoadVectorBitmap(direct) };
	auto [size, pos] { util::SizePos(util::MaxRegion(bitmaps)) };

	for (auto& bitmap : bitmaps) {
		Bitmap temp{ size, pos };
		temp.Insert(bitmap);
		bitmap = temp;
	}
	return bitmaps;
}
auto LoadBack(const std::filesystem::path& direct) {
	std::map<std::string, std::vector<png::Png>> back;
	for (const auto& sub_direct : std::filesystem::directory_iterator(direct)) {
		std::string label{ sub_direct.path().filename().string() };
		back.emplace(label, LoadVectorPng(direct / sub_direct));
	}
	return back;
}
std::map<std::string, std::vector<char>> LoadBattle(const std::filesystem::path& direct) {
	auto mark{ LoadMark(direct / "mark" / "large") };
	auto back{ LoadBack(direct / "land") };

	std::map<std::string, std::vector<char>> battle;
	battle.emplace("mark", store::Resource{}.StoreResource(mark));
	battle.emplace("land", store::Resource{}.StoreResource(back));

	return battle;
}