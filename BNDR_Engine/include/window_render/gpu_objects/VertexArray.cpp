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
#include "VertexArray.h"

namespace bndr {

	VertexArray::VertexArray(uint drawingMode, std::vector<float>&& vertexData, int dataBlockBytes, uint vertexBufferFlags, std::vector<uint>&& indexData) {

		drawMode = drawingMode;
		GL_DEBUG_FUNC(glGenVertexArrays(1, &arrayID));
		bind();

		// assign the pointers to new instances of VertexBuffer and IndexBuffer
		vBuffer = new VertexBuffer(std::move(vertexData), dataBlockBytes, vertexBufferFlags);
		iBuffer = new IndexBuffer(std::move(indexData));

		unbind();
	}

	VertexArray::VertexArray(uint drawingMode, std::vector<float>&& vertexData, int dataBlockBytes, uint vertexBufferFlags) {

		drawMode = drawingMode;
		GL_DEBUG_FUNC(glGenVertexArrays(1, &arrayID));
		bind();

		// only create an instance of a VertexBuffer
		vBuffer = new VertexBuffer(std::move(vertexData), dataBlockBytes, vertexBufferFlags);

		unbind();
	}

	VertexArray::VertexArray(const VertexArray& va) {

		drawMode = va.drawMode;
		glGenVertexArrays(1, &arrayID);
		bind();

		// create the vertex buffer from the other va's vb
		vBuffer = new VertexBuffer(*(va.getVBuffer()));
		// while the vertex buffer is guaranteed to be defined the index buffer is not
		if (va.getIBuffer() != nullptr) {

			iBuffer = new IndexBuffer(*(va.getIBuffer()));
		}
		unbind();
	}

	void VertexArray::render() {

		bind();
		// if the programmer wants to use an IndexBuffer then render using the IndexBuffer
		if (iBuffer != nullptr) {

			iBuffer->render(drawMode);
		}
		// otherwise use the VertexBuffer instead since the instance was constructed without an IndexBuffer
		else {

			vBuffer->render(drawMode);
		}
		unbind();
	}

	VertexArray::~VertexArray() {

		// delete the vertex buffer
		delete vBuffer;
		// delete the index buffer if it exists
		if (iBuffer != nullptr) {

			delete iBuffer;
		}
		// delete the vertex array
		glDeleteVertexArrays(1, &arrayID);
	}
}
