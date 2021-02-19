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
#include "GLDebug.h"

namespace bndr {

	// bndr::vertexBufferParams
	// Description: Flags to specify a variable number of vertex attributes for the constructor of bndr::VertexBuffer
	enum vertexBufferParams {

		RGBA_COLOR_ATTRIB = 0x01,
		VERTEX_NORMALS_ATTRIB = 0x02,
		TEXTURE_COORDS_ATTRIB = 0x04,
		TEXTURE_INDEX_ATTRIB = 0x08
	};

	// bndr::VertexBuffer
	// Description: The VertexBuffer class is used as an abstraction above OpenGL vertex buffers. All OpenGL operations are
	// meant to be done only by the class so that the programmer can just use the BNDR API.
	class BNDR_API VertexBuffer {

		uint bufferID;
		// the number of vertices we want to draw
		int verticesNumber;
		// floats per vertex block
		int floatsPerBlock;
		// store the flags for copying or moving
		uint vbFlags;

		static void loadVertexAttribs(uint attribIndex, int offset, int dataBlockBytes, uint flags) {

			// at bare minimum we have a single vertex attribute pointer for the positions
			GL_DEBUG_FUNC(glEnableVertexAttribArray(attribIndex));
			GL_DEBUG_FUNC(glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, dataBlockBytes, (void*)offset));
			// add to offset
			offset += 3 * sizeof(float);
			// increment the attrib index
			attribIndex++;
			// if the color attrib flag is set
			if (flags & RGBA_COLOR_ATTRIB) {

				GL_DEBUG_FUNC(glEnableVertexAttribArray(attribIndex));
				GL_DEBUG_FUNC(glVertexAttribPointer(attribIndex, 4, GL_FLOAT, GL_FALSE, dataBlockBytes, (void*)offset));
				offset += 4 * sizeof(float);
				attribIndex++;
			}
			// if the normals attrib flag is set
			if (flags & VERTEX_NORMALS_ATTRIB) {

				GL_DEBUG_FUNC(glEnableVertexAttribArray(attribIndex));
				GL_DEBUG_FUNC(glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, dataBlockBytes, (void*)offset));
				offset += 3 * sizeof(float);
				attribIndex++;
			}
			// if texture coordinates are specified
			if (flags & TEXTURE_COORDS_ATTRIB) {

				GL_DEBUG_FUNC(glEnableVertexAttribArray(attribIndex));
				GL_DEBUG_FUNC(glVertexAttribPointer(attribIndex, 2, GL_FLOAT, GL_FALSE, dataBlockBytes, (void*)offset));
				offset += 2 * sizeof(float);
				attribIndex++;
			}
			// if there is more than one texture a texture index will be specified
			if (flags & TEXTURE_INDEX_ATTRIB) {

				GL_DEBUG_FUNC(glEnableVertexAttribArray(attribIndex));
				GL_DEBUG_FUNC(glVertexAttribPointer(attribIndex, 1, GL_FLOAT, GL_FALSE, dataBlockBytes, (void*)offset));
				offset += sizeof(float);
			}
		}

	public:

		// bndr::VertexBuffer::VertexBuffer
		// Arguments: 
		//        vertexData = r-value reference of a vector of vertex data
		//        dataBlockBytes = the size in bytes for each block of vertex data
		//        flags = various uint flags defined in enum bndr::vertexBufferParams to customize VertexBuffer class
		// Description: From a vector of floats, the size of each data block, and flags that indicate how many vertex
		// attribs, this constructor will create an OpenGL vertex buffer in video memory that can be used in shaders
		VertexBuffer(std::vector<float>&& vertexData, int dataBlockBytes, uint flags);
		// the copy constructor
		VertexBuffer(const VertexBuffer&);
		// the move constructor is not allowed
		VertexBuffer(VertexBuffer&&) = delete;
		// assignment operator is not allowed
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		// read from the buffer data in the GPU
		// DO NOT USE: This is meant to be used automatically by the class
		float* readData() const;
		// write to the buffer data in the GPU
		// DO NOT USE: This is meanto to be used automatically by the class
		void writeData(float* data, int size);
		// bind the buffer
		inline void bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
		// unbind the buffer
		inline void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
		// returns the number of vertices
		inline int getNumVertices() { return verticesNumber; }
		// render the vertices
		void render(uint drawMode);
		// bndr::VertexBuffer::~VertexBuffer()
		// Description: Uses glDeleteBuffers to clear the buffer from graphics memory
		~VertexBuffer() { glDeleteBuffers(1, &bufferID); }
	};
}

