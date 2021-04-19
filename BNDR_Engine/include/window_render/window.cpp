/*MIT License

Copyright (c) 2021 Caleb Christopher Bender

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

	// define event queue for keyboard
	Queue<KeyEvent> Window::keyEvents;
	// define event queue for mouse buttons
	Queue<MouseEvent> Window::mouseEvents;
	// define queue for scroll events
	Queue<ScrollEvent> Window::scrollEvents;

	Window::Window(int x, int y, int width, int height, const char* title, uint flags) {

		// greet the client
		BNDR_MESSAGE("Hello from BNDR Engine!");

		// initialize glfw
		if (!glfwInit()) {

			BNDR_EXCEPTION("Failed to initialize GLFW");

		}
		glfwSetErrorCallback(errorCallback);

		// window hints/flags for the window
		// the window will always maintain its aspect ratio
		windowFlags = flags | bndr::MAINTAIN_ASPECT_RATIO;

		// single buffering for max fps
		glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);

		// specify version of OpenGL
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		// use the OpenGL core profile
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		if (windowFlags & NOT_RESIZEABLE) {

			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		}

		// create window

		if (windowFlags & FULLSCREEN_ONLY) {

			// if we only want to have the window be fullscreen
			// get the primary monitor and its video mode
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

			// now add some more hints
			glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

			window = glfwCreateWindow(videoMode->width, videoMode->height, title, monitor, NULL);
			// for obvious reasons the window is not going to be resizable
			windowFlags = windowFlags | NOT_RESIZEABLE;
		}
		else {

			// we don't care about the refresh rate being explicit
			glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
			window = glfwCreateWindow(width, height, title, NULL, NULL);
		}
		// make sure it created successfully
		if (window == NULL) {

			glfwTerminate();
			BNDR_EXCEPTION("Failed to create GLFW window");
		}

		// more window rules

		// position of the window
		glfwSetWindowPos(window, x, y);
		// if the window is resizable
		if (!(windowFlags & NOT_RESIZEABLE)) {

			// resizing constraints are half the size specified up to fullscreen
			glfwSetWindowSizeLimits(window, width / 2, height / 2, GLFW_DONT_CARE, GLFW_DONT_CARE);
		}
		// maintain aspect ratio
		if (windowFlags & MAINTAIN_ASPECT_RATIO) {

			glfwSetWindowAspectRatio(window, width, height);
		}
		aspect = (float)width / (float)height;
		// make keys sticky if the flag is set
		if (windowFlags & STICKY_KEYS) {

			glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
		}
		// make mouse buttons sticky if the flag is set
		if (windowFlags & STICKY_MOUSE) {

			glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
		}
		// make context for window
		glfwMakeContextCurrent(window);

		// callback functions
		glfwSetKeyCallback(window, Window::keyEventCallback);
		glfwSetMouseButtonCallback(window, Window::mouseEventCallback);
		glfwSetScrollCallback(window, Window::scrollEventCallback);
		glfwSetFramebufferSizeCallback(window, Window::windowResizeCallback);
		//glfwSetWindowSizeCallback(window, Window::windowResizeCallback);

		// create opengl context
		if (glewInit() != GLEW_OK) {

			glfwDestroyWindow(window);
			glfwTerminate();
			BNDR_EXCEPTION("OpenGL context failed to create");
		}

		// set viewport size
		glViewport(0, 0, width, height);

		// enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// equation by default is add the two alpha values
		glBlendEquation(GL_FUNC_ADD);
	}

	void Window::setIcon(const char* bitMapFile) {

		GLFWimage imageData = Window::loadIcon(bitMapFile);

		glfwSetWindowIcon(window, 1, &imageData);

		// delete the image from memory
		delete[] imageData.pixels;
	}

	std::pair<float, float> Window::getCursorPos() {
		
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return std::make_pair((float)x, (float)y);
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
