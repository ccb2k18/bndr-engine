#include "pch.h"
#include "VertexArray.h"

namespace bndr {

	VertexArray::VertexArray(uint drawingMode, VertexBuffer* vertices, IndexBuffer* indices) {

		// set the draw with indices flag
		vertexArrayFlags = vertexArrayFlags | RENDER_WITH_INDICES;
	}

	VertexArray::VertexArray(uint drawingMode, VertexBuffer* vertices) {

		// by not setting draw with indices flag we will automatically draw with the vertices
	}

	VertexArray::~VertexArray() {

		if (vBuffer != nullptr) {

			delete vBuffer;
		}
		if (iBuffer != nullptr) {

			delete iBuffer;
		}
	}
}
