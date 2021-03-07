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
#include "graphical_bedrocks.h"

namespace bndr {

	// define static member variables
	Window* PixelSurface::windowInstance = nullptr;

	PixelSurface::~PixelSurface() {

		// delete vertex array, color buffer, and matrices
		delete program;
		delete va;
		delete colorBuffer;
		delete translation;
		delete rotation;
		delete scale;
	}

	void PolySurface::setTranslation(float xTrans, float yTrans) {

		// update the translation matrix in RAM
		(*translation)[0] = xTrans;
		(*translation)[1] = yTrans;
		updateTranslationUniform();

	}

	void PolySurface::setRotation(float theta) {

		// convert theta to radians
		float rad = theta * (BNDR_PI / 180.0f);
		// update the rotation matrix in RAM
		(*rotation)[0] = cosf(rad);
		(*rotation)[1] = sinf(rad);
		updateRotationUniform();

	}

	void PolySurface::setScale(float xScale, float yScale) {

		// update the scale matrix in RAM
		(*scale)[0] = xScale;
		(*scale)[1] = yScale;
		updateScaleUniform();

	}

	void PolySurface::changeTranslationBy(float xTrans, float yTrans) {

		// update the translation matrix in RAM
		(*translation)[0] += xTrans;
		(*translation)[1] += yTrans;
		updateTranslationUniform();

	}

	void PolySurface::changeRotationBy(float theta) {

		// convert theta to radians
		float rad = theta * (BNDR_PI / 180.0f);
		// update the rotation matrix in RAM
		(*rotation)[0] += cosf(rad);
		(*rotation)[1] += sinf(rad);
		updateRotationUniform();

	}

	void PolySurface::changeScaleBy(float xScale, float yScale) {

		// update the scale matrix in RAM
		(*scale)[0] += xScale;
		(*scale)[1] += yScale;
		updateScaleUniform();

	}

	void PolySurface::setFillColor(const RGBAData& data) {

		colorBuffer[0] = (float)data.red / 255.0f;
		colorBuffer[1] = (float)data.green / 255.0f;
		colorBuffer[2] = (float)data.blue / 255.0f;
		colorBuffer[3] = (float)data.alpha / 255.0f;
		updateColorData();
	}

	void PolySurface::render() {

		program->use();

		// render the PolySurface
		va->render();

		program->unuse();
	}

	BasicRect::BasicRect(float x, float y, float width, float height, const RGBAData& color, int colorBufferSize, bool super)
		: GraphicsRect(x, y, width, height), PolySurface(colorBufferSize) {

		if (!super) {

			init(colorBufferSize);
			setFillColor(color);
			setRotationAboutCenter();
		}
	}

	VertexArray* BasicRect::generateVertexArray() {

		// load the vertex array data at runtime (different descendants will have different vertex arrays)
		return new VertexArray(TRIANGLES,
			{
				(*pos)[0], (*pos)[1], 0.0f,
				(*pos)[0], (*pos)[1] + (*size)[1], 0.0f,
				(*pos)[0] + (*size)[0], (*pos)[1] + (*size)[1], 0.0f,
				(*pos)[0] + (*size)[0], (*pos)[1], 0.0f
		},
			3 * sizeof(float), 0, { 0, 1, 2, 2, 3, 0 });
	}

	VertexArray* BasicTriangle::generateVertexArray() {

		return new VertexArray(TRIANGLES, {
			(*vertex1)[0], (*vertex1)[1], 0.0f,
			(*vertex2)[0], (*vertex2)[1], 0.0f,
			(*vertex3)[0], (*vertex3)[1], 0.0f
			}, sizeof(float) * 3, 0);
	}

	BasicTriangle::BasicTriangle(Vec2<float>&& coord1, Vec2<float>&& coord2, Vec2<float>&& coord3, const RGBAData& color, int colorBufferSize, bool super)
		: GraphicsTriangle(std::move(coord1), std::move(coord2), std::move(coord3)), PolySurface(colorBufferSize) {

		if (!super) {

			init(colorBufferSize);
			setFillColor(color);
			setRotationAboutCenter();
		}
	}

	VertexArray* ColorfulRect::generateVertexArray() {

		return new VertexArray(TRIANGLES, {

			(*pos)[0], (*pos)[1], 0.0f, colorBuffer[0], colorBuffer[1], colorBuffer[2], colorBuffer[3],
			(*pos)[0], (*pos)[1] + (*size)[1], 0.0f, colorBuffer[4], colorBuffer[5], colorBuffer[6], colorBuffer[7],
			(*pos)[0] + (*size)[0], (*pos)[1] + (*size)[1], 0.0f, colorBuffer[8], colorBuffer[9], colorBuffer[10], colorBuffer[11],
			(*pos)[0] + (*size)[0], (*pos)[1], 0.0f, colorBuffer[12], colorBuffer[13], colorBuffer[14], colorBuffer[15]
			}, 7 * sizeof(float), bndr::RGBA_COLOR_ATTRIB, { 0, 1, 2, 2, 3, 0 });
	}

	void ColorfulRect::defineColors(std::vector<RGBAData>& colors) {

		// specify cases based on how many colors that are provided
		int s = colors.size();
		switch (s) {

		case 1:

			setFillColor(colors[0]);
			break;
		case 2:

			setFillColors(colors[0], colors[1], colors[0], colors[1]);
			break;
		case 3:

			setFillColors(colors[0], colors[1], colors[2], colors[0]);
			break;
		case 4:

			setFillColors(colors[0], colors[1], colors[2], colors[3]);
			break;
		default:

			setFillColor(bndr::WHITE);
			break;
		}
	}

	ColorfulRect::ColorfulRect(float x, float y, float width, float height, std::vector<RGBAData>&& colors, int colorBufferSize, bool super)
		: BasicRect(x,y,width,height,colors[0],colorBufferSize,true) {

		if (!super) {

			init(colorBufferSize);
			setRotationAboutCenter();
			defineColors(colors);
		}
		
	}

	void ColorfulRect::updateColorData() {

		float updatedData[28] = {										
			(*pos)[0], (*pos)[1], 0.0f, colorBuffer[0], colorBuffer[1], colorBuffer[2], colorBuffer[3],
			(*pos)[0], (*pos)[1] + (*size)[1], 0.0f, colorBuffer[4], colorBuffer[5], colorBuffer[6], colorBuffer[7],
			(*pos)[0] + (*size)[0], (*pos)[1] + (*size)[1], 0.0f, colorBuffer[8], colorBuffer[9], colorBuffer[10], colorBuffer[11],
			(*pos)[0] + (*size)[0], (*pos)[1], 0.0f, colorBuffer[12], colorBuffer[13], colorBuffer[14], colorBuffer[15]
		};
		va->updateVertexBufferData(updatedData);
	}

	void ColorfulRect::setFillColor(const RGBAData& data) {

		float color[4] = { (float)data.red / 255.0f, (float)data.green / 255.0f, (float)data.blue / 255.0f, (float)data.alpha / 255.0f };
		for (int i = 0; i < 16; i++) { colorBuffer[i] = color[i % 4]; }
		updateColorData();
	}

	void ColorfulRect::setFillColors(const RGBAData& bottomLeft, const RGBAData& topLeft, const RGBAData& topRight, const RGBAData& bottomRight) {

		colorBuffer[0] = (float)bottomLeft.red/255.0f; colorBuffer[1] = (float)bottomLeft.green/255.0f; colorBuffer[2] = (float)bottomLeft.blue/255.0f; colorBuffer[3] = (float)bottomLeft.alpha/255.0f;
		colorBuffer[4] = (float)topLeft.red/255.0f; colorBuffer[5] = (float)topLeft.green/255.0f; colorBuffer[6] = (float)topLeft.blue/255.0f; colorBuffer[7] = (float)topLeft.alpha/255.0f;
		colorBuffer[8] = (float)topRight.red/255.0f; colorBuffer[9] = (float)topRight.green/255.0f; colorBuffer[10] = (float)topRight.blue/255.0f; colorBuffer[11] = (float)topRight.alpha/255.0f;
		colorBuffer[12] = (float)bottomRight.red/255.0f; colorBuffer[13] = (float)bottomRight.green/255.0f; colorBuffer[14] = (float)bottomRight.blue/255.0f; colorBuffer[15] = (float)bottomRight.alpha/255.0f;
		updateColorData();
	}

	VertexArray* ColorfulTriangle::generateVertexArray() {

		// create the vertex array with all the data
		return new VertexArray(TRIANGLES,
			{
				(*vertex1)[0], (*vertex1)[1], 0.0f, colorBuffer[0], colorBuffer[1], colorBuffer[2], colorBuffer[3],
				(*vertex2)[0], (*vertex2)[1], 0.0f, colorBuffer[4], colorBuffer[5], colorBuffer[6], colorBuffer[7],
				(*vertex3)[0], (*vertex3)[1], 0.0f, colorBuffer[8], colorBuffer[9], colorBuffer[10], colorBuffer[11]
			}, 7 * sizeof(float), RGBA_COLOR_ATTRIB);
	}

	void ColorfulTriangle::defineColors(std::vector<RGBAData>& colors) {

		// specify cases based on how many colors that are provided
		int s = colors.size();
		switch (s) {

		case 1:

			setFillColor(colors[0]);
			break;
		case 2:

			setFillColors(colors[0], colors[1], colors[0]);
			break;
		case 3:

			setFillColors(colors[0], colors[1], colors[2]);
			break;
		default:

			setFillColor(bndr::WHITE);
			break;
		}
	}

	ColorfulTriangle::ColorfulTriangle(Vec2<float>&& coord1, Vec2<float>&& coord2, Vec2<float>&& coord3, std::vector<RGBAData>&& colors, int colorBufferSize, bool super)
	: BasicTriangle(std::move(coord1), std::move(coord2), std::move(coord3), colors[0], colorBufferSize, true) {

		if (!super) {

			init(colorBufferSize);
			setRotationAboutCenter();
			defineColors(colors);
		}

	}

	void ColorfulTriangle::updateColorData() {

		float updatedData[21] = {

			(*vertex1)[0], (*vertex1)[1], 0.0f, colorBuffer[0], colorBuffer[1], colorBuffer[2], colorBuffer[3],
			(*vertex2)[0], (*vertex2)[1], 0.0f, colorBuffer[4], colorBuffer[5], colorBuffer[6], colorBuffer[7],
			(*vertex3)[0], (*vertex3)[1], 0.0f, colorBuffer[8], colorBuffer[9], colorBuffer[10], colorBuffer[11]
		};
		va->updateVertexBufferData(updatedData);
	}

	void ColorfulTriangle::setFillColor(const RGBAData& data) {

		float color[4] = { (float)data.red / 255.0f, (float)data.green / 255.0f, (float)data.blue / 255.0f, (float)data.alpha / 255.0f };
		for (int i = 0; i < 12; i++) { colorBuffer[i] = color[i % 4]; }
		updateColorData();
	}

	void ColorfulTriangle::setFillColors(const RGBAData& one, const RGBAData& two, const RGBAData& three) {

		colorBuffer[0] = (float)one.red / 255.0f; colorBuffer[1] = (float)one.green / 255.0f; colorBuffer[2] = (float)one.blue / 255.0f; colorBuffer[3] = (float)one.alpha / 255.0f;
		colorBuffer[4] = (float)two.red / 255.0f; colorBuffer[5] = (float)two.green / 255.0f; colorBuffer[6] = (float)two.blue / 255.0f; colorBuffer[7] = (float)two.alpha / 255.0f;
		colorBuffer[8] = (float)three.red / 255.0f; colorBuffer[9] = (float)three.green / 255.0f; colorBuffer[10] = (float)three.blue / 255.0f; colorBuffer[11] = (float)three.alpha / 255.0f;
		updateColorData();
	}

	VertexArray* TexturedRect::generateVertexArray() {

		return new VertexArray(TRIANGLES,
			{

			(*pos)[0], (*pos)[1], 0.0f, colorBuffer[0], colorBuffer[1], colorBuffer[2], colorBuffer[3], 0.0f, 0.0f,
			(*pos)[0], (*pos)[1] + (*size)[1], 0.0f, colorBuffer[4], colorBuffer[5], colorBuffer[6], colorBuffer[7], 0.0f, 1.0f,
			(*pos)[0] + (*size)[0], (*pos)[1] + (*size)[1], 0.0f, colorBuffer[8], colorBuffer[9], colorBuffer[10], colorBuffer[11], 1.0f, 1.0f,
			(*pos)[0] + (*size)[0], (*pos)[1], 0.0f, colorBuffer[12], colorBuffer[13], colorBuffer[14], colorBuffer[15], 1.0f, 0.0f
			}, 9 * sizeof(float), bndr::RGBA_COLOR_ATTRIB | bndr::TEXTURE_COORDS_ATTRIB, { 0, 1, 2, 2, 3, 0 });
	}

	TexturedRect::TexturedRect(float x, float y, float width, float height, std::vector<RGBAData>&& colors, std::initializer_list<Texture>&& texs, int colorBufferSize)
		: ColorfulRect(x, y, width, height, {}, colorBufferSize, true) {

		init(colorBufferSize);
		setRotationAboutCenter();
		defineColors(colors);

		texArr = new TextureArray(std::move(texs));

	}

	void TexturedRect::updateColorData() {

		float updatedData[36] = {
			(*pos)[0], (*pos)[1], 0.0f, colorBuffer[0], colorBuffer[1], colorBuffer[2], colorBuffer[3], 0.0f, 0.0f,
			(*pos)[0], (*pos)[1] + (*size)[1], 0.0f, colorBuffer[4], colorBuffer[5], colorBuffer[6], colorBuffer[7], 0.0f, 1.0f,
			(*pos)[0] + (*size)[0], (*pos)[1] + (*size)[1], 0.0f, colorBuffer[8], colorBuffer[9], colorBuffer[10], colorBuffer[11], 1.0f, 1.0f,
			(*pos)[0] + (*size)[0], (*pos)[1], 0.0f, colorBuffer[12], colorBuffer[13], colorBuffer[14], colorBuffer[15], 1.0f, 0.0f
		};
		va->updateVertexBufferData(updatedData);
	}
}
