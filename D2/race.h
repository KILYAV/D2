#pragma once
#include <string>
#include <utility/expr.h>
#include <utility/bitmap.h>

#include "shade.h"

namespace race {
	constexpr std::string_view imperial = "imperial";
	constexpr std::string_view heretic = "heretic";
	constexpr std::string_view undead = "undead";
	constexpr std::string_view dwarf = "dwarf";
	constexpr std::string_view neutral = "neutral";

	using choice = expr::labels<imperial, heretic, undead, dwarf>;
}

struct Race {
public:
	const std::string_view label;
	Race(const std::string_view new_label) :
		label{ new_label }
	{}
private:
	struct Resource {
		expr::map<race::choice, Bitmap> land;
		Resource();
	} static const resource;
public:
	static Race GetRace();
};