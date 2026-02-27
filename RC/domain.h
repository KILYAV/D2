#pragma once
#include <map>
#include <vector>
#include <filesystem>

#include "../../utility/bitmap.h"
#include "../../utility/store_load.h"

std::tuple<glm::ivec2, glm::ivec2> SizePosBMP(const Bitmap& bmp);

Bitmap LoadBitmap(const std::filesystem::path& direct);
png::Png LoadPng(const std::filesystem::path& direct);

std::vector<Bitmap> LoadVectorBitmap(const std::filesystem::path& direct);
std::vector<png::Png> LoadVectorPng(const std::filesystem::path& direct);

std::map<std::string, Bitmap> LoadRace(const std::filesystem::path& direct);
std::map<std::string, png::Png> LoadMapPng(const std::filesystem::path& direct);
std::map<std::string, std::vector<char>> LoadInterf(const std::filesystem::path& direct);
std::map<std::string, std::vector<char>> LoadBattle(const std::filesystem::path& direct);
std::map<std::string, std::map<std::string, std::string>> LoadShade(const std::filesystem::path& path);
std::map<std::string, std::map<std::string, std::map<std::string, std::vector<Bitmap>>>>
	LoadBatUnits(const std::filesystem::path& direct);

void Store(const std::string label, const auto& map) {
	using namespace store;
	std::ofstream file{ label, std::ios::binary };
	file << map;
	file.close();
}

int stbi_write_png___(char const* filename, int w, int h, int comp, const void* data, int stride_in_bytes);