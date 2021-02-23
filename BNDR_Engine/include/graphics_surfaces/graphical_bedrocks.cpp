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

	void Shape::setTranslation(float xTrans, float yTrans) {

		// update the translation matrix in RAM
		translation->setAt(0, 2, xTrans);
		translation->setAt(1, 2, yTrans);
		updateTranslationUniform();

	}

	void Shape::setRotation(float theta) {

		// convert theta to radians
		float rad = theta * (BNDR_PI / 180.0f);
		// update the rotation matrix in RAM
		rotation->setAt(0, 0, cosf(rad));
		rotation->setAt(0, 1, -sinf(rad));
		rotation->setAt(1, 0, sinf(rad));
		rotation->setAt(1, 1, cosf(rad));
		updateRotationUniform();

	}

	void Shape::setScale(float xScale, float yScale) {

		// update the scale matrix in RAM
		scale->setAt(0, 0, xScale);
		scale->setAt(1, 1, yScale);
		updateScaleUniform();

	}

	void Shape::changeTranslationBy(float xTrans, float yTrans) {

		// update the translation matrix in RAM
		translation->setAt(0, 2, translation->getAt(0, 2) + xTrans);
		translation->setAt(1, 2, translation->getAt(1, 2) + yTrans);
		updateTranslationUniform();

	}

	void Shape::changeRotationBy(float theta) {

		// convert theta to radians
		float rad = theta * (BNDR_PI / 180.0f);
		// update the rotation matrix in RAM
		rotation->setAt(0, 0, rotation->getAt(0, 0) + cosf(rad));
		rotation->setAt(0, 1, rotation->getAt(0, 1) - sinf(rad));
		rotation->setAt(1, 0, rotation->getAt(1, 0) + sinf(rad));
		rotation->setAt(1, 1, rotation->getAt(1, 1) + cosf(rad));
		updateRotationUniform();

	}

	void Shape::changeScaleBy(float xScale, float yScale) {

		// update the scale matrix in RAM
		scale->setAt(0, 0, scale->getAt(0, 0) + xScale);
		scale->setAt(1, 1, scale->getAt(1, 1) + yScale);
		updateScaleUniform();

	}

	void Shape::setFillColor(const RGBAData& data) {

		colorBuffer[0] = (float)data.red / 255.0f;
		colorBuffer[1] = (float)data.green / 255.0f;
		colorBuffer[2] = (float)data.blue / 255.0f;
		colorBuffer[3] = (float)data.alpha / 255.0f;
		updateColorData();
	}

	void Shape::render() {

		program->use();

		// render the shape
		va->render();

		program->unuse();
	}

	BasicRect::BasicRect(float x, float y, float width, float height, const RGBAData& color) : Shape(), GraphicsRect(x, y, width, height) {

		program = Program::defaultPolygonProgram();
		loadColorBuffer(4);
		setFillColor(color);

		va = new VertexArray(TRIANGLES,
			{
				x, y, 0.0f,
				x, y + height, 0.0f,
				x + width, y + height, 0.0f,
				x + width, y, 0.0f
			},
			3 * sizeof(float), 0, { 0, 1, 2, 2, 3, 0 });
	}

	BasicTriangle::BasicTriangle(const Vec2<float>& coord1, const Vec2<float>& coord2, const Vec2<float>& coord3, const RGBAData& color) : Shape() {

		program = Program::defaultPolygonProgram();
		loadColorBuffer(4);
		setFillColor(color);

		va = new VertexArray(TRIANGLES, {
			coord1.getValue(0), coord1.getValue(1), 0.0f,
			coord2.getValue(0), coord2.getValue(1), 0.0f,
			coord3.getValue(0), coord3.getValue(1), 0.0f
			}, sizeof(float) * 3, 0);
	}

	ColorfulRect::ColorfulRect(float x, float y, float width, float height, std::vector<RGBAData>&& colors) : Shape(), GraphicsRect(x, y, width, height) {

		program = Program::multiColorPolygonProgram();
		loadColorBuffer(16);
		// create the vertex array with all the data
		va = new VertexArray(TRIANGLES, {

			(*pos)[0], (*pos)[1], 0.0f, colorBuffer[0], colorBuffer[1], colorBuffer[2], colorBuffer[3],
			(*pos)[0], (*pos)[1] + (*size)[1], 0.0f, colorBuffer[4], colorBuffer[5], colorBuffer[6], colorBuffer[7],
			(*pos)[0] + (*size)[0], (*pos)[1] + (*size)[1], 0.0f, colorBuffer[8], colorBuffer[9], colorBuffer[10], colorBuffer[11],
			(*pos)[0] + (*size)[0], (*pos)[1], 0.0f, colorBuffer[12], colorBuffer[13], colorBuffer[14], colorBuffer[15]
			}, 7 * sizeof(float), bndr::RGBA_COLOR_ATTRIB, { 0, 1, 2, 2, 3, 0 });
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


}
