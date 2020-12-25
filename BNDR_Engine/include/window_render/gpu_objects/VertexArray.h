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
		QUADS = GL_QUADS
	};

	class BNDR_API VertexArray {
	
		// unique id for OpenGL
		uint arrayID;
		// mode to draw the vertices in
		uint drawMode;

		// flags for the VertexArray
		uint vertexArrayFlags;

		// VertexBuffer allocated on the heap
		VertexBuffer* vBuffer = nullptr;
		// IndexBuffer allocated on the heap
		IndexBuffer* iBuffer = nullptr;

	public:
		// bndr::VertexArray::VertexArray
		// Description: render the vertex array using the indices to the vertices
		VertexArray(uint drawingMode, VertexBuffer* vertices, IndexBuffer* indices);
		// bndr::VertexArray::VertexArray
		// Description: render the vertex array using just the vertices
		VertexArray(uint drawingMode, VertexBuffer* vertices);
		// the copy constructor is not allowed
		VertexArray(const VertexArray&) = delete;
		// the move constructor is not allowed
		VertexArray(VertexArray&&) = delete;
		// render the vertex array
		void render();
		// bind the vertex array
		inline void bind() { glBindVertexArray(arrayID); }
		// unbind the vertex array
		inline void unbind() { glBindVertexArray(0); }
		// bndr::VertexArray::~VertexArray
		// Description: Delete the VertexBuffer and IndexBuffer, as well as any textures
		~VertexArray();
	};
}

