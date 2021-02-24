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
#include "shaders.h"

namespace bndr {

	// define the shader map
	// std::unordered_map<std::string, uint> Program::programMap;
	std::unordered_map<std::string, std::pair<Shader, Shader>> Program::shaderMap;

	Shader::Shader(uint shaderType, const char* shaderSource, bool fromFile) {

		shaderID = glCreateShader(shaderType);
		// if we need to load the data from a file
		if (fromFile) {

			std::ifstream shaderFileBuffer(shaderSource, std::ios::in);
			if (!shaderFileBuffer.is_open()) {

				BNDR_EXCEPTION("Failed to open shader file");
			}
			std::stringstream shaderSourceStream;
			shaderSourceStream << shaderFileBuffer.rdbuf();
			shaderData = shaderSourceStream.str();

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

	Shader::Shader(Shader&& shader) noexcept {

		shaderID = shader.shaderID;
		shaderData = shader.shaderData;
	}


	Program::Program(Shader&& vertexShader, Shader&& fragmentShader) {

		std::string mapKey = Program::generateMapKey(vertexShader, fragmentShader);

		Shader* vShader = &vertexShader;
		Shader* fShader = &fragmentShader;
		// check if program already exists
		if (Program::programExists(mapKey.c_str())) {

			std::pair<Shader,Shader>* pairPtr = &Program::shaderMap[mapKey.c_str()];
			vShader = &pairPtr->first;
			fShader = &pairPtr->second;
			std::string message = "the program with map key " + std::string("\"") + mapKey + std::string("\"") + " already exists\n";
			BNDR_MESSAGE(message.c_str());
			return;
		}

		Program::linkProgram(programID, vShader, fShader);

		// the shaders have reached the end of their scope and will be deleted automatically in
		// the Shader class destructor once the function ends

		// now we must not forget to add the program to the map
		Program::shaderMap.insert(std::make_pair(mapKey.c_str(), std::make_pair(*vShader, *fShader)));
		std::string msg = "Added new program with hash key " + std::string("\"") + mapKey + std::string("\"");
		BNDR_MESSAGE(msg.c_str());
	}

	Program::Program(const Program& program) {

		programID = program.programID;
	}

	Program::Program(const char* mapKey) {

		std::pair<Shader,Shader>* pairPtr = &Program::shaderMap[mapKey];
		Shader* vShader = &pairPtr->first;
		Shader* fShader = &pairPtr->second;

		Program::linkProgram(programID, vShader, fShader);

		std::string message = "the program with map key " + std::string("\"") + mapKey + std::string("\"") + " already exists\n";
		BNDR_MESSAGE(message.c_str());
		return;
	}

	void Program::setFloatUniformValue(const char* uniformName, const float* data, uint dataType) const {

		use();
		try {

			int uniformLocation = glGetUniformLocation(programID, uniformName);
			if (uniformLocation == -1) {
				std::string data = "Cannot locate uniform '" + std::string(uniformName) + "' in shader program";
				BNDR_EXCEPTION(data.c_str());
			}
			switch (dataType) {

			case FLOAT:

				GL_DEBUG_FUNC(glUniform1f(uniformLocation, *data));
				break;
			case VEC2:

				GL_DEBUG_FUNC(glUniform2f(uniformLocation, data[0], data[1]));
				break;
			case VEC3:

				GL_DEBUG_FUNC(glUniform3f(uniformLocation, data[0], data[1], data[2]));
				break;
			case VEC4:

				GL_DEBUG_FUNC(glUniform4f(uniformLocation, data[0], data[1], data[2], data[3]));
				break;
			case MAT2X2:

				GL_DEBUG_FUNC(glUniformMatrix2fv(uniformLocation, 1, GL_TRUE, data));
				break;
			case MAT3X3:

				GL_DEBUG_FUNC(glUniformMatrix3fv(uniformLocation, 1, GL_TRUE, data));
				break;
			case MAT4x4:

				GL_DEBUG_FUNC(glUniformMatrix4fv(uniformLocation, 1, GL_TRUE, data));
				break;
			}
		}
		catch (std::runtime_error& e) {

			std::cout << e.what() << "\n";
		}
		unuse();
	}

	void Program::setFloatArrayUniformValue(const char* uniformName, const float* data, int arraySize) const {

		use();
		try {

			int uniformLocation = glGetUniformLocation(programID, uniformName);
			if (uniformLocation == -1) {
				std::string data = "Cannot locate uniform '" + std::string(uniformName) + "' in shader program";
				BNDR_EXCEPTION(data.c_str());
			}
			GL_DEBUG_FUNC(glUniform1fv(uniformLocation, arraySize, data));
		}
		catch (std::runtime_error& e) {

			std::cout << e.what();
		}
		unuse();
	}

	void Program::setIntArrayUniformValue(const char* uniformName, const int* data, int arraySize) const {

		use();
		try {

			int uniformLocation = glGetUniformLocation(programID, uniformName);
			if (uniformLocation == -1) {
				std::string data = "Cannot locate uniform '" + std::string(uniformName) + "' in shader program";
				BNDR_EXCEPTION(data.c_str());
			}
			GL_DEBUG_FUNC(glUniform1iv(uniformLocation, arraySize, data));
		}
		catch (std::runtime_error& e) {

			std::cout << e.what();
		}
		unuse();
	}

	void Program::setIntUniformValue(const char* uniformName, int value) {

		use();
		try {

			int uniformLocation = glGetUniformLocation(programID, uniformName);
			if (uniformLocation == -1) {
				std::string data = "Cannot locate uniform '" + std::string(uniformName) + "' in shader program";
				BNDR_EXCEPTION(data.c_str());
			}
			GL_DEBUG_FUNC(glUniform1i(uniformLocation, value));
		}
		catch (std::runtime_error& e) {

			std::cout << e.what();
		}
		unuse();

	}

	Program::~Program() {

		glDeleteProgram(programID);
	}
}
