#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "frame.h"

constexpr int version_major = 4;
constexpr int version_minor = 5;
constexpr unsigned uniform_buffer_size_base = 0;

Frame::GLAD::GLAD(const int width, const int height) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window{ glfwCreateWindow(width, height, "D2", NULL, NULL) };
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}
Frame::Data::Data(const glm::ivec2 new_size) :
	size{ new_size },
	pos{ (new_size.x + 1) >> 1, (new_size.y + 1) >> 1 },
	pixel{ 2 / size.x, 2 / size.y }
{
}
Frame::Instant::Instant(const int width, const int height) :
	GLAD{ width, height },
	Buffer{} 
{
	Buffer::load(Data{ glm::ivec2{ width, height } }).bind(uniform_buffer_size_base);
}
const Frame::Instant& Frame::Size(const glm::ivec2 size) {
	glfwSetWindowSize(glfwGetCurrentContext(), size.x, size.y);
	frame.load(Frame::Data{ size });
	return frame;
}
glm::ivec2 Frame::Size() {
	glm::ivec2 size;
	glfwGetWindowSize(glfwGetCurrentContext(), &size.x, &size.y);
	return size;
}
/*
Buffer::Buffer() :
	FBO{ std::invoke([]() {
		unsigned int FBO;
		glGenFramebuffers(1, &FBO);
		return FBO;
		}) }
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Size(width, height);
}
Buffer& Buffer::Load(const Texture& texture) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.ID(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return *this;
}
*/
/*
glBindTexture(GL_TEXTURE_2D, texture.ID());
glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, bfr.size.x, bfr.size.y);
glBindTexture(GL_TEXTURE_2D, 0);
*/
/*
glm::ivec2 Buffer::Size(int width, int height) {
	if (glm::ivec2{ width, height } == bfr.size)
		return bfr.size;

	bfr.size = { width, height };
	glBindTexture(GL_TEXTURE_2D, bfr.frame);
	SetTexParameter();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, bfr.texture);
	SetTexParameter();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	return bfr.size;
}
unsigned Buffer::Draw() {
	glBindFramebuffer(GL_FRAMEBUFFER, bfr.FBO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindTexture(GL_TEXTURE_2D, bfr.texture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, bfr.size.x, bfr.size.y);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return bfr.texture;
}
*/