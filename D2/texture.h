#pragma once
#include <string>
#include <glm/glm.hpp>

#include "../../utility/bitmap.h"

struct Texture {
private:
	glm::ivec2 size{ 0,0 };
	glm::ivec2 pos{ 0,0 };
	unsigned id = -1;
public:
	using pixel_type = bitmap::RGBA;

	Texture() = default;
	Texture(Texture&& other);
	Texture(const Bitmap& bitmap);
	Texture(const unsigned id, const glm::ivec2 pos = {});
	~Texture();

	Texture& operator= (Texture&& other);
	glm::ivec2 Pos() const {
		return pos;
	}
	glm::ivec2 Size() const {
		return size;
	}
	glm::ivec4 Region() const {
		return util::Region(size, pos);
	}
	Texture& Pos(const glm::ivec2 new_pos) {
		pos = new_pos;
		return *this;
	}
	Texture& Size(const glm::ivec2 new_size);
	unsigned ID() const {
		return id;
	}

	Texture& LoadResource(const unsigned id, const glm::ivec2 pos = {});

	static std::vector<Texture> GetVector(const std::vector<Bitmap>& bitmaps);
private:
	void LoadData(const void* data, const glm::ivec2 size, const unsigned channels);
};
