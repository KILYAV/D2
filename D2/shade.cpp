#include <fstream>
#include <numeric>
#include <iostream>
#include <filesystem>
#include <Windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility/store_load.h>

#include "shade.h"
#include "resource.h"

#include "../RC/domain.h"

unsigned CompileShade(const std::string& source, const unsigned GL_XXXX_SHADER);
unsigned CompileProgram(const unsigned vert, const unsigned geom, const unsigned frag);
Shade::Resource::Resource() {
	Store("resource/shade", LoadShade("../RC/shade"));

	std::map<std::string, std::map<std::string, std::string>> source;
	load::Resource{ std::invoke([]() {
		auto find{ FindResourceA(NULL, MAKEINTRESOURCEA(IDB_SHADE), "data") };
		auto load{ LoadResource(NULL, find) };
		return LockResource(load);
		}) }
	.LoadResource(source);

	for (const auto& [label, shades] : source) {
		auto vert{ CompileShade(shades.at(".vert"), GL_VERTEX_SHADER) };
		auto frag{ CompileShade(shades.at(".frag"), GL_FRAGMENT_SHADER) };
		auto geom{ shades.contains(".geom") ? CompileShade(shades.at(".geom"), GL_GEOMETRY_SHADER): 0 };
		this->shades.at(label) = CompileProgram(vert, geom, frag);
	}
}
Shade::Base::Base(std::string_view label) :
	prog{ resource.shades.at(label) },
	VAO{ std::invoke([]() {
		unsigned VAO;
		glGenVertexArrays(1, &VAO);
		return VAO;
		}) },
	VBO{ std::invoke([]() {
		unsigned VBO;
		glGenBuffers(1, &VBO);
		return VBO;
		}) }
{}
void Shade::Base::Attribute(const unsigned size) const {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
}
void Shade::Base::Attribute(const unsigned index, const unsigned size) const {
	glVertexAttribPointer(index, size / sizeof(float), GL_FLOAT, GL_FALSE, size, NULL);
	glEnableVertexAttribArray(index);
}
void Shade::Base::Attribute() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Shade::Base::Run() {
	glUseProgram(prog);
	glBindVertexArray(VAO);

	//glm::ivec4 data{1,1,-1,-1};
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::ivec4), &data);

	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}
void Shade::Base::Texture(const unsigned index, const unsigned id) const {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, id);
}

unsigned CompileShade(const std::string& source, const unsigned GL_XXXX_SHADER) {
	unsigned shade = glCreateShader(GL_XXXX_SHADER);
	auto ptr = source.data();
	int size = source.size();
	glShaderSource(shade, 1, &ptr, &size);
	glCompileShader(shade);
#ifdef _DEBUG
	int success;
	glGetShaderiv(shade, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shade, 512, NULL, infoLog);
		std::cout << infoLog;
	}
#endif // DEBUG
	return shade;
};
unsigned CompileProgram(const unsigned vert, const unsigned geom, const unsigned frag) {
	auto shade{ glCreateProgram() };
	glAttachShader(shade, vert);
	if (geom)
		glAttachShader(shade, geom);
	glAttachShader(shade, frag);
	glLinkProgram(shade);
#ifdef _DEBUG
	GLint success;
	glGetProgramiv(shade, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(shade, 512, NULL, infoLog);
		std::cout << infoLog;
	}
#endif // DEBUG
	glDeleteShader(vert);
	if (geom)
		glDeleteShader(geom);
	glDeleteShader(frag);
	return shade;
};