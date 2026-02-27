#include <map>
#include <set>
#include <vector>
#include <Windows.h>

#include "unit.h"
#include "resource.h"

#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"

#include "../utility/store_load.h"

using namespace unit;

Unit::Resource::Resource() {
	auto resource{ FindResource(NULL, MAKEINTRESOURCE(IDB_BATUNITS), L"DATA") };
	auto source{ LockResource(LoadResource(NULL, resource)) };

	load::Resource{ source }
		.LoadResource(map3_vector_bitmaps,
			load::Resource::Label<
				label::knight, label::squire,
				side::avers, side::revers,
				action::attack, action::hit, action::stand>);
}

Unit::Unit(std::string_view label, std::string_view side) :
	map_vector_bitmaps{ std::invoke([&]() {
		std::map<std::string_view, std::vector<Texture>> texture;
		for (const auto& [label, bitmaps] : resource.map3_vector_bitmaps.at(label).at(side)) {
			std::vector<Texture> textures;
			for (const auto& bitmap : bitmaps) {
				textures.emplace_back(Texture{ bitmap });
			}
			texture.emplace(label, std::move(textures));
		}
		return texture;
		})}
{}
/*
const std::tuple<const Unit::order_t&, unsigned> Unit::OrderIDSizePosFrame(data_t* data) {
	const auto& film{ texture_bitmaps.at(action) };
	data->pos = film.Pos();
	data->size = film.Size();
	data->frame = film.Frame();
	return { order, film.ID(), };
}
*/