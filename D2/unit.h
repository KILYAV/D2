#pragma once
#include <map>
#include <string>

#include "../utility/expr.h"
#include "texture.h"

namespace unit::action {
	constexpr std::string_view hit = "hit";
	constexpr std::string_view stand = "stand";
	constexpr std::string_view attack = "attack";
	constexpr std::string_view weapon = "weapon";

	constexpr std::string_view strike = "strike";
	constexpr std::string_view damage = "damage";
}
namespace unit::side {
	constexpr std::string_view avers = "avers";
	constexpr std::string_view revers = "revers";
}
namespace unit::label {
	constexpr std::string_view squire = "squire";
	constexpr std::string_view knight = "knight";
}
namespace unit {
	expr::label<action::hit, action::stand, action::attack> actions;
	expr::label<side::avers, side::revers> sides;
}
struct Unit {
public:
	const std::map<std::string_view, std::vector<Texture>> map_vector_bitmaps;
private:
	std::string_view action = unit::action::stand;
	struct order_t {
		std::string_view column;
		std::string_view row;
	} order;
	struct frame_t {
		unsigned count;
		unsigned number;
	} frame;

	struct Resource {
		using Map3VectorBitmap =
			std::map<std::string_view,
				std::map<std::string_view,
					std::map<std::string_view,
						std::vector<Bitmap>>>>;

		Map3VectorBitmap map3_vector_bitmaps;
		Resource();
	} static const resource;
public:
	struct data_t {
		glm::vec2 size;
		glm::vec2 pos;
		glm::vec2 frame;
		glm::vec2 offset;
	};

	Unit() = default;
	Unit(std::string_view label, std::string_view side);

	const order_t& Order() {
		return order;
	}
	Unit& Order(std::string_view column, std::string_view row) {
		order.column = column;
		order.row = row;
		return *this;
	}
};
#ifdef LEON_OPENGL_IMPLEMENTATION
	const Unit::Resource Unit::resource;
#endif

namespace group::column {
	constexpr std::string_view back = "back";
	constexpr std::string_view front = "front";
	using list = expr::label<column::front, column::back>;
}
namespace group::row::left {
	constexpr std::string_view left = "left_left";
	constexpr std::string_view center = "left_center";
}
namespace group::row::right {
	constexpr std::string_view right = "right_right";
	constexpr std::string_view center = "right_center";
}
namespace group::row {
	using list = expr::label<left::left, left::center, right::right, right::center>;
}
struct Party {

};
struct Army {

};
