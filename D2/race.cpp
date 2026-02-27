#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "race.h"
#include "texture.h"
#include "resource.h"

#include "../../utility/store_load.h"

const Shade::Race Shade::race;

Race::Resource::Resource() {
	std::map<std::string, Bitmap> source;
	load::Resource{ std::invoke([]() {
		auto find{ FindResourceA(NULL, MAKEINTRESOURCEA(IDB_RACE), "data") };
		auto load{ LoadResource(NULL, find) };
		return LockResource(load);
		}) }
	.LoadResource(source);

	for (auto& [label, bitmap] : source) {
		land.at(label) = std::move(bitmap);
	}
}
Race Race::GetRace() {
	expr::map<race::choice, Texture> land;
	for (const auto label : race::choice{}) {
		land.at(label) = Texture{ resource.land.at(label) };
	}

	Shade::race.Texture<shade::texture::land>(land.at(race::imperial).ID());

	auto windows{ glfwGetCurrentContext() };
	while (!glfwWindowShouldClose(windows)) {

	}

	return Race{ race::neutral };
}