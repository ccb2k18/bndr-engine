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
	// Description: Data structure to store rgb color values in. In instances of PixelSurface the colorBuffer will normalize
	// these values between 0.0f and 1.0f
	struct BNDR_API RGBAData {

		uint8 red;
		uint8 green;
		uint8 blue;
		uint8 alpha;
	};
	// declare colors
	// define colors
	static RGBAData RED = { 255, 0, 0, 255 };
	static RGBAData GREEN = { 0, 255, 0, 255 };
	static RGBAData BLUE = { 0, 0, 255, 255 };
	static RGBAData YELLOW = { 255, 255, 0, 255 };
	static RGBAData PURPLE = { 255, 0, 255, 255 };
	static RGBAData TORQUOISE = { 0, 255, 255, 255 };
	static RGBAData BLACK = { 0, 0, 0, 255 };
	static RGBAData WHITE = { 255, 255, 255, 255 };
	static RGBAData ORANGE = { 255, 128, 0, 255 };
	static RGBAData BROWN = { 64, 32, 0, 255 };


	// bndr::PixelSurface
	// Description: The fundamental abstract class for all graphical elements from shapes to images to character sprites
	class BNDR_API PixelSurface {

	protected:
		Program* program;
		// stores the vertices for the pixel surface
		VertexArray* va;
		// color buffer that stores colors for object
		float* colorBuffer;
		// every descendant of PixelSurface will be able to access the bndr::Window instance so that it is rendered correctly
		static Window* windowInstance;
		// matrices for transformations of the surface
		Mat3x3<float>* translation;
		Mat3x3<float>* rotation;
		Mat3x3<float>* scale;
		// this constructor will be called for every child of PixelSurface
		// by default the color buffer has a size of 4 for 1 rgba entry
		// this can be changed by calling the constructor below and passing in a color buffer size
		PixelSurface() : translation(Mat3x3<float>::HeapTransMat(0.0f, 0.0f)), rotation(Mat3x3<float>::HeapRotMat(0.0f)),
			scale(Mat3x3<float>::HeapScaleMat(1.0f, 1.0f)), program(nullptr), colorBuffer(nullptr), va(nullptr) {
		
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
		// update the color data in the shader program
		virtual void updateColorData() = 0;
		// load the color buffer into memory
		inline void loadColorBuffer(int length) { colorBuffer = new float[length]; }

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
	// Description: base class for all shapes (rectangles, triangles, and ellipses/circles)
	class BNDR_API Shape : public PixelSurface {

	protected:

		// update the translation uniform in the program
		inline virtual void updateTranslationUniform() override { program->setFloatUniformValue("translation", translation->getData(), bndr::MAT3X3); }
		// update the rotation uniform in the program
		inline virtual void updateRotationUniform() override { program->setFloatUniformValue("rotation", rotation->getData(), bndr::MAT3X3); }
		// update the scale uniform in the program
		inline virtual void updateScaleUniform() override { program->setFloatUniformValue("scale", scale->getData(), bndr::MAT3X3); }
		// update the color uniform in the program
		inline virtual void updateColorData() override { program->setFloatUniformValue("color", colorBuffer, VEC4); }
		// default constructor
		Shape(Program* prog) : PixelSurface() {
		
			program = prog;
			// update the transformation matrices
			updateTranslationUniform();
			updateRotationUniform();
			updateScaleUniform();
		}

	public:

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
		// set the fill color for the shape (converts a rgba color to floats)
		virtual void setFillColor(const RGBAData& data);
		// render the surface to the screen
		virtual void render() override;
		// get the program id
		inline uint getProgramID() { return program->getID(); }

	};

	// treated as an interface for BasicRect, ColorfulRect, and TexturedRect
	class BNDR_API GraphicsRect {

	protected:
		// 0: x, 1: y
		Vec2<float>* pos;
		// 0: width, 1: height
		Vec2<float>* size;
		// construct the GraphicsRect
		GraphicsRect(float x, float y, float width, float height) : pos(new Vec2<float>(x, y)), size(new Vec2<float>(width, height)) {}
		~GraphicsRect() { BNDR_MESSAGE("Deleted instance of GraphicsRect!"); delete pos; delete size; }
	};


	// bndr::BasicRect
	// Description: This is a basic rectangle that you may specify a single color for
	class BNDR_API BasicRect : public Shape, public GraphicsRect {

	public:

		BasicRect(float x, float y, float width, float height, const RGBAData& color = bndr::WHITE);
	};


	// bndr::BasicTriangle
	// Description: This is a basic triangle that you may specify a single color for
	class BNDR_API BasicTriangle : public Shape {

	public:

		BasicTriangle(const Vec2<float>& coord1, const Vec2<float>& coord2, const Vec2<float>& coord3, const RGBAData& color);

	};

	// bndr::ColorfulRect
	// Description: This is a rectangle that can have a color for every vertex
	class BNDR_API ColorfulRect : public Shape, public GraphicsRect {

	protected:

		// update the color data in the vertex buffer of va
		virtual void updateColorData() override;
	public:

		ColorfulRect(float x, float y, float width, float height, std::vector<RGBAData>&& colors = { bndr::WHITE });
		// set a single fill color for the rect (if you only want a solid color then you should probably just use BasicRect)
		// (converts a rgba color to floats)
		virtual void setFillColor(const RGBAData& data) override;
		// set a color for every corner of the rectangle
		void setFillColors(const RGBAData& bottomLeft, const RGBAData& topLeft, const RGBAData& topRight, const RGBAData& bottomRight);
	};
}

