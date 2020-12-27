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

namespace bndr {

	enum textureEnumFlags {

		TEXTURE_REPEAT = GL_REPEAT,
		TEXTURE_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		TEXTURE_LINEAR = GL_LINEAR,
		TEXTURE_NEAREST = GL_NEAREST
	};


	class Texture {

		uint textureID;
		// store all the texture ids so we do not have any duplicate textures in video memory
		static std::unordered_map<const char*, uint> textureIDs;
	public:

		Texture(const char* bitMapFile, uint textureSWrapping = TEXTURE_REPEAT,
			uint textureTWrapping = TEXTURE_REPEAT, uint textureMinFiltering = TEXTURE_NEAREST,
			uint textureMagFiltering = TEXTURE_NEAREST);
		// bind the texture
		inline void bind() { glBindTexture(GL_TEXTURE_2D, textureID); }
		// unbind the texture
		inline void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
	};
}

