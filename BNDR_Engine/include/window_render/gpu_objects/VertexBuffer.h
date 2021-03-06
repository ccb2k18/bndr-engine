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

		POSITIONS_ATTRIB = 0x01,
		RGBA_COLOR_ATTRIB = 0x02,
		VERTEX_NORMALS_ATTRIB = 0x04,
		TEXTURE_COORDS_ATTRIB = 0x08,
		TEXTURE_INDEX_ATTRIB = 0x10,
		INTERLEAVED_ATTRIBS = 0x40000000
	};

	// bndr::VertexBuffer
	// Description: The VertexBuffer class is used as an abstraction above OpenGL vertex buffers. All OpenGL operations are
	// meant to be done only by the class so that the programmer can just use the BNDR API.
	class VertexBuffer {

		uint bufferID;
		// the number of vertices we want to draw
		int verticesNumber;
		// floats per vertex block
		int floatsPerBlock;
		// store the flags for copying or moving
		uint vbFlags;

		static void saveInterleavedAttrib(uint flags, uint targetFlag, uint& attribIndex, int& offset, int numParams, int dataBlockBytes) {

			if (flags & targetFlag) {

				// enable the attribute
				GL_DEBUG_FUNC(glEnableVertexAttribArray(attribIndex));
				GL_DEBUG_FUNC(glVertexAttribPointer(attribIndex, numParams, GL_FLOAT, GL_FALSE, dataBlockBytes, (void*)offset));
				// add to offset
				offset += numParams * sizeof(float);
				// increment the attrib index
				attribIndex++;
			}
		}

		static void interleafVertexAttribs(int dataBlockBytes, uint flags) {

			uint attribIndex = (uint)0;
			int offset = 0;
			
			saveInterleavedAttrib(flags, POSITIONS_ATTRIB, attribIndex, offset, 3, dataBlockBytes);
			saveInterleavedAttrib(flags, RGBA_COLOR_ATTRIB, attribIndex, offset, 4, dataBlockBytes);
			saveInterleavedAttrib(flags, VERTEX_NORMALS_ATTRIB, attribIndex, offset, 3, dataBlockBytes);
			saveInterleavedAttrib(flags, TEXTURE_COORDS_ATTRIB, attribIndex, offset, 2, dataBlockBytes);
			saveInterleavedAttrib(flags, TEXTURE_INDEX_ATTRIB, attribIndex, offset, 1, dataBlockBytes);
		}

		static void saveBatchedAttrib(uint& attribIndex, int& offset, int numParams, float* data, int dataSize, uint& flags, uint flagTarget) {

			if (dataSize > 0) {

				// update flags value
				flags = flags | flagTarget;

				GL_DEBUG_FUNC(glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, data));
				GL_DEBUG_FUNC(glEnableVertexAttribArray(attribIndex));
				GL_DEBUG_FUNC(glVertexAttribPointer(attribIndex, numParams, GL_FLOAT, GL_FALSE, numParams * sizeof(float), (void*)offset));

				attribIndex++;
				offset += (sizeof(float) * dataSize);
			}
		};

		static void batchVertexAttribs(std::vector<float>& positions, std::vector<float>& colors, std::vector<float>& normals,
			std::vector<float>& textureCoords, std::vector<float>& textureIndices, uint& flags) {

			uint attribIndex = (uint)0;
			int offset = 0;

			saveBatchedAttrib(attribIndex, offset, 3, &positions[0], positions.size(), flags, POSITIONS_ATTRIB);
			saveBatchedAttrib(attribIndex, offset, 4, &colors[0], colors.size(), flags, RGBA_COLOR_ATTRIB);
			saveBatchedAttrib(attribIndex, offset, 3, &normals[0], normals.size(), flags, VERTEX_NORMALS_ATTRIB);
			saveBatchedAttrib(attribIndex, offset, 2, &textureCoords[0], textureCoords.size(), flags, TEXTURE_COORDS_ATTRIB);
			saveBatchedAttrib(attribIndex, offset, 1, &textureIndices[0], textureIndices.size(), flags, TEXTURE_INDEX_ATTRIB);
		}

		static void batchVertexAttribsFromPointers(float* positions, int positionsSize, float* colors = nullptr, int colorsSize = 0,
			float* normals = nullptr, int normalsSize = 0, float* textureCoords = nullptr, int textureCoordsSize = 0,
			float* textureIndices = nullptr, int textureIndicesSize = 0) {


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
		// bndr::VertexBuffer::VertexBuffer
		// Description: this constructor batches the vertex attributes into blocks instead of interleaving them
		VertexBuffer(std::vector<float>&& positions, std::vector<float>&& colors = {}, std::vector<float>&& normals = {},
			std::vector<float>&& textureCoords = {}, std::vector<float>&& textureIndices = {});
		// the copy constructor
		VertexBuffer(const VertexBuffer&);
		// the move constructor is not allowed
		VertexBuffer(VertexBuffer&&) = delete;
		// assignment operator is not allowed
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		// read from the buffer data in the GPU
		std::unique_ptr<float> readData() const;
		// write to the buffer data in the GPU
		// DO NOT USE: This is meanto to be used automatically by the class
		void writeData(float* data);
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

