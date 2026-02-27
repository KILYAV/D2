#pragma once
#include "uniform.h"

namespace uniform::buffer::label {
	constexpr std::string_view frame{ "frame" };
}

struct Frame {
private:
	struct GLAD {
	public:
		GLAD(const int width, const int height);
	};
	struct Data {
		glm::vec2 size;
		glm::vec2 pos;
		glm::vec2 pixel;

		Data(const glm::ivec2 size = { 0,0 });
	};
	using Buffer = Uniform::Buffer<uniform::buffer::label::frame, Data>;
	struct Instant :
		GLAD,
		Buffer
	{
		Instant(const int width, const int height);
	};

	static const Instant frame;
	Frame() = delete;
public:
	static const Frame::Instant& Size(const glm::ivec2 new_size);
	static glm::ivec2 Size();
};

/*
struct Buffer {
public:
	const unsigned FBO;
	Buffer();
	Buffer& Load(const Texture& texture);
};
*/
