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

		float red;
		float green;
		float blue;
		float alpha;
	};



	// bndr::PixelSurface
	// Description: The fundamental abstract class for all graphical elements from shapes to images to character sprites
	class BNDR_API PixelSurface {

	protected:
		// stores the vertices for the pixel surface
		VertexArray* va = nullptr;
		// every descendant of PixelSurface will be able to access the bndr::Window instance so that it is rendered correctly
		static Window* windowInstance;
		// matrices for transformations of the surface
		Mat3x3<float>* translation;
		Mat3x3<float>* rotation;
		Mat3x3<float>* scale;
		PixelSurface() : translation(Mat3x3<float>::HeapTransMat(0.0f, 0.0f)), rotation(Mat3x3<float>::HeapRotMat(0.0f)),
			scale(Mat3x3<float>::HeapScaleMat(1.0f, 1.0f)) {
		
			// ensure that a window instance has been defined
			if (windowInstance == nullptr) {

				BNDR_EXCEPTION("You have not defined the window instance for PixelSurfaces. You can do this by calling PixelSurface::setWindowInstance(&window) where window is of type bndr::Window.");
			}
		}
		// update the translation uniform in the program
		virtual void updateTranslationUniform() = 0;
		// update the rotation uniform in the program
		virtual void updateRotationUniform() = 0;
		// update the scale uniform in the program
		virtual void updateScaleUniform() = 0;

	public:

		PixelSurface(const PixelSurface&) = delete;
		PixelSurface(PixelSurface&&) = delete;
		// reset the translation matrix
		virtual void setTranslation(float xTrans, float yTrans) = 0;
		// reset the rotation matrix
		virtual void setRotation(float theta) = 0;
		// reset the scale matrix
		virtual void setScale(float xScale, float yScale) = 0;
		// change the translation matrix by a certain amount
		virtual void changeTranslationBy(float xTrans, float yTrans) = 0;
		// change the rotation matrix by a certain amount
		virtual void changeRotationBy(float theta) = 0;
		// change the scale matrix by a certain amount
		virtual void changeScaleBy(float xScale, float yScale) = 0;
		// render the surface to the screen
		virtual void render() = 0;
		// define the window instance for all pixel surfaces (if you don't do this nothing will draw and you will get an exception)
		static void setWindowInstance(Window* window) {

			windowInstance = window;
		}
		~PixelSurface();
	};

	// bndr::Shape
	// Description: base class for all shapes
	class BNDR_API Shape : public PixelSurface {

	protected:

		// program for a shape with one color
		static Program defaultPoly;
		// program for a shape with multiple colors
		static Program multiColorPoly;
		Vec3<float>* position;
		float* fillColor;
		// update the translation uniform in the program
		inline virtual void updateTranslationUniform() override { Shape::defaultPoly.setFloatUniformValue("translation", translation->getData(), bndr::MAT3X3); }
		// update the rotation uniform in the program
		inline virtual void updateRotationUniform() override { Shape::defaultPoly.setFloatUniformValue("rotation", rotation->getData(), bndr::MAT3X3); }
		// update the scale uniform in the program
		inline virtual void updateScaleUniform() override { Shape::defaultPoly.setFloatUniformValue("scale", scale->getData(), bndr::MAT3X3); }
		// update the color uniform in the program
		inline void updateColorUniform() { Shape::defaultPoly.setFloatUniformValue("color", fillColor, VEC4); }

	public:

		// bndr::Shape::Shape
		// x,y coordinate of the bottom left corner of the shape's box and then the width and height in pixels
		Shape(float x, float y, float width, float height);
		// reset the translation matrix
		virtual void setTranslation(float xTrans, float yTrans) override;
		// reset the rotation matrix in degrees
		virtual void setRotation(float theta) override;
		// reset the scale matrix
		virtual void setScale(float xScale, float yScale) override;
		// change the translation matrix by a certain amount
		virtual void changeTranslationBy(float xTrans, float yTrans) override;
		// change the rotation matrix by a certain amount in degrees
		virtual void changeRotationBy(float theta) override;
		// change the scale matrix by a certain amount
		virtual void changeScaleBy(float xScale, float yScale) override;
		// set the fill color for the shape
		void setFillColor(float red, float green, float blue, float alpha);
		// render the surface to the screen
		virtual void render() override;
		// destructor
		~Shape();

	};
}

