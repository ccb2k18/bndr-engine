/*MIT License

Copyright (c) 2020 Caleb Christopher Bender

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

		uint attribIndex = 0;
		int offset = 0;


		// at bare minimum we have a single vertex attribute pointer for the positions
		GL_DEBUG_FUNC(glEnableVertexAttribArray(attribIndex));
		GL_DEBUG_FUNC(glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, dataBlockBytes, (void*)offset));
		// increment the attrib index
		attribIndex++;
		// if the color attrib flag is set
		if (flags & RGBA_COLOR_ATTRIB) {

			GL_DEBUG_FUNC(glEnableVertexAttribArray(attribIndex));
			offset += 4;
			GL_DEBUG_FUNC(glVertexAttribPointer(attribIndex, 4, GL_FLOAT, GL_FALSE, dataBlockBytes, (void*)offset));
			attribIndex++;
		}
		// if the normals attrib flag is set
		if (flags & VERTEX_NORMALS_ATTRIB) {

			GL_DEBUG_FUNC(glEnableVertexAttribArray(attribIndex));
			offset += 3;
			GL_DEBUG_FUNC(glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, dataBlockBytes, (void*)offset));
			attribIndex++;
		}
		// if texture coordinates are specified
		if (flags & TEXTURE_COORDS_ATTRIB) {

			GL_DEBUG_FUNC(glEnableVertexAttribArray(attribIndex));
			offset += 2;
			GL_DEBUG_FUNC(glVertexAttribPointer(attribIndex, 2, GL_FLOAT, GL_FALSE, dataBlockBytes, (void*)offset));
			attribIndex++;
		}
		// if there is more than one texture a texture index will be specified
		if (flags & TEXTURE_INDEX_ATTRIB) {

			GL_DEBUG_FUNC(glEnableVertexAttribArray(attribIndex));
			offset += 1;
			GL_DEBUG_FUNC(glVertexAttribPointer(attribIndex, 1, GL_FLOAT, GL_FALSE, dataBlockBytes, (void*)offset));
		}
		unbind();
	}

	void VertexBuffer::render(uint drawMode) {

		bind();
		GL_DEBUG_FUNC(glDrawArrays(drawMode, 0, verticesNumber));
		unbind();
	}
}
