#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "uniform.h"

Uniform::Base::Base(const unsigned size) :
	id{ std::invoke([&]() {
		unsigned uniform = 0;
		glGenBuffers(1, &uniform);
		glBindBuffer(GL_UNIFORM_BUFFER, uniform);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		return uniform;
		})}
{}
void Uniform::Base::load(const unsigned size, const void* data) const {
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData(GL_UNIFORM_BUFFER, NULL, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Uniform::Base::bind(const unsigned base) const {
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBindBufferBase(GL_UNIFORM_BUFFER, base, id);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
