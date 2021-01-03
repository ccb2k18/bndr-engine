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
#include "keyboard_mouse_events.h"

namespace bndr {


	KeyEvent::KeyEvent(const KeyEvent& keyEvent) {

		state = keyEvent.state;
		key = keyEvent.key;
	}

	MouseEvent::MouseEvent(const MouseEvent& mouseEvent) {

		state = mouseEvent.state;
		button = mouseEvent.button;
		x = mouseEvent.x;
		y = mouseEvent.y;
	}

	ScrollEvent::ScrollEvent(const ScrollEvent& scrollEvent) {

		state = scrollEvent.state;
		button = scrollEvent.button;
		x = scrollEvent.x;
		y = scrollEvent.y;
		offset = scrollEvent.offset;
	}



	KeyEvent::KeyEvent(KeyEvent&& keyEvent) noexcept {

		state = keyEvent.state;
		key = keyEvent.key;
	}

	MouseEvent::MouseEvent(MouseEvent&& mouseEvent) noexcept {

		state = mouseEvent.state;
		button = mouseEvent.button;
		x = mouseEvent.x;
		y = mouseEvent.y;
	}

	ScrollEvent::ScrollEvent(ScrollEvent&& scrollEvent) noexcept {

		state = scrollEvent.state;
		button = scrollEvent.button;
		x = scrollEvent.x;
		y = scrollEvent.y;
		offset = scrollEvent.offset;
	}
}