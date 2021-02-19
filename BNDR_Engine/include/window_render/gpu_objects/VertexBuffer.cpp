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
#include "VertexBuffer.h"

namespace bndr {


	VertexBuffer::VertexBuffer(std::vector<float>&& vertexData, int dataBlockBytes, uint flags) {

		// generate and bind the buffer
		GL_DEBUG_FUNC(glGenBuffers(1, &bufferID));
		bind();
		GL_DEBUG_FUNC(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), &vertexData[0], GL_DYNAMIC_DRAW));

		// the vertices number is equal to the size of the entire data divided by the size of each data block
		verticesNumber = (sizeof(float) * vertexData.size()) / dataBlockBytes;
		floatsPerBlock = dataBlockBytes / 4;
		vbFlags = flags;

		VertexBuffer::loadVertexAttribs((uint)0, 0, dataBlockBytes, vbFlags);
		unbind();
	}


	VertexBuffer::VertexBuffer(const VertexBuffer& vb) {

		verticesNumber = vb.verticesNumber;
		floatsPerBlock = vb.floatsPerBlock;
		vbFlags = vb.vbFlags;
		float* vertexData = vb.readData();

		// generate and bind the buffer
		GL_DEBUG_FUNC(glGenBuffers(1, &bufferID));
		bind();
		GL_DEBUG_FUNC(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesNumber * floatsPerBlock, vertexData, GL_DYNAMIC_DRAW));
		delete[] vertexData;

		VertexBuffer::loadVertexAttribs((uint)0, 0, floatsPerBlock * sizeof(float), vbFlags);
		unbind();

	}

	float* VertexBuffer::readData() const {

		float* data = new float[verticesNumber*floatsPerBlock];
		bind();
		GL_DEBUG_FUNC(glGetBufferSubData(GL_ARRAY_BUFFER, 0, verticesNumber * floatsPerBlock * sizeof(float), (void*)data));
		unbind();
		return data;
	}

	void VertexBuffer::writeData(float* data, int size) {

		bind();
		GL_DEBUG_FUNC(glBufferData(GL_ARRAY_BUFFER, verticesNumber * floatsPerBlock * sizeof(float), data, GL_DYNAMIC_DRAW));
		unbind();
	}

	void VertexBuffer::render(uint drawMode) {

		bind();
		GL_DEBUG_FUNC(glDrawArrays(drawMode, 0, verticesNumber));
		unbind();
	}
}
