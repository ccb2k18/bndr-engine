#pragma once
#include <pch.h>

void clearGLErrors();
void checkGLError();

#define GL_DEBUG_FUNC(func) clearGLErrors(); func; checkGLError()

