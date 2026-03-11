#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <utility/store_load.h>

#include "race.h"
#include "texture.h"
#include "resource.h"

const Shade::Race Shade::race;

Race::Resource::Resource() {
	std::map<std::string, std::vector<char>> source;
	load::Resource{ std::invoke([]() {
		auto find{ FindResourceA(NULL, MAKEINTRESOURCEA(IDB_RACE), "data") };
		auto load{ LoadResource(NULL, find) };
		return LockResource(load);
		}) }
	.LoadResource(source);

	std::map<std::string, Bitmap> land_temp;
	load::Resource{ source.at("land").data() }.LoadResource(land_temp);
	for (auto& [label, bitmap] : land_temp) {
		land.at(label) = std::move(bitmap);
	}
	std::map<std::string, Bitmap> menu_temp;
	load::Resource{ source.at("menu").data() }.LoadResource(menu_temp);
	menu = std::move(menu_temp.at("menu"));
}
Race Race::GetRace() {
	expr::map<race::choice, Texture> land;
	for (const auto label : race::choice{}) {
		land.at(label) = Texture{ resource.land.at(label) };
	}
	Texture menu = Texture{ resource.menu };

	Shade::race.Texture<shade::texture::land>(land.at(race::imperial).ID());
	Shade::race.Texture<shade::texture::menu>(menu.ID());

	Shade::race.Vertex<shade::atribute::in_land_region>(land.at(race::imperial).Region());
	Shade::race.Vertex<shade::atribute::in_menu_region>(menu.Region());

	auto windows{ glfwGetCurrentContext() };
	while (!glfwWindowShouldClose(windows)) {
		Shade::race.Run();
	}

	return Race{ race::neutral };
}