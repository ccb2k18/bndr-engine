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

#include <pch.h>
#include "textures.h"

namespace bndr {

	// define the static map
	std::unordered_map<const char*, uint> Texture::textureIDs;

	Texture::Texture(const char* bitMapFile, uint textureSWrapping, uint textureTWrapping, uint textureMinFiltering,
		uint textureMagFiltering) {
	
		// if the texture already exists
		if (Texture::textureIDs.find(bitMapFile) != Texture::textureIDs.end()) {

			std::cout << "already exists!\n";
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
		BitMapData imageData = Texture::loadbitMap(bitMapFile);
		// create the 2D image and its respective mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageData.width, imageData.height,
			0, GL_BGR, GL_UNSIGNED_BYTE, imageData.ptr.get());
		glGenerateMipmap(GL_TEXTURE_2D);

		// finally add the texture id to the unordered map so we don't load it again next time
		std::cout << "added new texture!\n";
		Texture::textureIDs.insert(std::make_pair(bitMapFile, textureID));

		// unbind the texture before returning
		unbind();
	}
}
