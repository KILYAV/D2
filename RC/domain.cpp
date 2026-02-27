#include <fstream>

#define STORE_LOAD_IMPLEMENTATION
#include "domain.h"

int stbi_write_png___(char const* filename, int w, int h, int comp, const void* data, int stride_in_bytes) {
	return stbi_write_png(filename, w, h, comp, data, stride_in_bytes);
}

std::tuple<glm::ivec2, glm::ivec2> SizePosBMP(const Bitmap& bmp) {
	int up = 0, down = -1, left = -1, right = 0;
	for (unsigned y = 0; y < bmp.Size().y; ++y) {
		for (unsigned x = 0; x < bmp.Size().x; ++x) {
			if (0 != bmp.Pixel({ x,y }).a) {
				down = y < down ? y : down;
				up = y > up ? y : up;
				left = x < left ? x : left;
				right = x > right ? x : right;
			}
		}
	}
	glm::ivec4 region;
	region.x = right + 1;
	region.y = up + 1;
	region.z = left;
	region.w = down;

	glm::ivec2 size;
	size.x = region.x - region.z;
	size.y = region.y - region.w;

	glm::ivec2 pos;
	pos.x = region.x - ((size.x + 1) >> 1);
	pos.y = region.y - ((size.y + 1) >> 1);

	return std::tuple{ size, pos };
}
Bitmap LoadBitmap(const std::filesystem::path& direct) {
	std::string label{ direct.string() };
	std::cout << label << "\n";

	int X, Y, nrChannels;
	//stbi_set_flip_vertically_on_load(true);
	auto data = stbi_load(label.data(), &X, &Y, &nrChannels, 0);
	Bitmap bitmap{ data, { X,Y }, { (X + 1) >> 1, (Y + 1) >> 1 } };

	auto [size, pos] { SizePosBMP(bitmap) };
	return bitmap.Size(size, pos);
}
png::Png LoadPng(const std::filesystem::path& direct) {
	std::string label{ direct.string() };
	std::cout << label << "\n";

	png::Png png;
	png.lenght =std::filesystem::file_size(direct);
	static_cast<std::unique_ptr<char[]>&>(png)
		= std::unique_ptr<char[]>(new char[png.lenght]);

	std::ifstream file{ direct, std::ios::binary };
	if (false == file.is_open())
		throw std::invalid_argument{"invalid_name_file"};

	file.read(png.get(), png.lenght);
	file.close();

	return png;
}
std::vector<Bitmap> LoadVectorBitmap(const std::filesystem::path& direct) {
	std::vector<Bitmap> bitmaps;
	for (const auto& file : std::filesystem::directory_iterator(direct)) {
		bitmaps.emplace_back(LoadBitmap(direct / file));
	}
	return bitmaps;
}
std::vector<png::Png> LoadVectorPng(const std::filesystem::path& direct) {
	std::vector<png::Png> pngs;
	for (const auto& file : std::filesystem::directory_iterator(direct)) {
		pngs.emplace_back(LoadPng(direct / file));
	}
	return pngs;
}
std::map<std::string, png::Png> LoadMapPng(const std::filesystem::path& direct) {
	std::map<std::string, png::Png> pngs;
	for (const auto& file : std::filesystem::directory_iterator(direct)) {
		pngs.emplace(file.path().filename().string(), LoadPng(direct / file));
	}
	return pngs;
}
