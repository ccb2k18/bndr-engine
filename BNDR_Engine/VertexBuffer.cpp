#include <pch.h>
#include "VertexBuffer.h"

namespace bndr {

	VertexBuffer::VertexBuffer(std::vector<float>& vecRefData) {

		// generate and bind the buffer
		glGenBuffers(1, &bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	}

	VertexBuffer::VertexBuffer(std::vector<float>&& tempVecData) {

		// generate and bind the buffer
		glGenBuffers(1, &bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	}
}
