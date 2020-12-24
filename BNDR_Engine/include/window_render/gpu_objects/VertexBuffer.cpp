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

	VertexBuffer::VertexBuffer(std::vector<float>&& vecRefData, int dataBlockBytes, uint flags) {

		// generate and bind the buffer
		glGenBuffers(1, &bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vecRefData.size(), &vecRefData[0], GL_DYNAMIC_DRAW);

		uint attribIndex = 0;
		int offset = 0;


		// at bare minimum we have a single vertex attribute pointer for the positions
		glEnableVertexAttribArray(attribIndex);
		glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, dataBlockBytes, (const void*)offset);
		// increment the attrib index
		attribIndex++;
		// if the color attrib flag is set
		if (flags & RGBA_COLOR_ATTRIB) {

			glEnableVertexAttribArray(attribIndex);
			offset += 4;
			glVertexAttribPointer(attribIndex, 4, GL_FLOAT, GL_FALSE, dataBlockBytes, (const void*)offset);
			attribIndex++;
		}
		// if the normals attrib flag is set
		if (flags & VERTEX_NORMALS_ATTRIB) {

			glEnableVertexAttribArray(attribIndex);
			offset += 3;
			glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, dataBlockBytes, (const void*)offset);
			attribIndex++;
		}
		// if texture coordinates are specified
		if (flags & TEXTURE_COORDS_ATTRIB) {

			glEnableVertexAttribArray(attribIndex);
			offset += 2;
			glVertexAttribPointer(attribIndex, 2, GL_FLOAT, GL_FALSE, dataBlockBytes, (const void*)offset);
			attribIndex++;
		}
		// if there is more than one texture a texture index will be specified
		if (flags & TEXTURE_INDEX_ATTRIB) {

			glEnableVertexAttribArray(attribIndex);
			offset += 1;
			glVertexAttribPointer(attribIndex, 1, GL_FLOAT, GL_FALSE, dataBlockBytes, (const void*)offset);
		}
	}

	VertexBuffer::~VertexBuffer() {

		glDeleteBuffers(1, &bufferID);
	}
}
