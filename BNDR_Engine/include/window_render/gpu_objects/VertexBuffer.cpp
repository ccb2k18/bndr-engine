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

#include <pch.h>
#include "VertexBuffer.h"

namespace bndr {


	VertexBuffer::VertexBuffer(std::vector<float>&& vertexData, int dataBlockBytes, uint flags) {

		// generate and bind the buffer
		GL_DEBUG_FUNC(glGenBuffers(1, &bufferID));
		bind();
		GL_DEBUG_FUNC(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), &vertexData[0], GL_DYNAMIC_DRAW));

		// the vertices number is equal to the size of the entire data divided by the size of each data block
		verticesNumber = (sizeof(float) * vertexData.size()) / dataBlockBytes;
		floatsPerBlock = dataBlockBytes / 4;
		// we are interleaving the attributes so save that in the flags
		vbFlags = flags | INTERLEAVED_ATTRIBS | POSITIONS_ATTRIB;

		VertexBuffer::interleafVertexAttribs(dataBlockBytes, vbFlags);
		unbind();
	}

	VertexBuffer::VertexBuffer(std::vector<float>&& positions, std::vector<float>&& colors, std::vector<float>&& normals,
		std::vector<float>&& textureCoords, std::vector<float>&& textureIndices) {

		// generate and bind the buffer
		GL_DEBUG_FUNC(glGenBuffers(1, &bufferID));
		bind();
		int size = positions.size() + colors.size() + normals.size() + textureCoords.size() + textureIndices.size();
		// allocate block of data equal to the total size of all the data
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

		verticesNumber = size;
		floatsPerBlock = 1;
		vbFlags = 0;
		// batch each attribute data into the buffer
		VertexBuffer::batchVertexAttribs(positions, colors, normals, textureCoords, textureIndices, vbFlags);
		unbind();
	}


	VertexBuffer::VertexBuffer(const VertexBuffer& vb) {

		verticesNumber = vb.verticesNumber;
		floatsPerBlock = vb.floatsPerBlock;
		vbFlags = vb.vbFlags;
		std::unique_ptr<float> vertexData = vb.readData();

		// generate and bind the buffer
		GL_DEBUG_FUNC(glGenBuffers(1, &bufferID));
		bind();

		// if the attributes are interleaved
		if (vbFlags & INTERLEAVED_ATTRIBS) {

			GL_DEBUG_FUNC(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesNumber * floatsPerBlock, (const void*)vertexData.get(), GL_DYNAMIC_DRAW));
			VertexBuffer::interleafVertexAttribs(floatsPerBlock * sizeof(float), vbFlags);
		}
		// otherwise batch the attributes
		else {

			GL_DEBUG_FUNC(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesNumber, (const void*)NULL, GL_DYNAMIC_DRAW));
			/*std::pair<float*, int> attribs[5] = { std::make_pair(nullptr, 0) };
			float blockSizes[5] = { 0, 0, 0, 0, 0 };
			if (vbFlags & POSITIONS_ATTRIB) { blockSizes[0] = 3 * sizeof(float); }
			if (vbFlags & RGBA_COLOR_ATTRIB) { blockSizes[1] = 4 * sizeof(float); }
			if (vbFlags & VERTEX_NORMALS_ATTRIB) { blockSizes[2] = 3 * sizeof(float); }
			if (vbFlags & TEXTURE_COORDS_ATTRIB) { blockSizes[3] = 2 * sizeof(float); }
			if (vbFlags & TEXTURE_INDEX_ATTRIB) { blockSizes[4] = sizeof(float); }
			int blockSizesSum = blockSizes[0] + blockSizes[1] + blockSizes[2] + blockSizes[3] + blockSizes[4];
			uint attribIndex = (uint)0;
			int offset = 0;
			for (int i = 0; i < 5; i++) {

				if (blockSizes[i] != 0) {

					attribs[i].first = (float*)(vertexData.get() + offset);
					attribs->second = 
				}
			}*/
		}
		unbind();

	}

	std::unique_ptr<float> VertexBuffer::readData() const {

		float* data(new float[verticesNumber*floatsPerBlock]);
		bind();
		GL_DEBUG_FUNC(glGetBufferSubData(GL_ARRAY_BUFFER, 0, verticesNumber * floatsPerBlock * sizeof(float), (void*)data));
		unbind();
		return std::unique_ptr<float>(data);
	}

	void VertexBuffer::writeData(float* data) {

		bind();
		//void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		GL_DEBUG_FUNC(glBufferSubData(GL_ARRAY_BUFFER, 0, verticesNumber * floatsPerBlock * sizeof(float), (const void*)data));
		//memcpy(ptr, data, verticesNumber * floatsPerBlock * sizeof(float));
		//GL_DEBUG_FUNC(glUnmapBuffer(GL_ARRAY_BUFFER));
		unbind();
	}

	void VertexBuffer::render(uint drawMode) {

		bind();
		GL_DEBUG_FUNC(glDrawArrays(drawMode, 0, verticesNumber));
		unbind();
	}
}
