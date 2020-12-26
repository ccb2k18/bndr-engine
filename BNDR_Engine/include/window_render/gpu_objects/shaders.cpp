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

		glDeleteShader(shaderID);
	}
}
