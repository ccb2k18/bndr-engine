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
#include "shaders.h"

namespace bndr {

	Shader::Shader(uint shaderType, const char* shaderSource, bool fromFile) {

		shaderID = glCreateShader(shaderType);
		std::string shaderData;
		// if we need to load the data from a file
		if (fromFile) {

			std::ifstream shaderFileBuffer(shaderSource, std::ios::in);
			if (!shaderFileBuffer.is_open()) {

				BNDR_EXCEPTION("Failed to open shader file");
			}
			std::stringstream shaderSourceStream;
			shaderSourceStream << shaderFileBuffer.rdbuf();
			shaderData = shaderSourceStream.str();
			/*
			// get the end of the file
			shaderFileBuffer.seekg(0, shaderFileBuffer.end);
			int fileLength = shaderFileBuffer.tellg();
			// go back to the beginning
			shaderFileBuffer.seekg(0, shaderFileBuffer.beg);
			// reallocate the memory block for the string
			shaderData.reserve(fileLength);
			// read the data from the file
			shaderFileBuffer.read(&shaderData[0], fileLength);
			// close the file
			shaderFileBuffer.close();
			*/

		}
		// if we aren't reading from a file, then the const char pointer is the actual source code
		else {

			shaderData = shaderSource;
		}

		const char* shaderSourceCode = shaderData.c_str();
		// give the source code to the OpenGL shader
		GL_DEBUG_FUNC(glShaderSource(shaderID, 1, &shaderSourceCode, NULL));

		// now it's time to compile the shader
		glCompileShader(shaderID);

		int infoLogLen;
		// check for compilation errors
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLen);
		if (infoLogLen > 0) {

			char infoLogBuffer[250];
			glGetShaderInfoLog(shaderID, 250, &infoLogLen, infoLogBuffer);
			// raise an exception with the shader compilation error
			BNDR_EXCEPTION(infoLogBuffer);
		}
	}

	Shader::~Shader() {

		std::cout << "Shader deleted!\n";
		glDeleteShader(shaderID);
	}


	Program::Program(Shader&& vertexShader, Shader&& fragmentShader) {

		// create the program and attach the shaders
		programID = glCreateProgram();
		glAttachShader(programID, vertexShader.getShaderID());
		glAttachShader(programID, fragmentShader.getShaderID());

		// link the program
		glLinkProgram(programID);

		// check for link errors
		int infoLogLen;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLen);

		if (infoLogLen > 0) {

			char infoLogBuffer[250];
			glGetProgramInfoLog(programID, 250, &infoLogLen, infoLogBuffer);
			// raise an exception with the program link error
			BNDR_EXCEPTION(infoLogBuffer);
		}

		// detach the shaders
		glDetachShader(programID, vertexShader.getShaderID());
		glDetachShader(programID, fragmentShader.getShaderID());

		// the shaders have reached the end of their scope and will be deleted automatically once the function ends
	}

	Program::~Program() {

		glDeleteProgram(programID);
	}
}
