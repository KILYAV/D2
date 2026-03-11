#pragma once
#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <utility/expr.h>

#include "uniform.h"
#include "frame.h"

namespace shade::label {
	constexpr std::string_view race{ "race" };
	constexpr std::string_view sprite{ "sprite" };

	using list = expr::labels<race, sprite>;
}
namespace shade::texture {
	constexpr std::string_view land{ "land" };
	constexpr std::string_view menu{ "menu" };

	using race = expr::labels<land, menu>;
}
namespace shade::block {
	using race = expr::labels<uniform::buffer::label::frame>;
}
namespace shade::atribute {
	constexpr std::string_view in_land_region{ "in_land_region" };
	constexpr std::string_view in_menu_region{ "in_menu_region" };

	using race = expr::tuple<
		expr::value<in_land_region, glm::ivec4>,
		expr::value<in_menu_region, glm::ivec4>>;
}

struct Shade {
private:
	struct Resource {
		expr::map<shade::label::list, unsigned> shades;
		Resource();
	} static const resource;

	struct Base {
	public:
		const unsigned prog;
		const unsigned VAO;
		const unsigned VBO;

		Base(std::string_view label);
		void Run() const;

	protected:
		void Texture(const unsigned index, const unsigned id) const;
		//void Block(const unsigned tx, const unsigned id);
		void Attribute(const unsigned size) const;
		void Attribute(const unsigned offset, const unsigned size, const unsigned index) const;
		void Attribute() const;
		void Vertex(const unsigned offset, const unsigned size, void* attr) const;
	};
private:
	template<const std::string_view& label, class Textures, class Blocks, class Attributes>
	struct Instant :
		Base
	{
		Instant();
		template<const std::string_view& label>
		void Texture(const unsigned id) const {
			Base::Texture(Textures::Index(label), id);
		}
		template<const std::string_view& label>
		void Vertex(Attributes::template Value<label>::template type attr) const {
			Base::Vertex(Attributes::template Offset<label>(), sizeof(attr), &attr);
		}
	private:
		template<unsigned... Is>
		void Attribute(std::integer_sequence<unsigned, Is...>) const;
	};
public:
	using Race = Instant<shade::label::race, shade::texture::race, shade::block::race, shade::atribute::race>;
	//using Sprite = Instant<shade::label::race, shade::texture::race, shade::block::race>;

	static const Race race;
	//static Sprite sprite;
};
template<const std::string_view& label, class Textures, class Blocks, class Attributes>
Shade::Instant<label, Textures, Blocks, Attributes>::Instant() :
	Base{ label }
{
	Attribute(std::make_integer_sequence<unsigned, Attributes::size>{});
}
template<const std::string_view& label, class Textures, class Blocks, class Attributes>
template<unsigned... Is>
void Shade::Instant<label, Textures, Blocks, Attributes>::Attribute(std::integer_sequence<unsigned, Is...>) const {
	Base::Attribute((Attributes::template Size<Is>() + ...));
	(Base::Attribute(Attributes::template Offset<Is>(), Attributes::template Size<Is>(), Is), ...);
	Base::Attribute();
}