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

#include "pch.h"
#include "IndexBuffer.h"

namespace bndr {

	IndexBuffer::IndexBuffer(std::vector<uint>&& indexData) {

		glGenBuffers(1, &bufferID);
		bind();
		size = indexData.size();
		GL_DEBUG_FUNC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * size, (const void*)&indexData[0], GL_DYNAMIC_DRAW));
		unbind();
	}

	IndexBuffer::IndexBuffer(const IndexBuffer& ib) {

		glGenBuffers(1, &bufferID);
		size = ib.size;
		std::unique_ptr<uint> data = ib.readData();
		bind();
		GL_DEBUG_FUNC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * size, (const void*)data.get(), GL_DYNAMIC_DRAW));
		unbind();
	}

	std::unique_ptr<uint> IndexBuffer::readData() const {

		uint* data = new uint[size];
		bind();
		GL_DEBUG_FUNC(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size * sizeof(uint), (void*)data));
		unbind();
		return std::unique_ptr<uint>(data);
	}

	void IndexBuffer::render(uint drawMode) {

		bind();
		GL_DEBUG_FUNC(glDrawElements(drawMode, size, GL_UNSIGNED_INT, NULL));
		unbind();
	}
}
