#include "../include/aogl/stdafx.hpp"
#include "../include/aogl/window.hpp"
#include "../include/aogl/error.hpp"

namespace aogl {

	window::window(int width, int height, const std::string& title, flag options) {
		
		if (!glfwInit()) throw window_creation_exception("failed to initialize glfw!");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		handle.reset(glfwCreateWindow(width, height, title.c_str(), options == flag::FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr, nullptr));

		if (handle.get() == nullptr) throw window_creation_exception("failed to create glfw window!");

		glfwMakeContextCurrent(handle.get());

		glewExperimental = true;
		if (glewInit() != GLEW_OK) throw window_creation_exception("failed to initialize glew!");

	}

	bool window::is_open() {
		return !glfwWindowShouldClose(handle.get());
	}

	void window::present() {
		glfwPollEvents();
		glfwSwapBuffers(handle.get());
	}
}
