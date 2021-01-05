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
		int width;
		int height;
	};

	// forward declare texture array
	class TextureArray;

	class BNDR_API Texture {

		// the slot the texture goes in (default is 0 but is set automatically when using a TextureArray)
		uint textureSlot = GL_TEXTURE0;
		uint textureID;
		// store all the texture ids so we do not have any duplicate textures in video memory
		static std::unordered_map<const char*, uint> textureIDs;

		constexpr void updateTextureSlot(uint slot) { textureSlot = slot; }

		// only the texture array class can use this method (this method takes a temporary Texture object that is about
		// to go out of scope and transfers its data over to this one)
		constexpr void overwriteData(uint preexistingID, uint slot) { textureID = preexistingID; textureSlot = slot; }

	public:

		// default constructor
		Texture() : textureSlot(GL_TEXTURE0), textureID((uint)0) {}

		Texture(const char* bitMapFile, uint textureSWrapping = TEXTURE_REPEAT,
			uint textureTWrapping = TEXTURE_REPEAT, uint textureMinFiltering = TEXTURE_NEAREST,
			uint textureMagFiltering = TEXTURE_NEAREST);
		// bind the texture
		inline void bind() { glActiveTexture(textureSlot); glBindTexture(GL_TEXTURE_2D, textureID); }
		// unbind the texture
		inline void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
		// get the id of the texture
		inline int getID() { return (int)textureID; }
		// get the slot of the texture
		inline int getSlot() { return (int)textureSlot; }
		// load a bitmap file into memory
		static BitMapData loadBitMap(const char* bitMapFile) {

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
			// get the image size, width, and height
			int size = *(int*)&header[0x22];
			int width = *(int*)&header[0x12];
			int height = *(int*)&header[0x16];

			// if the size is missing then fill it in
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
		// texture array has access to the private method updateTextureSlot
		friend class TextureArray;
	};

	// bndr::TextureArray
	// Description: This class is a system dependent texture container. It is system dependent because depending on your
	// gpu, you have x number of textures that can be bound at one time. The advantage of using a texture array is that it
	// is much faster that a regular texture. This is because each instance of the texture class always uses the same texture
	// slot, while texture array can have 8, 16, or even 32 textures bound at once depending on your gpu.
	class BNDR_API TextureArray {

		// texture array
		Texture* textures = nullptr;
		// size of the array
		int size;
		// gpu-dependent (some gpus have more texture slots than others)
		static int maxTextureSlots;

	public:

		TextureArray(std::initializer_list<Texture>&& textureList);
		// bind all textures
		void bindAll();
		// unbind all textures
		inline void unbindAll() { glBindTexture(GL_TEXTURE_2D, 0); }
		// bind a specific texture
		inline void bindAt(int index) { textures[index].bind(); }
		// get the id at a specific index
		inline int getIDAt(int index) { return textures[index].getID(); }
		// get the slot at a specific index
		inline int getSlotAt(int index) { return textures[index].getSlot(); }
		~TextureArray();
	};

}

