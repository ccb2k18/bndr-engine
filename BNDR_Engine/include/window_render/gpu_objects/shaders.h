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
	class Shader {

		uint shaderID;
		// shader source code
		std::string shaderData;

	public:

		Shader() : shaderID((uint)0), shaderData("") {}
		// bndr::Shader::Shader
		// Arguments:
		//        shaderType = the type of shader you are creating (vertex, fragment, or geometry)
		//        shaderSource = If fromFile is true then shaderSource will be interpreted as a file path.
		//        Otherwise it will be interpreted as the actual source code data.
		//        fromFile = Indicates whether the Shader class should compile from file or from source code data
		// Description: This constructor creates and compiles an OpenGL shader to be used in a program
		Shader(uint shaderType, const char* shaderSource, bool fromFile = false);
		// copy constructor
		Shader(const Shader& shader) { shaderID = shader.shaderID; shaderData = shader.shaderData; }

		inline Shader& operator=(const Shader& shader) { shaderID = shader.shaderID; shaderData = shader.shaderData; return (*this); }
		// move constructor
		Shader(Shader&& shader) noexcept;
		// get the shaderID (read-only)
		inline uint getShaderID() { return shaderID; }
		// get the shader source code c string
		inline const char* getShaderSource() { return shaderData.c_str(); }
		// get length of shader source code
		inline int getShaderLength() { return shaderData.size(); }
		~Shader() { BNDR_MESSAGE("Shader Deleted!"); }
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
	class Program {

		uint programID;
		// make sure we do not have duplicate programs as well as store static template programs in the map

		// map of previously created shaders so that shaders are not created more than once for new programs
		static std::unordered_map<std::string, std::pair<Shader, Shader>> shaderMap;

		// save the key so when we copy the program it generates the equivalent program quickly from the
		// respective shaders
		std::string programKey;
	public:

		Program() : programID(0) {}
		// bndr::Program::Program
		// Arguments:
		//        vertexShader = The compiled vertex shader to link with the program
		//        fragmentShader = The compiled fragment shader to link with the program
		Program(Shader&& vertexShader, Shader&& fragmentShader);
		Program(const Program& program);
		Program(Program&& program) = delete;
		Program& operator=(const Program& program) = delete;
		inline uint getID() { return programID; }
		// use the program
		inline void use() const { glUseProgram(programID); }
		// stop using the program
		inline void unuse() const { glUseProgram(0); }
		// modify a uniform value that whose primitive attribute(s) is/are of type float
		void setFloatUniformValue(const char* uniformName, const float* data, uint dataType) const;
		// modify a uniform value that is an array of type float
		void setFloatArrayUniformValue(const char* uniformName, const float* data, int arraySize) const;
		// modify a uniform value that is an array of type int
		void setIntArrayUniformValue(const char* uniformName, const int* data, int arraySize) const;
		// modify a uniform value that is a single int
		void setIntUniformValue(const char* uniformName, int value);
		// deletes the OpenGL program
		~Program();

		static void linkProgram(uint& programID, Shader* vShader, Shader* fShader) {

			// create the program and attach the shaders
			programID = glCreateProgram();
			glAttachShader(programID, vShader->getShaderID());
			glAttachShader(programID, fShader->getShaderID());

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
			glDetachShader(programID, vShader->getShaderID());
			glDetachShader(programID, fShader->getShaderID());
		}

		// program templates

		// this template is meant to be used for polygons of one single color
		static Program* defaultPolygonProgram() {

			std::string vert = "# version 330 core\n"
				"layout (location = 0) in vec3 position;\n"
				"uniform vec2 center;\n"
				"uniform vec3 translation;\n"
				"uniform vec3 rotation;\n"
				"uniform vec3 scale;\n"
				"uniform vec4 color;\n"
				"out vec4 fragColor;\n"
				"void main() {\n"
				"vec3 rotCenter = vec3(center, 0.0);\n"
				"vec3 newRot = vec3(rotCenter.x * rotation.x - rotCenter.y * rotation.y, rotCenter.y * rotation.x + rotCenter.x * rotation.y, 0.0);\n"
				"vec3 newPos = position;\n"
				"newPos.x *= scale.x;\n"
				"newPos.y *= scale.y;\n"
				"newPos += -rotCenter + newRot + rotCenter + translation;\n"
				"newPos.z = 0.0;\n"
				"gl_Position = vec4(newPos, 1.0);\n"
				"fragColor = color;\n"
				"}\0";
			std::string frag = "# version 330 core\n"
				"in vec4 fragColor;\n"
				"out vec4 finalColor;\n"
				"void main() {\n"
				"finalColor = fragColor;\n"
				"}\0";
			return Program::generateProgramFromSource(vert, frag);
		}

		// this template is for drawing polygons with multiple blended colors for each vertex
		static Program* multiColorPolygonProgram() {

			std::string vert = "# version 330 core\n"
				"layout (location = 0) in vec3 position;\n"
				"layout (location = 1) in vec4 color;\n"
				"uniform vec2 center;\n"
				"uniform vec3 translation;\n"
				"uniform vec3 rotation;\n"
				"uniform vec3 scale;\n"
				"out vec4 fragColor;\n"
				"void main() {\n"
				"vec3 rotCenter = vec3(center, 0.0);\n"
				"vec3 newRot = vec3(rotCenter.x * rotation.x - rotCenter.y * rotation.y, rotCenter.y * rotation.x + rotCenter.x * rotation.y, 0.0);\n"
				"vec3 newPos = position;\n"
				"newPos.x *= scale.x;\n"
				"newPos.y *= scale.y;\n"
				"newPos += -rotCenter + newRot + rotCenter + translation;\n"
				"newPos.z = 0.0;\n"
				"gl_Position = vec4(newPos, 1.0);\n"
				"fragColor = color;\n"
				"}\0";
			std::string frag = "# version 330 core\n"
				"in vec4 fragColor;\n"
				"out vec4 finalColor;\n"
				"void main() {\n"
				"finalColor = fragColor;\n"
				"}\0";
			return Program::generateProgramFromSource(vert, frag);
		}

		// this template is meant for textured rects or triangles with only one texture
		static Program* texPolygonProgram(int numTexes) {

			std::string vert = "# version 330 core\n"
				"layout (location = 0) in vec3 position;\n"
				"layout (location = 1) in vec4 color;\n"
				"layout (location = 2) in vec2 texCoords;\n"
				"uniform vec2 center;\n"
				"uniform vec3 translation;\n"
				"uniform vec3 rotation;\n"
				"uniform vec3 scale;\n"
				"out vec4 fragColor;\n"
				"out vec2 fragTexCoords;\n"
				"void main() {\n"
				"vec3 rotCenter = vec3(center, 2.0);\n"
				"vec3 newRot = vec3(rotCenter.x * rotation.x - rotCenter.y * rotation.y, rotCenter.y * rotation.x + rotCenter.x * rotation.y, 0.0);\n"
				"vec3 newPos = position;\n"
				"newPos.x *= scale.x;\n"
				"newPos.y *= scale.y;\n"
				"newPos += -rotCenter + newRot + rotCenter + translation;\n"
				"newPos.z = 0.0;\n"
				"gl_Position = vec4(newPos, 1.0);\n"
				"fragColor = color;\n"
				"fragTexCoords = texCoords;\n"
				"}\0";
			std::string frag;
			switch (numTexes) {

			default:

				frag = "# version 330 core\n"
					"in vec4 fragColor;\n"
					"in vec2 fragTexCoords;\n"
					"uniform sampler2D tex0;\n"
					"out vec4 finalColor;\n"
					"void main(){\n"
					"finalColor = texture(tex0, fragTexCoords) * fragColor;\n"
					"}\0";
				break;
			case 2:

				frag = "# version 330 core\n"
					"in vec4 fragColor;\n"
					"in vec2 fragTexCoords;\n"
					"uniform sampler2D tex0;\n"
					"uniform sampler2D tex1;\n"
					"uniform float nestedTexAlphaWeight;\n"
					"out vec4 finalColor;\n"
					"void main(){\n"
					"finalColor = mix(texture(tex0, fragTexCoords),texture(tex1, fragTexCoords), nestedTexAlphaWeight) * fragColor;\n"
					"}\0";
				break;
			case 3:

				frag = "# version 330 core\n"
					"in vec4 fragColor;\n"
					"in vec2 fragTexCoords;\n"
					"uniform sampler2D tex0;\n"
					"uniform sampler2D tex1;\n"
					"uniform sampler2D tex2;\n"
					"uniform float nestedTexAlphaWeight;\n"
					"uniform float outerTexAlphaWeight;\n"
					"out vec4 finalColor;\n"
					"void main(){\n"
					"finalColor = mix(mix(texture(tex0, fragTexCoords),texture(tex1, fragTexCoords), nestedTexAlphaWeight), texture(tex2, fragTexCoords), outerTexAlphaWeight) * fragColor;\n"
					"}\0";
				break;


			}
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
			if (Program::shaderMap.find(mapKey) != Program::shaderMap.end()) {

				return true;
			}
			return false;
		}

		static Program* generateProgramFromSource(std::string& vShaderSource, std::string& fShaderSource) {

			std::string programKey = Program::generateMapKey(vShaderSource.c_str(), fShaderSource.c_str(), vShaderSource.size(), fShaderSource.size());
			if (Program::programExists(programKey.c_str())) {

				return new Program(programKey.c_str());
			}
			return new Program(Shader(VERTEX_SHADER, vShaderSource.c_str()), Shader(FRAGMENT_SHADER, fShaderSource.c_str()));
		}
	};
}


