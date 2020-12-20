/*MIT License

Copyright (c) 2020 Caleb Christopher Bender

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

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

	// define event queue for keyboard
	Queue<KeyEvent> Window::keyEvents;
	// define event queue for mouse buttons
	Queue<MouseEvent> Window::mouseEvents;
	// define queue for scroll events
	Queue<ScrollEvent> Window::scrollEvents;

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

		// callback functions
		glfwSetKeyCallback(window, Window::keyEventCallback);
		glfwSetMouseButtonCallback(window, Window::mouseEventCallback);
		glfwSetScrollCallback(window, Window::scrollEventCallback);

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
