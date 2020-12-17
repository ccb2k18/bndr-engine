#pragma once

#include <pch.h>

typedef GLFWwindow* screen;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char uchar;

namespace bndr {

	void errorCallback(int code, const char* message);
	BNDR_API void clearGLErrors();
	BNDR_API void checkGLError();

	enum WindowStates {

		WINDOW_CLOSE = 0x01
	};

	class BNDR_API Window {
	
		screen window;
		// the aspect ratio
		float aspect;
		uint windowFlags = 0x0;

	public:

		Window(int x, int y, int width, int height, const char* title);
		// inline functions meant to abstract glfw functionality from the bndr namespace

		// state-checking methods

		// flush the display
		inline void flush() { glfwSwapBuffers(window); }
		// check for events
		inline void pollEvents() { glfwPollEvents(); }
		// check if the window is currently open
		inline bool isOpen() { return !glfwWindowShouldClose(window); }
		// set the close flag to true (useful for user-defined quit scenarios)
		inline void quitWindow() { windowFlags = windowFlags | WINDOW_CLOSE; }
		// get the mouse cursor position
		std::pair<float, float> getCursorPos();
		// updates flags regarding the window and whether or not the user has quit
		// returns true if the window is still open
		bool update();

		// rendering methods

		// set the clear screen color
		inline void setClearColor(float red, float green, float blue, float alpha) { glClearColor(red, green, blue, alpha); }
		// clear the screen
		inline void clear() { glClear(GL_COLOR_BUFFER_BIT); }

		// customization

		// destroy GLFW window and terminate GLFW
		~Window();

	};
}

