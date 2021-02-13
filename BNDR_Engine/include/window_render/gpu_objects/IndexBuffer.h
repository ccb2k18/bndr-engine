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

	class BNDR_API IndexBuffer {

		uint bufferID;
		uint size;

	public:

		IndexBuffer(std::vector<uint>&& indexData);
		// the copy constructor
		IndexBuffer(const IndexBuffer&);
		// the move constructor is not allowed
		IndexBuffer(IndexBuffer&&) = delete;
		// assignment operator is not allowed
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		// read from the buffer data in the GPU
		// DO NOT USE: This is meant to be used automatically by the class
		uint* readData() const;
		// bind the buffer
		inline void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID); }
		// unbind the buffer
		inline void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
		// returns the number of indices
		inline uint getSize() { return size; }
		// render the vertices using indices
		void render(uint drawMode);
		// bndr::IndexBuffer::~IndexBuffer()
		// Description: Uses glDeleteBuffers to clear the buffer from graphics memory
		~IndexBuffer() { glDeleteBuffers(1, &bufferID); }
	};
}

