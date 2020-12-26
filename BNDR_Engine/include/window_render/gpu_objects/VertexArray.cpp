#include "pch.h"
#include "VertexArray.h"

namespace bndr {

	VertexArray::VertexArray(uint drawingMode, std::vector<float>&& vertexData, int dataBlockBytes, uint vertexBufferFlags, std::vector<uint>&& indexData) {

		drawMode = drawingMode;
		glGenVertexArrays(1, &arrayID);
		bind();

		// assign the pointers to new instances of VertexBuffer and IndexBuffer
		vBuffer = new VertexBuffer(std::move(vertexData), dataBlockBytes, vertexBufferFlags);
		iBuffer = new IndexBuffer(std::move(indexData));

		unbind();
	}

	VertexArray::VertexArray(uint drawingMode, std::vector<float>&& vertexData, int dataBlockBytes, uint vertexBufferFlags) {

		drawMode = drawingMode;
		GL_DEBUG_FUNC(glGenVertexArrays(1, &arrayID));
		bind();

		// only create an instance of a VertexBuffer
		vBuffer = new VertexBuffer(std::move(vertexData), dataBlockBytes, vertexBufferFlags);

		unbind();
	}

	void VertexArray::render() {

		bind();
		// if the wants to use an IndexBuffer then render using the IndexBuffer
		if (iBuffer != nullptr) {

			iBuffer->render(drawMode);
		}
		// otherwise use the VertexBuffer instead since the instance was constructed without an index buffer
		else {

			vBuffer->render(drawMode);
		}
		unbind();
	}

	VertexArray::~VertexArray() {

		// delete the vertex buffer
		if (vBuffer != nullptr) {

			delete vBuffer;
		}
		// delete the index buffer
		if (iBuffer != nullptr) {

			delete iBuffer;
		}
		// delete the vertex array
		glDeleteVertexArrays(1, &arrayID);
	}
}
