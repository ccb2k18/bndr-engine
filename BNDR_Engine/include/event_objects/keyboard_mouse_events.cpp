#include <pch.h>
#include "keyboard_mouse_events.h"

namespace bndr {


	KeyEvent::KeyEvent(const KeyEvent& keyEvent) {

		std::cout << "Copied KeyEvent!\n";
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

		std::cout << "Moved KeyEvent!\n";
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