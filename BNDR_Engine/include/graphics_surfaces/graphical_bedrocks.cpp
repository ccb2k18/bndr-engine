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
	// program for shape base class is default polygon program
	Program Shape::defaultPoly;
	Program Shape::multiColorPoly;


	PixelSurface::~PixelSurface() {

		// delete vertex array, color buffer, and matrices
		delete va;
		delete colorBuffer;
		delete translation;
		delete rotation;
		delete scale;
	}

	Shape::Shape() {

		definePrograms();
	}

	void Shape::setTranslation(float xTrans, float yTrans) {

		// update the translation matrix in RAM
		translation->setAt(0, 2, xTrans);
		translation->setAt(1, 2, yTrans);

	}

	void Shape::setRotation(float theta) {

		// convert theta to radians
		float rad = theta * (BNDR_PI / 180.0f);
		// update the rotation matrix in RAM
		rotation->setAt(0, 0, cosf(rad));
		rotation->setAt(0, 1, -sinf(rad));
		rotation->setAt(1, 0, sinf(rad));
		rotation->setAt(1, 1, cosf(rad));

	}

	void Shape::setScale(float xScale, float yScale) {

		// update the scale matrix in RAM
		scale->setAt(0, 0, xScale);
		scale->setAt(1, 1, yScale);

	}

	void Shape::changeTranslationBy(float xTrans, float yTrans) {

		// update the translation matrix in RAM
		translation->setAt(0, 2, translation->getAt(0, 2) + xTrans);
		translation->setAt(1, 2, translation->getAt(1, 2) + yTrans);

	}

	void Shape::changeRotationBy(float theta) {

		// convert theta to radians
		float rad = theta * (BNDR_PI / 180.0f);
		// update the rotation matrix in RAM
		rotation->setAt(0, 0, rotation->getAt(0, 0) + cosf(rad));
		rotation->setAt(0, 1, rotation->getAt(0, 1) - sinf(rad));
		rotation->setAt(1, 0, rotation->getAt(1, 0) + sinf(rad));
		rotation->setAt(1, 1, rotation->getAt(1, 1) + cosf(rad));

	}

	void Shape::changeScaleBy(float xScale, float yScale) {

		// update the scale matrix in RAM
		scale->setAt(0, 0, scale->getAt(0, 0) + xScale);
		scale->setAt(1, 1, scale->getAt(1, 1) + yScale);

	}

	void Shape::setFillColor(const RGBAData& data) {

		float rgba[4] = { data.red, data.green, data.blue, data.alpha };
		colorBuffer[0] = rgba[0];
		colorBuffer[1] = rgba[1];
		colorBuffer[2] = rgba[2];
		colorBuffer[3] = rgba[3];
	}

	void Shape::render(Program* currentProgram) {

		// update the matrices
		updateTranslationUniform();
		updateRotationUniform();
		updateScaleUniform();

		// update the fill color(s)
		updateColorData();


		currentProgram->use();

		// render the shape
		va->render();

		currentProgram->unuse();
	}

	BasicRect::BasicRect(float x, float y, float width, float height) : Shape(), GraphicsRect(x, y, width, height) {

		loadColorBuffer(4);

		va = new VertexArray(TRIANGLES,
			{
				x, y, 0.0f,
				x, y + height, 0.0f,
				x + width, y + height, 0.0f,
				x + width, y, 0.0f
			},
			3 * sizeof(float), 0, { 0, 1, 2, 2, 3, 0 });
	}

	BasicTriangle::BasicTriangle(const Vec2<float>& coord1, const Vec2<float>& coord2, const Vec2<float>& coord3) : Shape() {

		loadColorBuffer(4);

		va = new VertexArray(TRIANGLES, {
			coord1.getValue(0), coord1.getValue(1), 0.0f,
			coord2.getValue(0), coord2.getValue(1), 0.0f,
			coord3.getValue(0), coord3.getValue(1), 0.0f
			}, sizeof(float) * 3, 0);
	}

	ColorfulRect::ColorfulRect(float x, float y, float width, float height) : Shape(), GraphicsRect(x, y, width, height) {

		loadColorBuffer(16);

		va = new VertexArray(TRIANGLES, {

			x, y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			x, y + height, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			x + width, y + height, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			x + width, y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
			}, 7 * sizeof(float), bndr::RGBA_COLOR_ATTRIB, { 0, 1, 2, 2, 3, 0 });

	}

	void ColorfulRect::updateColorData(Program* currentProgram) {

		float updatedData[28] = {
			(*pos)[0], (*pos)[1], 0.0f, colorBuffer[0], colorBuffer[1], colorBuffer[2], colorBuffer[3],
			(*pos)[0], (*pos)[1] + (*size)[1], 0.0f, colorBuffer[4], colorBuffer[5], colorBuffer[6], colorBuffer[7],
			(*pos)[0] + (*size)[0], (*pos)[1] + (*size)[1], 0.0f, colorBuffer[8], colorBuffer[9], colorBuffer[10], colorBuffer[11],
			(*pos)[0] + (*size)[0], (*pos)[1], 0.0f, colorBuffer[12], colorBuffer[13], colorBuffer[14], colorBuffer[15]
		};
		va->updateVertexBufferData(updatedData);
	}

	void ColorfulRect::setFillColor(const RGBAData& data) {

		float color[4] = { data.red, data.green, data.blue, data.alpha };
		for (int i = 0; i < 16; i++) { colorBuffer[i] = color[i % 4]; }
	}

	void ColorfulRect::setFillColors(const RGBAData& bottomLeft, const RGBAData& topLeft, const RGBAData& topRight, const RGBAData& bottomRight) {

		colorBuffer[0] = bottomLeft.red; colorBuffer[1] = bottomLeft.green; colorBuffer[2] = bottomLeft.blue; colorBuffer[3] = bottomLeft.alpha;
		colorBuffer[4] = topLeft.red; colorBuffer[5] = topLeft.green; colorBuffer[6] = topLeft.blue; colorBuffer[7] = topLeft.alpha;
		colorBuffer[8] = topRight.red; colorBuffer[9] = topRight.green; colorBuffer[10] = topRight.blue; colorBuffer[11] = topRight.alpha;
		colorBuffer[12] = bottomRight.red; colorBuffer[13] = bottomRight.green; colorBuffer[14] = bottomRight.blue; colorBuffer[15] = bottomRight.alpha;
	}


}
