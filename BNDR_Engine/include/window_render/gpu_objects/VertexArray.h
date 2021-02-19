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
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace bndr {

	// various flags to set for the VertexArray class for customization

	// One can draw triangles, lines, or quadrilaterals
	enum vertexDrawingModes {

		TRIANGLES = GL_TRIANGLES,
		LINES = GL_LINES,
	};

	class BNDR_API VertexArray {
	
		// unique id for OpenGL
		uint arrayID;
		// mode to draw the vertices in
		uint drawMode;

		// flags for the VertexArray
		// uint vertexArrayFlags;

		// VertexBuffer allocated on the heap
		VertexBuffer* vBuffer = nullptr;
		// IndexBuffer allocated on the heap
		IndexBuffer* iBuffer = nullptr;

		// used for copy constructor of VertexArray
		inline const VertexBuffer* getVBuffer() const { return vBuffer; }
		inline const IndexBuffer* getIBuffer() const { return iBuffer; }

	public:

		// bndr::VertexArray::VertexArray
		// Description: render the vertex array using the indices to the vertices
		VertexArray(uint drawingMode, std::vector<float>&& vertexData, int dataBlockBytes, uint vertexBufferFlags, std::vector<uint>&& indexData);
		// bndr::VertexArray::VertexArray
		// Description: render the vertex array using just the vertices
		VertexArray(uint drawingMode, std::vector<float>&& vertexData, int dataBlockBytes, uint vertexBufferFlags);
		// the copy constructor
		VertexArray(const VertexArray&);
		// the move constructor is not allowed
		VertexArray(VertexArray&&) = delete;
		// assignment operator is not allowed
		VertexArray& operator=(const VertexArray&) = delete;
		// update the vertex buffer data
		inline void updateVertexBufferData(float* data) { bind();  vBuffer->writeData(data); unbind(); }
		// render the vertex array
		void render();
		// bind the vertex array
		inline void bind() const { glBindVertexArray(arrayID); }
		// unbind the vertex array
		inline void unbind() const { glBindVertexArray(0); }
		// bndr::VertexArray::~VertexArray
		// Description: Delete the VertexBuffer and IndexBuffer, as well as any textures
		~VertexArray();
	};
}

