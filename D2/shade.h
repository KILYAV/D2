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

	using race = expr::labels<land>;
}
namespace shade::block {
	using race = expr::labels<uniform::buffer::label::frame>;
}
namespace shade::atribute {
	constexpr std::string_view land_size_pos{ "land_size_pos" };

	using race = expr::tuple<expr::value<land_size_pos, glm::ivec4>>;
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

	protected:
		void Run();
		void Texture(const unsigned index, const unsigned id) const;
		//void Block(const unsigned tx, const unsigned id);
		void Attribute(const unsigned size) const;
		void Attribute(const unsigned index, const unsigned size) const;
		void Attribute() const;
		template<class Type>
		void Attribute(const unsigned index, const Type& attr) const {
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Type), &attr);
		};
	};
private:
	template<const std::string_view& label, class Textures, class Blocks, class Attributes>
	struct Instant :
		Base,
		Attributes
	{
		Instant();
		template<const std::string_view& label>
		void Texture(const unsigned id) const {
			Base::Texture(Textures::Index<label>::value, id);
		}
		template<const std::string_view& label>
		void Attribute(Attributes::template value<label>::template type& attr) const {
			Base::Attribute(attr);
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
	Base::Attribute((Attributes::template value<Is>::size + ...));
	(Base::Attribute(Is, Attributes::template value<Is>::size), ...);
	Base::Attribute();
}