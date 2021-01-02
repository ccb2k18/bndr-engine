/*MIT License

Copyright (c) 2020 Caleb Christopher Bender

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
#include "VertexArray.h"

namespace bndr {

	enum textureEnumFlags {

		TEXTURE_REPEAT = GL_REPEAT,
		TEXTURE_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		TEXTURE_LINEAR = GL_LINEAR,
		TEXTURE_NEAREST = GL_NEAREST
	};

	struct BitMapData {

		std::unique_ptr<uchar> ptr;
		uint width;
		uint height;
	};

	// forward declare texture array
	class TextureArray;

	class BNDR_API Texture {

		// the slot the texture goes in (default is 0 but is set automatically when using a TextureArray)
		uint textureSlot = 0x84C0;
		uint textureID;
		// store all the texture ids so we do not have any duplicate textures in video memory
		static std::unordered_map<const char*, uint> textureIDs;

		constexpr void updateTextureSlot(uint slot) { textureSlot = slot; }
	public:

		Texture(const char* bitMapFile, uint textureSWrapping = TEXTURE_REPEAT,
			uint textureTWrapping = TEXTURE_REPEAT, uint textureMinFiltering = TEXTURE_NEAREST,
			uint textureMagFiltering = TEXTURE_NEAREST);
		// bind the texture
		inline void bind() { glBindTexture(GL_TEXTURE_2D, textureID); }
		// unbind the texture
		inline void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
		// load a bitmap file into memory
		static BitMapData loadbitMap(const char* bitMapFile) {

			std::ifstream bitMapBuffer(bitMapFile, std::ios::in | std::ios::binary);
			// check if the file opened successfully
			if (!bitMapBuffer.is_open()) {

				std::string message = "Failed to open bit map file " + std::string("'") + std::string(bitMapFile) + std::string("'");
				BNDR_EXCEPTION(message.c_str());
			}
			// read the header of the bitmap
			uchar header[54];
			bitMapBuffer.read(static_cast<char*>(static_cast<void*>(&header[0])), 54);

			// check if the bitmap file starts with B and M
			if (header[0] != (uchar)'B' || header[1] != (uchar)'M') {

				std::string message = "The bitmap file " + std::string("'") + std::string(bitMapFile) + std::string("'") + " has an invalid file format";
				BNDR_EXCEPTION(message.c_str());
			}
			// load the data offset, size, width, and height;
			uint dataOffset = (uint)header[0x0A];
			uint size = (uint)header[0x0E];
			uint width = (uint)header[0x12];
			uint height = (uint)header[0x16];

			// if there is missing information then guess what it is
			if (dataOffset == 0) { dataOffset = 54; }
			if (size == 0) { size = width * height * 3; }
			// if either the width or height is 0 raise an error
			if (width == 0 || height == 0) { BNDR_EXCEPTION("Cannot read from a bitmap with a dimension of 0 pixels"); }

			// create buffer for the image data
			std::unique_ptr<uchar> imageData(new uchar[size]);
			// now read the remaining image data
			bitMapBuffer.read(static_cast<char*>(static_cast<void*>(imageData.get())), size);
			bitMapBuffer.close();
			return { std::move(imageData), width, height };

		}
	};

	class TextureArray {

		Texture* textures;
		int size;
		static int maxTextureSlots;
	};

}

