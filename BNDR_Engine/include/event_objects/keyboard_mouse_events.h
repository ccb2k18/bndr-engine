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
#include "events.h"

namespace bndr {

	// retrieve GLFW key codes to be packaged into the BNDR API
	enum BNDR_API keyCodes {

		KEY_0 = GLFW_KEY_0, KEY_1 = GLFW_KEY_1, KEY_2 = GLFW_KEY_2, KEY_3 = GLFW_KEY_3, KEY_4 = GLFW_KEY_4,
		KEY_5 = GLFW_KEY_5, KEY_6 = GLFW_KEY_6, KEY_7 = GLFW_KEY_7, KEY_8 = GLFW_KEY_8, KEY_9 = GLFW_KEY_9,
		KEY_Q = GLFW_KEY_Q, KEY_W = GLFW_KEY_W, KEY_E = GLFW_KEY_E, KEY_R = GLFW_KEY_R, KEY_T = GLFW_KEY_T,
		KEY_Y = GLFW_KEY_Y, KEY_U = GLFW_KEY_U, KEY_I = GLFW_KEY_I, KEY_O = GLFW_KEY_O, KEY_P = GLFW_KEY_P,
		KEY_A = GLFW_KEY_A, KEY_S = GLFW_KEY_S, KEY_D = GLFW_KEY_D, KEY_F = GLFW_KEY_F, KEY_G = GLFW_KEY_G,
		KEY_H = GLFW_KEY_H, KEY_J = GLFW_KEY_J, KEY_K = GLFW_KEY_K, KEY_L = GLFW_KEY_L, KEY_Z = GLFW_KEY_Z,
		KEY_X = GLFW_KEY_X, KEY_C = GLFW_KEY_C, KEY_V = GLFW_KEY_V, KEY_B = GLFW_KEY_B, KEY_N = GLFW_KEY_N,
		KEY_M = GLFW_KEY_M, KEY_ENTER = GLFW_KEY_ENTER, KEY_ESCAPE = GLFW_KEY_ESCAPE, KEY_DELETE = GLFW_KEY_DELETE,
		KEY_BACKSPACE = GLFW_KEY_BACKSPACE, KEY_CAPS_LOCK = GLFW_KEY_CAPS_LOCK, KEY_UP = GLFW_KEY_UP,
		KEY_DOWN = GLFW_KEY_DOWN, KEY_RIGHT = GLFW_KEY_RIGHT, KEY_LEFT = GLFW_KEY_LEFT, KEY_R_SHIFT = GLFW_KEY_RIGHT_SHIFT,
		KEY_L_SHIFT = GLFW_KEY_LEFT_SHIFT, KEY_R_ALT = GLFW_KEY_RIGHT_ALT, KEY_L_ALT = GLFW_KEY_LEFT_ALT,
		KEY_PLUS = GLFW_KEY_KP_ADD, KEY_MINUS = GLFW_KEY_KP_SUBTRACT, KEY_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
		KEY_DIVIDE = GLFW_KEY_KP_DIVIDE, KEY_R_CTRL = GLFW_KEY_RIGHT_CONTROL, KEY_L_CTRL = GLFW_KEY_LEFT_CONTROL,
		KEY_TAB = GLFW_KEY_TAB, KEY_F1 = GLFW_KEY_F1, KEY_F2 = GLFW_KEY_F2, KEY_F3 = GLFW_KEY_F3, KEY_F4 = GLFW_KEY_F4,
		KEY_F5 = GLFW_KEY_F5, KEY_F6 = GLFW_KEY_F6, KEY_F7 = GLFW_KEY_F7, KEY_F8 = GLFW_KEY_F8, KEY_F9 = GLFW_KEY_F9,
		KEY_F10 = GLFW_KEY_F10, KEY_F11 = GLFW_KEY_F11, KEY_F12 = GLFW_KEY_F12, KEY_INSERT = GLFW_KEY_INSERT,
		KEY_HOME = GLFW_KEY_HOME, KEY_END = GLFW_KEY_END, KEY_EQUAL = GLFW_KEY_EQUAL, KEY_SPACE = GLFW_KEY_SPACE
	};

	// potential states of a key
	enum BNDR_API keyStates {

		KEY_PRESS = GLFW_PRESS,
		KEY_RELEASE = GLFW_RELEASE
	};

	// retrieve GLFW mouse codes to be packaged into the BNDR API
	enum BNDR_API mouseCodes {

		MOUSE_LEFT = GLFW_MOUSE_BUTTON_LEFT,
		MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
		MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
	};

	// potential states of a mouse action
	enum BNDR_API mouseStates {

		MOUSE_PRESS = GLFW_PRESS,
		MOUSE_RELEASE = GLFW_RELEASE
	};

	// This KeyEvent class inherits from the Event class
	// It is used for keyboard events
	class BNDR_API KeyEvent : public Event {

	protected:

		uint key;

	public:

		KeyEvent(int keyState, uint keyValue) : Event(keyState), key(keyValue) {}
		KeyEvent(const KeyEvent& keyEvent);
		KeyEvent(KeyEvent&& keyEvent) noexcept;
		inline uint getKey() { return key; }
		~KeyEvent() {}
	};

	// This MouseEvent class inherits from the Event class
	// Its job is to detect mouse clicks (not scrolling, that is the job of the Scroll Event class)
	class BNDR_API MouseEvent : public Event {

	protected:

		uint button;
		float x;
		float y;

	public:

		MouseEvent(int mouseState, uint mouseButton, float mouseX, float mouseY) : Event(mouseState), button(mouseButton), x(mouseX), y(mouseY) {}
		MouseEvent() : Event(0), button((uint)0), x(0.0f), y(0.0f) {}
		MouseEvent(const MouseEvent& mouseEvent);
		MouseEvent(MouseEvent&& mouseEvent) noexcept;
		inline uint getButton() { return button; }
		inline float getX() { return x; }
		inline float getY() { return y; }
	};

	// The ScrollEvent class is created when the user uses the scroll wheel on the mouse
	// This is especially useful for UI elements that scroll
	class BNDR_API ScrollEvent : public MouseEvent {

	protected:

		// offset is one dimensional on the y-axis
		float offset;
	public:

		ScrollEvent(float mouseX, float mouseY, float yOffset) : MouseEvent(0, 0, mouseX, mouseY), offset(yOffset) {}
		ScrollEvent(const ScrollEvent& scrollEvent);
		ScrollEvent(ScrollEvent&& scrollEvent) noexcept;
		inline float getOffset() { return offset; }

	};
}


