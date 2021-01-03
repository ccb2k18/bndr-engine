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
#include "GLDebug.h"


namespace bndr {

	enum shaderTypes {

		VERTEX_SHADER = GL_VERTEX_SHADER,
		FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
		GEOMETRY_SHADER = GL_GEOMETRY_SHADER
	};

	// bndr::Shader
	// Description: base class for all shaders in the application
	class BNDR_API Shader {

		uint shaderID;
	public:

		// bndr::Shader::Shader
		// Arguments:
		//        shaderType = the type of shader you are creating (vertex, fragment, or geometry)
		//        shaderSource = If fromFile is true then shaderSource will be interpreted as a file path.
		//        Otherwise it will be interpreted as the actual source code data.
		//        fromFile = Indicates whether the Shader class should compile from file or from source code data
		// Description: This constructor creates and compiles an OpenGL shader to be used in a program
		Shader(uint shaderType, const char* shaderSource, bool fromFile = false);
		// copy constructor is not allowed
		Shader(const Shader&) = delete;
		// move constructor
		Shader(Shader&& shader) noexcept;
		// get the shaderID (read-only)
		inline uint getShaderID() { return shaderID; }
		~Shader();
	};

	enum uniformDataTypes {

		FLOAT = 1,
		VEC2 = 2,
		VEC3 = 3,
		VEC4 = 4,
		MAT2X2 = 5,
		MAT3X3 = 6,
		MAT4x4 = 7
	};

	// bndr::Program
	// Description: class that contains OpenGL program given shaders to compile and link
	class BNDR_API Program {

		uint programID;
	public:

		// bndr::Program::Program
		// Arguments:
		//        vertexShader = The compiled vertex shader to link with the program
		//        fragmentShader = The compiled fragment shader to link with the program
		Program(Shader&& vertexShader, Shader&& fragmentShader);
		// copy constructor is not allowed
		Program(const Program&) = delete;
		// move constructor is not allowed
		Program(Program&&) = delete;
		// use the program
		inline void use() { glUseProgram(programID); }
		// stop using the program
		inline void unuse() { glUseProgram(0); }
		// modify a uniform value that whose primitive attribute(s) is/are of type float
		void setFloatUniformValue(const char* uniformName, float* data, uint dataType);
		// modify a uniform value that is an array of type float
		void setFloatArrayUniformValue(const char* uniformName, float* data, int arraySize);
		// modify a uniform value that is an array of type float
		void setIntUniformValue(const char* uniformName, int* data, int arraySize);
		// deletes the OpenGL program
		~Program();
	};
}


