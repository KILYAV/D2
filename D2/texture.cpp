#include <map>
#include <random>
#include <iostream>
#include <Windows.h>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"

#include "../..//utility/stb_image.h"

Texture::Texture(Texture&& other) :
	size{ other.size },
	pos{ other.pos },
	id{ other.id }
{
	other.size = { 0,0 };
	other.pos = { 0,0 };
	other.id = -1;
}
Texture::Texture(const Bitmap& bitmap) :
	size{ bitmap.Size() },
	pos{ bitmap.Pos() },
	id{ std::invoke([]() {
		unsigned texture;
		glGenTextures(1, &texture);
		return texture;
		})}
{
	LoadData(bitmap.Data(), bitmap.Size(), bitmap.SizePixel());
}
Texture::Texture(const unsigned new_id, const glm::ivec2 new_pos) :
	pos{ new_pos },
	id{ new_id }
{
	glBindTexture(GL_TEXTURE_2D, id);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &size.x);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &size.y);
	glBindTexture(GL_TEXTURE_2D, 0);
}
Texture::~Texture() {
	if (-1 != id)
		glDeleteTextures(1, &id);
}
Texture& Texture::operator= (Texture&& other) {
	this->size = other.size;
	this->pos = other.pos;
	this->id = other.id;

	other.size = { 0,0 };
	other.pos = { 0,0 };
	other.id = -1;
	return *this;
}

int GL_XXX[]{ GL_RED, GL_GREEN, GL_BLUE, GL_RGB, GL_RGBA };

void SetTexParameter() {
	constexpr float borderColor[] = { 0., 0., 0., 0. };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

Texture& Texture::Size(const glm::ivec2 new_size) {
	size = new_size;
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	return *this;
}
Texture& Texture::LoadResource(const unsigned id, const glm::ivec2 new_pos) {
	auto resource = FindResourceA(NULL, MAKEINTRESOURCEA(id), "PNG");
	auto source{ LockResource(::LoadResource(NULL, resource)) };
	int channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load_from_memory(static_cast<const unsigned char*>(source),
		SizeofResource(NULL, resource), &size.x, &size.y, &channels, 0);
	
	LoadData(data, size, channels);
	pos = new_pos;
	return *this;
}
void Texture::LoadData(const void* data, const glm::ivec2 size, const unsigned channels) {
	glBindTexture(GL_TEXTURE_2D, id);
	SetTexParameter();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_XXX[channels], size.x, size.y, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::vector<Texture> Texture::GetVector(const std::vector<Bitmap>& bitmaps) {
	std::vector<Texture> textures;
	for (const auto& bitmap : bitmaps) {
		textures.emplace_back(Texture{ bitmap });
	}
	return textures;
}
