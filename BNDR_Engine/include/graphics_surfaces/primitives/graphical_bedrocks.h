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
	struct VAContainer {

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

		RGBAData(uint8 r, uint8 g, uint8 b, uint8 a) : red(r), green(g), blue(b), alpha(a) {}
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

	// if you want coordinates to be in percent screen size instead of absolute values in pixels
	typedef int percent;

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
		static Vec2<float> windowInitialSize;
		static float windowAspect;
		// matrices for transformations of the surface
		Vec2<float>* translation;
		//Vec3<float>* rotation;
		float rotation;
		Vec2<float>* scale;
		// this constructor will be called for every child of PixelSurface
		// by default the color buffer has a size of 4 for 1 rgba entry
		// this can be changed by calling the constructor below and passing in a color buffer size
		PixelSurface() : translation(new Vec2<float>(0.0f, 0.0f)), rotation(0.0f),
			scale(new Vec2<float>(1.0f, 1.0f)), program(nullptr), colorBuffer(nullptr), va(nullptr) {
		
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
		PixelSurface& operator=(const PixelSurface&) = delete;
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
		// get the initial size of the window
		static Vec2<float> getWindowInitialSize() { return windowInitialSize; }
		// get the window aspect ratio
		static float getWindowAspect() { return PixelSurface::windowAspect; }
		// define the window instance for all pixel surfaces (if you don't do this nothing will draw and you will get an exception)
		static void setWindowInstance(Window* window) {

			if (windowInstance != nullptr) {

				BNDR_EXCEPTION("You cannot reinstaniate the window instance after it has already been set.");
			}
			windowInstance = window;
			std::pair<float, float> windowSize = window->getSize();
			windowInitialSize = Vec2<float>(windowSize.first, windowSize.second);
			windowAspect = windowSize.second / windowSize.first;

		}
		~PixelSurface();
	};

	// bndr::PolySurface
	// Description: base class for all PolySurfaces (rectangles, triangles, and ellipses/circles)
	class BNDR_API PolySurface : public PixelSurface {

	protected:

		// update the translation uniform in the program
		inline virtual void updateTranslationUniform() override { program->setFloatUniformValue("translation", translation->getData(), VEC2); }
		// update the rotation uniform in the program
		inline virtual void updateRotationUniform() override { program->setFloatUniformValue("theta", &rotation, FLOAT); }
		// update the scale uniform in the program
		inline virtual void updateScaleUniform() override { program->setFloatUniformValue("scale", scale->getData(), VEC2); }
		// update the color uniform in the program
		inline virtual void updateColorData() override { program->setFloatUniformValue("color", colorBuffer, VEC4); }
		// default constructor
		PolySurface() : PixelSurface() {}
		// colorBufferSize and numTexes are used by children of PolySurface
		void init(int colorBufferSize, bool hasTex) {

			// generate the program for the polysurface
			program = generateShaderProgram(hasTex ? 1 : 0);
			// define the aspect ratio in the program
			program->setFloatUniformValue("aspect", &windowAspect, FLOAT);
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

		// converts screen space coordinates into OpenGL space coordinates
		static Vec2<float> convertScreenSpaceToGLSpace(Vec2<float>&& coordinate) {

			Vec2<float> newCoordinate;
			// retrieve window size
			Vec2<float> size = PixelSurface::windowInstance->getSize();
			newCoordinate[0] = ((coordinate[0] / size[1]) * 2.0f) - 1.0f;
			//newCoordinate[0] /= PixelSurface::windowAspect;
			newCoordinate[1] = ((coordinate[1] / size[1]) * 2.0f) - 1.0f;
			return newCoordinate;
		}

		// use for size of rect
		static Vec2<float> convertScreenSpaceBetween0And2(Vec2<float>&& sizeCoordinate) {

			Vec2<float> newCoordinate;
			// retrieve window size
			Vec2<float> size = PixelSurface::windowInitialSize;//PixelSurface::windowInstance->getSize();
			newCoordinate[0] = (sizeCoordinate[0] / size[1]) * 2.0f;
			//newCoordinate[0] /= PixelSurface::windowAspect;
			newCoordinate[1] = (sizeCoordinate[1] / size[1]) * 2.0f;
			return newCoordinate;
		}

		// convert from normalized coordinates to pixels
		static float convertCoordFromGLSpaceToScreenSpace(float dimension, bool isX) {

			Vec2<float> screenSize = PixelSurface::windowInstance->getSize();
			return (dimension + 1.0f) * (((isX) ? screenSize[0] : screenSize[1]) / 2.0f);
		}

		// convert from normalized size to pixels
		static float convertSizeFrom0And2ToScreenSpace(float dimension, bool isWidth) {

			Vec2<float> screenSize = PixelSurface::windowInstance->getSize();
			return (dimension) * (((isWidth) ? screenSize[0] : screenSize[1]) / 2.0f);
		}

		// convert from percent to GL space
		static Vec2<float> convertCoordFromPercentToGLSpace(Vec2<float>&& coordinate) {

			return (((coordinate / 100.0f) * 2.0f) - 1.0f);
		}

		// convert from GL space to percent
		// (percent is in terms of screen height)
		static Vec2<float> convertCoordFromGLSpaceToPercent(Vec2<float>&& coordinate) {

			return ((coordinate + 1.0f) / 2.0f) * 100.0f;
		}

		// convert from 0 to 2 to percent
		static Vec2<float> convertCoordFrom0and2ToPercent(Vec2<float>&& coordinate) {

			return ((coordinate) / 2.0f) * 100.0f;
		}

		// convert percent to between 0 and 2
		static Vec2<float> convertCoordFromPercentTo0and2(Vec2<float>&& coordinate) {

			return (((coordinate / 100.0f) * 2.0f));
		}


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
		// whether the rotation is about the center
		mutable bool aboutCenter = false;
		GraphicsEntity() : center(new Vec2<float>()) {}
	public:
		// update the center of rotation of the entity
		inline void updateCenterUniform(Program* program) const { program->setFloatUniformValue("center", center->getData(), VEC2); aboutCenter = true; }
		// update the center of rotation of the entity with a custom point
		inline void updateCenterUniform(Program* program, const Vec2<float>& point) const { program->setFloatUniformValue("center", point.getData(), VEC2); aboutCenter = false; }
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
		GraphicsRect(Vec2<float>&& newPos, Vec2<float>&& newSize) : GraphicsEntity(), pos(new Vec2<float>(newPos[0], newPos[1])),
			size(new Vec2<float>(newSize[0], newSize[1])) {
		
			(*center) = (*pos) + (*size) / 2.0f;
		}
	public:
		inline virtual Vec2<float> getPos() { return Vec2<float>(*pos); }
		inline void setPos(const Vec2<float>& vec) {
			(*pos) = vec;
		}
		inline virtual void updateCenter() {
			// update the center of the shape based on position and size
			(*center) = (*pos) + (*size) / 2.0f;
		}
		inline void addPos(const Vec2<float>& vec) { (*pos) += vec; }
		inline Vec2<float> virtual getSize() { return Vec2<float>(*size); }
		inline Vec2<float> virtual getCenter() {
			return Vec2<float>(*center);
		}
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

		BasicRect() : GraphicsRect(Vec2<float>(), Vec2<float>()) {}
		// contstructor for absolute coordinates and width and height in pixels
		BasicRect(float x, float y, float width, float height, const RGBAData& color = bndr::WHITE, int colorBufferSize = 4, bool super = false);
		
		// the rotation is about the PolySurface's center by default
		// you only need to call this if you changed the center of rotation to a different point
		// using setRotationAboutPoint(float x, float y)
		inline void setRotationAboutCenter() { updateCenterUniform(program); }
		inline void setRotationAboutPoint(const Vec2<float>& point) { updateCenterUniform(program, point); }
		// get the rendered size of the rect taking into account scale
		inline Vec2<float> getSize() override { return Vec2<float>((*size)[0] * (*scale)[0], (*size)[1] * (*scale)[1]); }
		// update the rendered center of the rect taking into account the rendered position and size
		inline void updateCenter() override { (*center) = (*pos) + getSize() / 2.0f; }
		// get the rendered position of the shape in GL coordinates (calling this repeatedly on many surfaces will slow down performance)
		Vec2<float> getPos() override {

			Vec2<float> newPos = (*pos);
			// first scale the position with the size
			newPos[0] *= (*scale)[0];
			newPos[1] *= (*scale)[1];
			// then apply the rotation only if it is not rotating about its center
			if (!aboutCenter) {

				Vec2<float> unitRotation(cosf(rotation), sinf(rotation));
				newPos -= *center;
				newPos = Vec2<float>(newPos[0] * unitRotation[0] - newPos[1] * unitRotation[1], newPos[1] * unitRotation[0] + newPos[0] * unitRotation[1]);
				newPos += *center;
			}
			// apply the translation
			newPos += *translation;
			// return the position
			return newPos;
		}
		// get the rendered center position of the shape in GL coordinates
		inline Vec2<float> getCenter() override { return getPos() + getSize() / 2.0f; }

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

		ColorfulRect() {}
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
	// Description: This is a rectangle that can have a color for every vertex as well as a texture
	class BNDR_API TexturedRect : public ColorfulRect {

	protected:

		// texture to store the texture of the rect
		Texture* tex = nullptr;
		// textured rect will specify additional values as opposed to colorful rect
		virtual VertexArray* generateVertexArray() override;
		// update the color data in the vertex buffer of va
		virtual void updateColorData() override;
		// generate a program that allows for multiple colors to be used with multiple textures
		inline virtual Program* generateShaderProgram(int numTexes = 0) override { return (numTexes == 1) ? Program::texPolygonProgram(1) : Program::multiColorPolygonProgram(); }
	public:

		TexturedRect(float x, float y, float width, float height, std::vector<RGBAData>&& colors = { bndr::WHITE }, Texture* newTex = nullptr, int colorBuffer = 16);
		// copy constructor
		TexturedRect(const TexturedRect& texRect);
		// move constructor is not allowed
		// assignment operator is not allowed
		TexturedRect& operator=(const TexturedRect&) = delete;
		void render() override;
		// change the texture that the textured rect will render
		inline void changeTexture(Texture* newTex) { tex = newTex; }
		// get the texture of the TexturedRect
		// WARNING: Do not use this method! This is meant to be used by classes only!
		inline Texture* getTexture() { return tex; }
		~TexturedRect() { if (tex != nullptr) { delete tex; } }
	};
}

