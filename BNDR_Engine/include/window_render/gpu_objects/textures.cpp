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
#include "textures.h"

namespace bndr {

	// define the static map
	std::unordered_map<const char*, uint> Texture::textureIDs;
	// define max texture slots
	int TextureArray::maxTextureSlots = 0;

	Texture::Texture(const char* bitMapFile, uint textureSWrapping, uint textureTWrapping, uint textureMinFiltering,
		uint textureMagFiltering) {
	
		// if the texture already exists
		if (Texture::textureIDs.find(bitMapFile) != Texture::textureIDs.end()) {

			BNDR_MESSAGE("texture already exists!\n");
			// assign the id to the already existing texture and return
			textureID = Texture::textureIDs[bitMapFile];
			return;
		}
		// otherwise generate it and get the bitmap data
		glGenTextures(1, &textureID);
		bind();

		// how the texture is wrapped on the surface
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureSWrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureTWrapping);
		// are the texture pixels smooth (TEXTURE_LINEAR) or are they sharp (TEXTURE_NEAREST)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMinFiltering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureMagFiltering);

		// load the bitmap image bits and the width and height
		BitMapData imageData = Texture::loadBitMap(bitMapFile);
		uchar* pixelPtr = imageData.ptr.release();
		// create the 2D image and its respective mipmaps
		GL_DEBUG_FUNC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageData.width, imageData.height,
			0, GL_BGR, GL_UNSIGNED_BYTE, pixelPtr));
		glGenerateMipmap(GL_TEXTURE_2D);

		// delete the data
		delete[] pixelPtr;

		// finally add the texture id to the unordered map so we don't load it again next time
		BNDR_MESSAGE("added new texture!");
		Texture::textureIDs.insert(std::make_pair(bitMapFile, textureID));

		// unbind the texture before returning
		unbind();
	}

	TextureArray::TextureArray(std::initializer_list<Texture>&& textureList) {

		// check if the max number of texture slots has been queried
		if (maxTextureSlots == 0) { 
			
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
			std::string message = "This machine's gpu supports " + std::to_string(maxTextureSlots) + " texture slots";
			BNDR_MESSAGE(message.c_str());
		
		}
		// start at the first slot
		uint slot = GL_TEXTURE0;
		size = textureList.size();
		textures = new Texture[size];

		// overwrite each of the texture ids in our array with the values in the initializer list
		for (int i = 0; i < size; i++) {

			// notice that each texture slot will always be within the max number of slots by using the modulus operator
			textures[i].overwriteData((textureList.begin() + i)->textureID, slot + (i % maxTextureSlots));
		}
	}

	void TextureArray::bindAll() {
	
		for (int i = 0; i < size; i++) {

			textures[i].bind();
		}
	}

	TextureArray::~TextureArray() {

		// you'll notice that when we destroy a texture object, the OpenGL texture never gets destroyed. This may seem like
		// it will cause memory leaks but it won't. With the texture class we ensure that the same texture never gets loaded
		// into video memory more than once. If we actually deleted the texture from graphics memory every time we destroy
		// a texture object, then we would have to reload it all over again instead of just getting the texture id insanely
		// fast. All in all it makes sense to keep the texture in video memory until the program terminates
		delete[] textures;
	}
}
