#include "domain.h"

auto LoadMenu() {

}
std::map<std::string, std::vector<char>> LoadInterf(const std::filesystem::path& direct) {
	auto choose{ LoadMapPng(direct / "choose" / "race" / "back") };
	//auto menu{ LoadMenu(direct / "choose" / "race" / "menu") };

	std::map<std::string, std::vector<char>> interf;
	interf.emplace("choose", store::Resource{}.StoreResource(choose));

	return interf;
}