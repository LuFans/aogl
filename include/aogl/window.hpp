#pragma once

#include <functional>

namespace aogl {

	class window {
	public:

		enum flag {
			EXIT_ON_CLOSE,
			FULLSCREEN
		};

	private:

		struct destroy_glfw_window {
			void operator()(GLFWwindow* ptr) const {
				glfwDestroyWindow(ptr);
			}
		};

		/* GLFW window handle */
		std::unique_ptr<GLFWwindow, destroy_glfw_window> handle;

	public:

		/**
		 * \brief standard constructor to create a window
		 */
		window(int width, int height, const std::string& title, flag options);

		/**
		 * \brief check if the window should still be running
		 */
		bool is_open();

		/**
		 * \brief swaps framebuffers and updates events
		 */
		void present();

	};

}
