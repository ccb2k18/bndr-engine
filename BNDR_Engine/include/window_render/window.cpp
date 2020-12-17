#include <pch.h>
#include "window.h"

namespace bndr {

	void errorCallback(int code, const char* message) {
		
		std::cout << "The Process exited with code " << code << " and message: " << message << '\n';
		throw std::runtime_error("error");
	}

	void clearGLErrors() {

		while (glGetError()) {

		}
	}
	void checkGLError() {

		uint errorCode = glGetError();
		if (errorCode != GL_NO_ERROR) {

			std::cout << "OpenGL reported error code " << errorCode << '\n';
			exit(-1);
		}
	}

	Window::Window(int x, int y, int width, int height, const char* title) {

		// greet the client
		BNDR_MESSAGE("Hello from BNDR Engine!");

		// initialize glfw
		if (!glfwInit()) {

			BNDR_EXCEPTION("Failed to initialize GLFW");

		}
		glfwSetErrorCallback(errorCallback);

		// window hints for the window

		// disable double buffering so we have unlimited fps
		// glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);

		// create window
		window = glfwCreateWindow(width, height, title, NULL, NULL);
		// make sure it created successfully
		if (window == NULL) {

			glfwTerminate();
			BNDR_EXCEPTION("Failed to create GLFW window");
		}

		// more window rules

		// position of the window
		glfwSetWindowPos(window, x, y);
		// resizing constraints are half the size specified up to the size specified
		glfwSetWindowSizeLimits(window, width / 2, height / 2, width, height);
		// maintain aspect ratio
		glfwSetWindowAspectRatio(window, width, height);
		aspect = (float)width / (float)height;
		// make keys sticky and mouse sticky
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
		glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
		// make context for window
		glfwMakeContextCurrent(window);

		// create opengl context
		if (glewInit() != GLEW_OK) {

			glfwDestroyWindow(window);
			glfwTerminate();
			BNDR_EXCEPTION("OpenGL context failed to create");
		}
	}

	std::pair<float, float> Window::getCursorPos() {
		
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	bool Window::update() {

		pollEvents();

		if (!isOpen() || (windowFlags & WINDOW_CLOSE)) {
		
			return false;
		}
		return true;
	}

	Window::~Window() {

		// destruct window
		glfwDestroyWindow(window);
		glfwTerminate();
	}

}
