#include "pch.h"
#include "IndexBuffer.h"

namespace bndr {

	IndexBuffer::IndexBuffer(std::vector<uint>&& indexData, uint flags) {

		glGenBuffers(1, &bufferID);
		bind();
		size = indexData.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * size, &indexData[0], GL_DYNAMIC_DRAW);
		unbind();
	}

	void IndexBuffer::render(uint drawMode) {

		bind();
		glDrawElements(drawMode, size, GL_UNSIGNED_INT, NULL);
		unbind();
	}
}
