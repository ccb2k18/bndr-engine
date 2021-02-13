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
#include "../window_render/window.h"
#include "../window_render/gpu_objects/textures.h"
#include "../window_render/gpu_objects/shaders.h"

namespace bndr {

	// bndr::RGBData
	// Description: Data structure to store rgb color values in
	struct BNDR_API RGBAData {

		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alpha;
	};



	// bndr::PixelSurface
	// Description: The fundamental abstract class for all graphical elements from shapes to images to character sprites
	class BNDR_API PixelSurface {

	protected:
		// stores the vertices for the pixel surface
		VertexArray* va;
		// every descendant of PixelSurface will be able to access the bndr::Window instance so that it is rendered correctly
		static Window* windowInstance;
		// matrices for transformations of the surface
		Mat3x3<float>* translation;
		Mat3x3<float>* rotation;
		Mat3x3<float>* scale;

	public:

		PixelSurface() : translation(Mat3x3<float>::HeapTransMat(0.0f, 0.0f)), rotation(Mat3x3<float>::HeapRotMat(0.0f)),
		scale(Mat3x3<float>::HeapScaleMat(1.0f, 1.0f)) {}
		PixelSurface(const PixelSurface&) = delete;
		//PixelSurface(PixelSurface&&) = delete;
		PixelSurface(Mat3x3<float>* trans, Mat3x3<float>* rot, Mat3x3<float> sca);
		virtual void setTranslation(float xTrans, float yTrans) = 0;
		virtual void setRotation(float theta) = 0;
		virtual void setScale(float xScale, float yScale) = 0;
		virtual void changeTranslationBy(float xTrans, float yTrans) = 0;
		virtual void changeRotationBy(float theta) = 0;
		virtual void changeScaleBy(float xScale, float yScale) = 0;
		virtual void render() = 0;
		static void setWindowInstance(Window* window) {

			windowInstance = window;
		}
		~PixelSurface();
	};

	// bndr::Shape
	// Description: abstract class for all shapes
	class BNDR_API Shape : public PixelSurface {

	protected:

		static Program* defaultPoly;
		Vec3<float>* position;

	public:

	};
}

