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

	// bndr::VAContainer
	// a structure to contain the vertex array arguments to be passed down in the PolySurface class hierarchy
	struct BNDR_API VAContainer {

		uint drawingMode;
		std::vector<float> vertexData;
		int dataBlockBytes;
		uint vertexBufferFlags;
		std::vector<uint> indexData;
	};


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
	static const RGBAData RED = { 255, 0, 0, 255 };
	static const RGBAData GREEN = { 0, 255, 0, 255 };
	static const RGBAData BLUE = { 0, 0, 255, 255 };
	static const RGBAData YELLOW = { 255, 255, 0, 255 };
	static const RGBAData PURPLE = { 255, 0, 255, 255 };
	static const RGBAData TORQUOISE = { 0, 255, 255, 255 };
	static const RGBAData BLACK = { 0, 0, 0, 255 };
	static const RGBAData WHITE = { 255, 255, 255, 255 };
	static const RGBAData ORANGE = { 255, 128, 0, 255 };
	static const RGBAData BROWN = { 64, 32, 0, 255 };

	// bndr::PixelSurface
	// Description: The fundamental abstract class for all graphical elements from PolySurfaces to images to character sprites
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
		Vec3<float>* translation;
		Vec3<float>* rotation;
		Vec3<float>* scale;
		// this constructor will be called for every child of PixelSurface
		// by default the color buffer has a size of 4 for 1 rgba entry
		// this can be changed by calling the constructor below and passing in a color buffer size
		PixelSurface() : translation(new Vec3<float>()), rotation(new Vec3<float>()),
			scale(new Vec3<float>(1.0f, 1.0f, 0.0f)), program(nullptr), colorBuffer(nullptr), va(nullptr) {
		
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

	// bndr::PolySurface
	// Description: base class for all PolySurfaces (rectangles, triangles, and ellipses/circles)
	class BNDR_API PolySurface : public PixelSurface {

	protected:

		// update the translation uniform in the program
		inline virtual void updateTranslationUniform() override { program->setFloatUniformValue("translation", translation->getData(), VEC3); }
		// update the rotation uniform in the program
		inline virtual void updateRotationUniform() override { program->setFloatUniformValue("rotation", rotation->getData(), VEC3); }
		// update the scale uniform in the program
		inline virtual void updateScaleUniform() override { program->setFloatUniformValue("scale", scale->getData(), VEC3); }
		// update the color uniform in the program
		inline virtual void updateColorData() override { program->setFloatUniformValue("color", colorBuffer, VEC4); }
		// default constructor
		PolySurface() : PixelSurface() {}
		// colorBufferSize and numTexes are used by children of PolySurface
		void init(int colorBufferSize, int numTexes) {

			// generate the program for the polysurface
			program = generateShaderProgram(numTexes);
			// load the color buffer with the correct number of colors
			loadColorBuffer(colorBufferSize);
			// load the vertex array data
			va = generateVertexArray();
			// update the transformation matrices
			updateTranslationUniform();
			updateRotationUniform();
			updateScaleUniform();
		}
		// virtual function that will be overridden by children so that the appropriate program is created
		inline virtual Program* generateShaderProgram(int numTexes = 0) { return Program::defaultPolygonProgram(); }
		// virtual function that specifies the vertex array data for the surface
		virtual VertexArray* generateVertexArray() {

			return nullptr;
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
		// set the fill color for the PolySurface (converts a rgba color to floats)
		virtual void setFillColor(const RGBAData& data);
		// render the surface to the screen
		virtual void render() override;
		// get the program id
		inline uint getProgramID() { return program->getID(); }

	};

	// parent class of GraphicsRect and GraphicsTriangle
	class BNDR_API GraphicsEntity {

	protected:

		Vec2<float>* center;
		GraphicsEntity() : center(new Vec2<float>()) {}
	public:
		// update the center of rotation of the entity
		inline void updateCenterUniform(Program* program) const { program->setFloatUniformValue("center", center->getData(), VEC2); }
		// update the center of rotation of the entity with a custom point
		inline void updateCenterUniform(Program* program, const Vec2<float>& point) const { program->setFloatUniformValue("center", point.getData(), VEC2); }
		~GraphicsEntity() { delete center; }
	};


	// treated as an interface for BasicRect, ColorfulRect, and TexturedRect
	class BNDR_API GraphicsRect : public GraphicsEntity {

	protected:
		// 0: x, 1: y
		Vec2<float>* pos;
		// 0: width, 1: height
		Vec2<float>* size;
		// construct the GraphicsRect
		GraphicsRect(float x, float y, float width, float height) : GraphicsEntity(), pos(new Vec2<float>(x, y)),
			size(new Vec2<float>(width, height)) {
		
			(*center)[0] = (x + x + width) / 2.0f;
			(*center)[1] = (y + y + height) / 2.0f;
		}
	public:
		~GraphicsRect() { delete pos; delete size; }
	};

	// treated as an interface for BasicTriangle, ColorfulTriangle, and TexturedTriangle
	class BNDR_API GraphicsTriangle : public GraphicsEntity {

	protected:

		Vec2<float>* vertex1;
		Vec2<float>* vertex2;
		Vec2<float>* vertex3;

		GraphicsTriangle(Vec2<float>&& v1, Vec2<float>&& v2, Vec2<float>&& v3) : GraphicsEntity(),
			vertex1(new Vec2<float>(v1)), vertex2(new Vec2<float>(v2)), vertex3(new Vec2<float>(v3)) {
		
			(*center)[0] = ((*vertex1)[0] + (*vertex2)[0] + (*vertex3)[0]) / 3.0f;
			(*center)[1] = ((*vertex1)[1] + (*vertex2)[1] + (*vertex3)[1]) / 3.0f;
		}
	public:
		~GraphicsTriangle() { delete vertex1; delete vertex2; delete vertex3; }
	};


	// bndr::BasicRect
	// Description: This is a basic rectangle that you may specify a single color for
	class BNDR_API BasicRect : public GraphicsRect, public PolySurface {

	protected:

		virtual VertexArray* generateVertexArray() override;
	public:

		BasicRect(float x, float y, float width, float height, const RGBAData& color = bndr::WHITE, int colorBufferSize = 4, bool super = false);
		// the rotation is about the PolySurface's center by default
		// you only need to call this if you changed the center of rotation to a different point
		// using setRotationAboutPoint(float x, float y)
		inline void setRotationAboutCenter() { updateCenterUniform(program); }
		inline void setRotationAboutPoint(const Vec2<float>& point) { updateCenterUniform(program, point); }
	};


	// bndr::BasicTriangle
	// Description: This is a basic triangle that you may specify a single color for
	class BNDR_API BasicTriangle : public GraphicsTriangle, public PolySurface {

	protected:

		virtual VertexArray* generateVertexArray() override;
	public:

		BasicTriangle(Vec2<float>&& coord1, Vec2<float>&& coord2, Vec2<float>&& coord3, const RGBAData& color = bndr::WHITE, int colorBufferSize = 4, bool super = false);
		// the rotation is about the PolySurface's center by default
		// you only need to call this if you changed the center of rotation to a different point
		// using setRotationAboutPoint(float x, float y)
		inline void setRotationAboutCenter() { updateCenterUniform(program); }
		inline void setRotationAboutPoint(const Vec2<float>& point) { updateCenterUniform(program, point); }

	};

	// bndr::ColorfulRect
	// Description: This is a rectangle that can have a color for every vertex
	class BNDR_API ColorfulRect : public BasicRect {

	protected:

		// colorful rect will specify additional values as opposed to basic rect
		virtual VertexArray* generateVertexArray() override;
		// update the color data in the vertex buffer of va
		virtual void updateColorData() override;
		// generate a program that allows for multiple colors to be used
		inline virtual Program* generateShaderProgram(int numTexes = 0) override { return Program::multiColorPolygonProgram(); }
		// define the colors of the rectangle
		void defineColors(std::vector<RGBAData>& colors);
	public:

		ColorfulRect(float x, float y, float width, float height, std::vector<RGBAData>&& colors = { bndr::WHITE }, int colorBufferSize = 16, bool super = false);
		// set a single fill color for the rect (if you only want a solid color then you should probably just use BasicRect)
		// (converts a rgba color to floats)
		virtual void setFillColor(const RGBAData& data) override;
		// set a color for every corner of the rectangle
		virtual void setFillColors(const RGBAData& bottomLeft, const RGBAData& topLeft, const RGBAData& topRight, const RGBAData& bottomRight);
	};

	// bndr::ColorfulTriangle
	// Description: This is a triangle that can have a color for every vertex
	class BNDR_API ColorfulTriangle : public BasicTriangle {

	protected:

		virtual VertexArray* generateVertexArray() override;
		// update the color data in the vertex buffer of va
		virtual void updateColorData() override;
		// generate a program that allows for multiple colors to be used
		inline virtual Program* generateShaderProgram(int numTexes = 0) override { return Program::multiColorPolygonProgram(); }
		// define the colors of the triangle
		void defineColors(std::vector<RGBAData>& colors);
	public:

		ColorfulTriangle(Vec2<float>&& coord1, Vec2<float>&& coord2, Vec2<float>&& coord3, std::vector<RGBAData>&& colors = { bndr::WHITE }, int colorBufferSize = 12, bool super = false);
		// set a single fill color for the triangle (if you only want a solid color then you should probably just use BasicTriangle)
		// (converts a rgba color to floats)
		virtual void setFillColor(const RGBAData& data) override;
		// set a color for every vertex
		void setFillColors(const RGBAData& one, const RGBAData& two, const RGBAData& three);

	};

	// bndr::TexturedRect
	// Description: This is a rectangle that can have a color for every vertex as well as blended textures
	class BNDR_API TexturedRect : public ColorfulRect {

	protected:

		// texture array to store the textures of the rect
		TextureArray* texArr;
		// textured rect will specify additional values as opposed to colorful rect
		virtual VertexArray* generateVertexArray() override;
		// update the color data in the vertex buffer of va
		virtual void updateColorData() override;
		// generate a program that allows for multiple colors to be used with multiple textures
		inline virtual Program* generateShaderProgram(int numTexes = 0) override { return Program::texPolygonProgram(numTexes); }
		// set the nested alpha weight for mix (for two or three textures bound at once)
		inline void setNestedAlphaWeight(float value) { program->setFloatUniformValue("nestedTexAlphaWeight", &value, bndr::FLOAT); }
		// set the outer alpha weight for mix (three textures bound at once)
		inline void setOuterAlphaWeight(float value) { program->setFloatUniformValue("outerTexAlphaWeight", &value, bndr::FLOAT); }
	public:

		TexturedRect(float x, float y, float width, float height, std::vector<RGBAData>&& colors = { bndr::WHITE }, std::initializer_list<Texture>&& texs = {}, std::vector<float>&& alphaWeights = {}, int colorBuffer = 16);
		void render() override;
		~TexturedRect() { delete texArr; }
	};
}

