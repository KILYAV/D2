#include <Windows.h>

#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"

#include "../utility/store_load.h"

#include "label.h"
#include "shade.h"
#include "battle.h"

Battle::Resource::Resource() {
	const auto resource{ FindResource(NULL, MAKEINTRESOURCE(IDB_BATTLE), L"DATA") };
	const auto source{ LockResource(LoadResource(NULL, resource)) };

	std::map<std::string_view, std::vector<char>> map_vector_char;
	load::Resource{ source }
		.LoadResource(map_vector_char, load::Resource::Label<battle::land, battle::mark>);
	load::Resource{ (void*)map_vector_char.at(battle::land).data() }
		.LoadResource(bitmaps_land,
			load::Resource::Label<race::imperial, race::dwarf, race::heretic, race::undead>);
	load::Resource{ (void*)map_vector_char.at(battle::mark).data() }
		.LoadResource(bitmaps_mark, load::Resource::Label<battle::mark>);
}
Battle::Battle(std::string_view race) :
	texture_land{ std::invoke([&]() {
		const auto& bitmaps{ resource.bitmaps_land.at(race) };
		const unsigned rand = std::rand() * bitmaps.size() / RAND_MAX;

		Texture texture_land{ bitmaps[rand] };
		texture_land.Pos({ 0, (Frame::Size().y - texture_land.Size().y + 1) >> 1 });

		return texture_land;
		})},
	textures_mark{ std::invoke([]() {
		std::vector<Texture> mark;
		mark = Texture::GetVector(resource.bitmaps_mark);
		return mark;
		})},
	offsets{ std::invoke([]() {
		const auto size{ Uniform::frame.Data().size };
		const int X = size.x / 10;
		const int Y = size.y / 10;

		expr::map<group::column::list,
			expr::map<group::row::list, glm::ivec2>> offset;

		auto& front{ offset.at(group::column::front) };
		front.at(group::row::left::left) = { -X * 1, Y * 3 };
		front.at(group::row::left::center) = { -X * 1, Y * 1 };
		front.at(group::row::right::center) = { -X * 1, -Y * 1 };
		front.at(group::row::right::right) = { -X * 1, -Y * 3 };

		auto& back{ offset.at(group::column::back) };
		back.at(group::row::left::left) = { -X * 3, Y * 3 };
		back.at(group::row::left::center) = { -X * 3, Y * 1 };
		back.at(group::row::right::center) = { -X * 3, -Y * 1 };
		back.at(group::row::right::right) = { -X * 3, -Y * 3 };

		return offset;
		})}
{
	alive.reserve(count_max_unit);
	dead.reserve(count_max_unit);

	auto& data{ Uniform::battle.Data() };
	data.land_region = texture_land.Region();
	data.mark_region = textures_mark.front().Region();
	Uniform::battle.Load();
}
Battle& Battle::AddUnit(std::unique_ptr<Unit> unit) {
	alive.push_back(std::move(unit));
	return *this;
}
Battle& Battle::Run() {
	auto window{ glfwGetCurrentContext() };
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		static_cast<Battle*>(glfwGetWindowUserPointer(window))->CursorPosCallBack(xpos, ypos);
		});
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
		static_cast<Battle*>(glfwGetWindowUserPointer(window))->MouseCallBack(button, action, mods);
		});

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_land.ID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures_mark.front().ID());

	/*
	std::array<Unit::data_t, count_max_unit> vertex;
	for (int index = 0, count = alive.size(); index < count; ++index) {
		const auto& [order, ID] { alive[index]->OrderIDSizePosFrame(&vertex[index]) };
		vertex[index].offset = offsets.at(order.column).at(order.row);

		glActiveTexture(GL_TEXTURE2 + index);
		glBindTexture(GL_TEXTURE_2D, ID);
	}
	*/
	Shade::sprite.Run();
	glfwSwapBuffers(glfwGetCurrentContext());

	//BindFilmUnits();

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return *this;
}

void Battle::CursorPosCallBack(double xpos, double ypos) {
	return;
}
void Battle::MouseCallBack(int button, int action, int mods) {
	return;
}

//stbi_write_png("temp.png", bitmap.Size().x, bitmap.Size().y, 4, bitmap.Data(), bitmap.SizeLine());
