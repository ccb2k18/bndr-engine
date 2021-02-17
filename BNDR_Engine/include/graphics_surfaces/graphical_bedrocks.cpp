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

		// delete vertex array and matrices
		delete va;
		delete translation;
		delete rotation;
		delete scale;
	}

	Shape::Shape(float x, float y, float width, float height) : PixelSurface() {

		// if defaultPoly and multiColorPoly have not been defined then define them
		if (defaultPoly.getID() == 0) {

			defaultPoly = Program::defaultPolygonProgram();
			multiColorPoly = Program::multiColorPolygonProgram();
		}
		// create the fill color array on the heap
		fillColor = new float[4];
		// color will be white by default
		setFillColor(1.0f, 1.0f, 1.0f, 1.0f);
		// set the position
		position = new Vec3<float>(x, y, 0.0f);

		va = new VertexArray(TRIANGLES, 
			{
				x, y, 0.0f,
				x, y + height, 0.0f,
				x + width, y + height, 0.0f,
				x + width, y, 0.0f
			},
			3 * sizeof(float), 0, {0, 1, 2, 2, 3, 0});
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

	void Shape::setFillColor(float red, float green, float blue, float alpha) {

		fillColor[0] = red;
		fillColor[1] = green;
		fillColor[2] = blue;
		fillColor[3] = alpha;
	}

	void Shape::render() {

		// update the matrices
		updateTranslationUniform();
		updateRotationUniform();
		updateScaleUniform();

		// update the fill color
		updateColorUniform();


		Shape::defaultPoly.use();

		// render the shape
		va->render();

		Shape::defaultPoly.unuse();
	}

	Shape::~Shape() {

		delete position;
		delete fillColor;
	}


}
