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
	class Shader {

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
		// get the shaderID (read-only)
		inline uint getShaderID() { return shaderID; }
		~Shader();
	};
}


