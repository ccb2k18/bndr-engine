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

#pragma once
#include <pch.h>
#include "include/graphics_surfaces/primitives/graphical_bedrocks.h";

namespace bndr {

	// style flags to apply to the ui rectangle
	// ANCHOR_TO_CENTER: sets the x and y coordinate to be at the center of rectangle as opposed to the top left corner
	// X_COORD_IS_PERCENT: if flag is set the x coordinate is treated as a percentage value where 100.0f would be 100% of the screen width
	// Y_COORD_IS_PERCENT: if flag is set the y coordinate is treated as a percentage value where 100.0f would be 100% of the screen height
	// WIDTH_IS_PERCENT: if flag is set the width is treated as a percentage value where 100.0f would be 100% of the screen width
	// HEIGHT_IS_PERCENT: if flag is set the height is treated as a percentage value where 100.0f would be 100% of the screen height
	enum BNDR_API FrameRectStyles {

		UIRECT_ANCHOR_TO_CENTER = 1,
		UIRECT_X_COORD_IS_PERCENT = 2,
		UIRECT_Y_COORD_IS_PERCENT = 4,
		UIRECT_WIDTH_IS_PERCENT = 8,
		UIRECT_HEIGHT_IS_PERCENT = 16
	};


	// an optimal class for a graphics component with only a single animation frame
	class BNDR_API FrameRect {

		// this TexturedRect will manage all the lower level drawing stuff for us
		TexturedRect* texRect = nullptr;
	public:
		FrameRect(float x, float y, float width, float height, std::vector<RGBAData>&& colors = { bndr::WHITE }, Texture* newTex = nullptr, uint styleFlags = 0);
		inline void render() { texRect->render(); }
		~FrameRect();
	};
}

