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
#include "../../data_structures/matrices.h"
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
		// shader source code
		std::string shaderData;
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
		// get the shader source code c string
		inline const char* getShaderSource() { return shaderData.c_str(); }
		// get length of shader source code
		inline int getShaderLength() { return shaderData.size(); }

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
		// make sure we do not have duplicate programs as well as store static template programs in the map
		static std::unordered_map<const char*, uint> programMap;
	public:

		Program() : programID(0) {}
		// bndr::Program::Program
		// Arguments:
		//        vertexShader = The compiled vertex shader to link with the program
		//        fragmentShader = The compiled fragment shader to link with the program
		Program(Shader&& vertexShader, Shader&& fragmentShader);
		Program(const Program& program);
		Program& operator=(const Program& program) { programID = program.programID; return *this; }
		inline uint getID() { return programID; }
		// use the program
		inline void use() { glUseProgram(programID); }
		// stop using the program
		inline void unuse() { glUseProgram(0); }
		// modify a uniform value that whose primitive attribute(s) is/are of type float
		void setFloatUniformValue(const char* uniformName, float* data, uint dataType);
		// modify a uniform value that is an array of type float
		void setFloatArrayUniformValue(const char* uniformName, float* data, int arraySize);
		// modify a uniform value that is an array of type int
		void setIntArrayUniformValue(const char* uniformName, int* data, int arraySize);
		// modify a uniform value that is a single int
		void setIntUniformValue(const char* uniformName, int value);
		// deletes the OpenGL program
		~Program();

		// program templates

		// this template is meant to be used for polygons of one single color
		static Program defaultPolygonProgram() {

			std::string vert = "# version 330 core\nlayout (location = 0) in vec3 position;\nuniform mat3 translation;\nuniform mat3 rotation;\nuniform mat3 scale;\nuniform vec4 color;\nout vec4 fragColor;\nvoid main() {\nvec3 newPos = translation * rotation * scale * position;\nnewPos.z = 0.0;\ngl_Position = vec4(newPos, 1.0);\nfragColor = color;\n}\0";
			std::string frag = "# version 330 core\nin vec4 fragColor;\nout vec4 finalColor;\nvoid main() {\nfinalColor = fragColor;\n}\0";
			return Program::generateProgramFromSource(vert, frag);
		}

		// this template is for drawing polygons with multiple blended colors for each vertex
		static Program multiColorPolygonProgram() {

			std::string vert = "# version 330 core\nlayout (location = 0) in vec3 position;\nlayout (location = 1) in vec4 color;\nuniform mat3 translation;\nuniform mat3 rotation;\nuniform mat3 scale;\nout vec4 fragColor;\nvoid main() {\nvec3 newPos = translation * rotation * scale * position;\ngl_Position = vec4(newPos, 1.0);\nfragColor = color;\n}\0";
			std::string frag = "# version 330 core\nin vec4 fragColor;\nout vec4 finalColor;\nvoid main() {\nfinalColor = fragColor;\n}\0";
			return Program::generateProgramFromSource(vert, frag);
		}
	private:

		// retrieve existing program with program map key
		// this constructor is private because it is only allowed to be called when the key is valid (see generateFromProgramSource(...))
		Program(const char* programMapKey);

		// generates a map key for a given vertex and fragment shader
		static std::string generateMapKey(Shader& vertexShader, Shader& fragmentShader) {

			const char* vShaderSource = vertexShader.getShaderSource();
			const char* fShaderSource = fragmentShader.getShaderSource();

			return generateMapKey(vShaderSource, fShaderSource, vertexShader.getShaderLength(), fragmentShader.getShaderLength());
		}

		// generates a map key for a given vertex and fragment shader source and char lengths
		static std::string generateMapKey(const char* vertexShader, const char* fragmentShader, int vertexShaderLen, int fragmentShaderLen) {

			// we strategically pick a key that is almost certainly guaranteed to be unique
			// in order to avoid overwritting issues

			// first we create the string for the map key
			char key[11];
			key[10] = '\0';
			std::string mapKey(key);

			// then for each shader pick five characters from it
			int vShaderIncrement = (vertexShaderLen) / 5;
			int fShaderIncrement = (fragmentShaderLen) / 5;

			const char* vShaderSource = vertexShader;
			const char* fShaderSource = fragmentShader;

			for (int i = 0; i < 5; i++) {

				// skip the first 32 ascii characters to avoid overwriting buffer
				mapKey[i] = (char)((((int)vShaderSource[((i + 1) * vShaderIncrement)] + (int)vShaderSource[i * vShaderIncrement]) % 95) + 32);
			}
			for (int i = 6; i < 11; i++) {

				mapKey[i - 1] = (char)(((int)fShaderSource[((i - 5) * fShaderIncrement)] + (int)fShaderSource[(i - 6) * fShaderIncrement]) % 95) + 32;
			}
			return mapKey;
		}

		static bool programExists(const char* mapKey) {

			// check if program already exists
			if (Program::programMap.find(mapKey) != Program::programMap.end()) {

				return true;
			}
			return false;
		}

		static Program generateProgramFromSource(std::string& vShaderSource, std::string& fShaderSource) {

			std::string programKey = Program::generateMapKey(vShaderSource.c_str(), fShaderSource.c_str(), vShaderSource.size(), fShaderSource.size());
			if (Program::programExists(programKey.c_str())) {

				return Program(programKey.c_str());
			}
			return Program(Shader(VERTEX_SHADER, vShaderSource.c_str()), Shader(FRAGMENT_SHADER, fShaderSource.c_str()));
		}
	};
}


