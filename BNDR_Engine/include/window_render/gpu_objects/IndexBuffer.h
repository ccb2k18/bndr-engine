#pragma once
#include <pch.h>

namespace bndr {

	class BNDR_API IndexBuffer {

		uint bufferID;
		uint size;

	public:

		IndexBuffer(std::vector<uint>&& indexData);
		// the copy constructor is not allowed
		IndexBuffer(const IndexBuffer&) = delete;
		// the move constructor is not allowed
		IndexBuffer(IndexBuffer&&) = delete;
		// bind the buffer
		inline void bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID); }
		// unbind the buffer
		inline void unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
		// returns the number of indices
		inline uint getSize() { return size; }
		// render the vertices using indices
		void render(uint drawMode);
		// bndr::IndexBuffer::~IndexBuffer()
		// Description: Uses glDeleteBuffers to clear the buffer from graphics memory
		~IndexBuffer() { glDeleteBuffers(1, &bufferID); }
	};
}

