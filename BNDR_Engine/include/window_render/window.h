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

#pragma once

#include <pch.h>
#include "../event_objects/keyboard_mouse_events.h"

// typedef to hide glfw functionality in the BNDR API
typedef GLFWwindow* screen;

namespace bndr {

	void errorCallback(int code, const char* message);

	// bndr::windowFlags
	// Description: Flags to use so that the bndr::Window can be customized to the programmer's content
	enum windowFlags {

		WINDOW_CLOSE = 0x01,
		FULLSCREEN_RESIZABLE = 0x0,
		MAINTAIN_ASPECT_RATIO = 0x04,
		STICKY_KEYS = 0x08,
		STICKY_MOUSE = 0x10,
		NOT_RESIZEABLE = 0x20,
		FULLSCREEN_ONLY = 0x40

	};

	// bndr::Window
	// Description: This class is meant to be a customizeable window that allows for keyboard and mouse event queue
	// traversals. There are a few rendering methods, but the BNDR rendering API is meant to be used for more complicated
	// operations.
	class BNDR_API Window {
	
		screen window;
		// the aspect ratio
		float aspect;
		// various flags for customizing the window
		uint windowFlags = 0x0;
		// event queue for keyboard
		static Queue<KeyEvent> keyEvents;
		// event queue for mouse buttons
		static Queue<MouseEvent> mouseEvents;
		// event queue for scroll events
		static Queue<ScrollEvent> scrollEvents;
		// vao so OpenGL doesn't complain
		uint vao;

	public:

		// bndr::Window::Window
		// Arguments:
		//        x = The position of the top left corner x coord of the window on the monitor (irrelevant for FULLSCREEN_ONLY)
		//        y = The position of the top left corner y coord of the window on the monitor (irrelevant for FULLSCREEN_ONLY)
		//        width = The width of the window in pixels
		//        height = The height of the window in pixels
		//        title = A c-string that will specify the title of the window
		//        flags = Various flags to customize the window
		// Description: Constructs a BNDR window based on the programmer specifications and creates an OpenGL context for that window
		Window(int x, int y, int width, int height, const char* title, uint flags);
		// inline functions meant to abstract glfw functionality from the bndr namespace

		// state-checking methods

		// flush the display
		inline void flush() { glfwSwapBuffers(window); }
		// check for events
		inline void pollEvents() { glfwPollEvents(); }
		// check if the window is currently open
		inline bool isOpen() { return !glfwWindowShouldClose(window); }
		// set the close flag to true (useful for custom quit scenarios)
		inline void quitWindow() { windowFlags = windowFlags | WINDOW_CLOSE; }
		// get the width and height of the window
		inline std::pair<int, int> getSize() { int w, h; glfwGetWindowSize(window, &w, &h); return std::make_pair(w, h); }
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
		// sets the icon of the window
		// the method expects a 24-bit bitmap image in Blue-Green-Red format
		void setIcon(const char* bitMapFile);

		// event queues and event callbacks

		// returns a reference to the key event queue
		static inline Queue<KeyEvent>& getKeyEvents() { return keyEvents; }
		// returns a reference to the mouse event queue
		static inline Queue<MouseEvent>& getMouseEvents() { return mouseEvents; }
		// returns a reference to the scroll event queue
		static inline Queue<ScrollEvent>& getScrollEvents() { return scrollEvents; }
		// bndr::Window::~Window
		// Description: This destructor calls glfw functions to clean up the window and memory associated with it
		~Window();

	private:

		// load an icon image
		static GLFWimage loadIcon(const char* bitMapFile) {

			std::ifstream bitMapBuffer(bitMapFile, std::ios::in);
			// check if the file opened successfully
			if (!bitMapBuffer.is_open()) {

				std::string message = "Failed to open bit map file " + std::string("'") + std::string(bitMapFile) + std::string("'");
				BNDR_EXCEPTION(message.c_str());
			}
			// read the header of the bitmap
			uchar header[54];
			bitMapBuffer.read(static_cast<char*>(static_cast<void*>(&header[0])), 54);

			// check if the bitmap file starts with B and M
			if (header[0] != (uchar)'B' || header[1] != (uchar)'M') {

				std::string message = "The bitmap file " + std::string("'") + std::string(bitMapFile) + std::string("'") + " has an invalid file format";
				BNDR_EXCEPTION(message.c_str());
			}
			// get the image size, width, and height
			//int size = *(int*)&header[0x22];
			int width = *(int*)&header[0x12];
			int height = *(int*)&header[0x16];

			// if the size is missing then fill it in
			int size = width * height * 3;
			// if either the width or height is 0 raise an error
			if (width == 0 || height == 0) { BNDR_EXCEPTION("Cannot read from a bitmap with a dimension of 0 pixels"); }

			// create buffer for the image data
			uchar* imageData = new uchar[size];
			// now read the remaining image data
			bitMapBuffer.read(static_cast<char*>(static_cast<void*>(imageData)), size);
			bitMapBuffer.close();

			// convert the image to rgba format
			int rgbaSize = width * height * 4;
			uchar* rgbaImage = new uchar[rgbaSize];
			int j = 0;
			for (int i = 0; i < size; i+=3) {

				rgbaImage[j] = *(uchar*)&imageData[i + 2];
				rgbaImage[j + 1] = *(uchar*)&imageData[i + 1];
				rgbaImage[j + 2] = *(uchar*)&imageData[i];
				rgbaImage[j + 3] = (uchar)255;
				j += 4;
			}
			// delete the old image data
			delete[] imageData;

			// reverse the image data so it is right-side up
			j = rgbaSize-4;
			int i = 0;
			while (true) {

				// red
				uchar temp = rgbaImage[i];
				rgbaImage[i] = rgbaImage[j];
				rgbaImage[j] = temp;

				// green
				temp = rgbaImage[i + 1];
				rgbaImage[i + 1] = rgbaImage[j + 1];
				rgbaImage[j + 1] = temp;

				// blue
				temp = rgbaImage[i + 2];
				rgbaImage[i + 2] = rgbaImage[j + 2];
				rgbaImage[j + 2] = temp;

				// alpha
				temp = rgbaImage[i + 3];
				rgbaImage[i + 3] = rgbaImage[j + 3];
				rgbaImage[j + 3] = temp;


				i+=4;
				j-=4;

				if (i == j || abs(i-j) == 4) { break; }
			}

			return { width, height, rgbaImage };
			
		}

		// key callback
		static void keyEventCallback(screen window, int key, int scancode, int action, int mods) {

			uint theKey = (uint)key;
			Window::keyEvents.enqueue(std::move(KeyEvent(action, theKey)));
		}
		// mouse callback
		static void mouseEventCallback(screen window, int button, int action, int mods) {

			uint theButton = (uint)button;
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			Window::mouseEvents.enqueue(std::move(MouseEvent(action, theButton, (float)x, (float)y)));
		}
		// scroll callback
		static void scrollEventCallback(screen window, double xOff, double yOff) {

			double x, y;
			glfwGetCursorPos(window, &x, &y);
			Window::scrollEvents.enqueue(std::move(ScrollEvent((float)x, (float)y, (float)yOff)));
		}
		// resize callback
		static void windowResizeCallback(screen window, int width, int height) {

			// respecify the state of the viewport
			glViewport(0, 0, width, height);
		}
		// customization

	};
}

