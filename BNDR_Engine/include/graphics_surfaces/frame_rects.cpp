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
#include "frame_rects.h"

namespace bndr {

	FrameRect::FrameRect(float x, float y, float width, float height, std::vector<RGBAData>&& colors, Texture* newTex, uint styleFlags) {

		// get the initial window size so we can convert coordinates from percents to pixels if we need to
		Vec2<float> initialSize = PixelSurface::getWindowInitialSize();

		if (styleFlags & UIRECT_X_COORD_IS_PERCENT) {

			x = (x / 100.0f) * initialSize[0];
		}
		if (styleFlags & UIRECT_Y_COORD_IS_PERCENT) {

			y = (y / 100.0f) * initialSize[1];
		}
		if (styleFlags & UIRECT_WIDTH_IS_PERCENT) {

			width = (width / 100.0f) * initialSize[0];
		}
		if (styleFlags & UIRECT_HEIGHT_IS_PERCENT) {

			height = (height / 100.0f) * initialSize[1];
		}

		// now adjust it so that it is anchored to the center if required
		if (styleFlags & UIRECT_ANCHOR_TO_CENTER) {

			x -= width / 2.0f;
			y -= height / 2.0f;
		}

		// we need to convert the y coordinate so that 0 is at the top of the screen
		y = (initialSize[1] - y) - height;
		
		// now create the textured rect to manage the low level graphics stuff for us
		texRect = new TexturedRect(x, y, width, height, std::move(colors), newTex);
	}

	FrameRect::~FrameRect() {
		delete texRect;
	}
}
