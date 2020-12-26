#include <pch.h>
#include "GLDebug.h"

void clearGLErrors() {

	while (glGetError());
}
void checkGLError() {

	uint errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) {

		std::string message = "OpenGL reported error code " + std::to_string(errorCode);
		BNDR_EXCEPTION(message.c_str());
	}
}