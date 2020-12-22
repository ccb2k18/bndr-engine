#pragma once

#include <pch.h>

namespace bndr {

	enum vertexBufferParams {


	};


	class VertexBuffer {

		uint bufferID;
	public:

		VertexBuffer(std::vector<float>& vecRefData);
		VertexBuffer(std::vector<float>&& tempVecData);
	};
}

