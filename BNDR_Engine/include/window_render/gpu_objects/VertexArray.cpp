#include "pch.h"
#include "VertexArray.h"

namespace bndr {

	VertexArray::VertexArray(uint drawingMode, VertexBuffer* vertices, IndexBuffer* indices) {

		drawMode = drawingMode;
		glGenVertexArrays(1, &arrayID);
		bind();

		// assign the pointers
		vBuffer = vertices;
		iBuffer = indices;

		// bind them to the vertex array
		vBuffer->bind();
		iBuffer->bind();

		unbind();
	}

	VertexArray::VertexArray(uint drawingMode, VertexBuffer* vertices) {

		drawMode = drawingMode;
		GL_DEBUG_FUNC(glGenVertexArrays(1, &arrayID));
		bind();

		vBuffer = vertices;
		vBuffer->bind();

		unbind();
	}

	void VertexArray::render() {

		// if the wants to use an IndexBuffer then render using the IndexBuffer
		if (iBuffer != nullptr) {

			iBuffer->render(drawMode);
		}
		// otherwise use the VertexBuffer instead since the instance was constructed without an index buffer
		else {

			vBuffer->render(drawMode);
		}
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
