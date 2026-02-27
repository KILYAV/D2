#pragma once
#include <string>
#include <glm/glm.hpp>
#include <utility/expr.h>

struct Uniform {
public:
	struct Battle {
		glm::vec4 land_region;
		glm::vec4 mark_region;

		Battle() = default;
	};
private:
	struct Base {
		const unsigned id;

		Base(const unsigned size);
		void bind(const unsigned base) const;
	protected:
		void load(const unsigned size, const void* data) const;
	};
public:
	template<const std::string_view& label, class Data>
	struct Buffer :
		Base
	{
	public:
		Buffer() :
			Base(sizeof(Data))
		{}
		Buffer(const Data& data) :
			Base(sizeof(Data))
		{
			load(data);
		}

		const Buffer& load(const Data& data) const {
			Base::load(sizeof(Data), &data);
			return *this;
		}
	};
};