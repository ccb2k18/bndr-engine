// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
#define DLL_EXPORT __declspec(dllexport)

// add headers that you want to pre-compile here
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <thread>
#include "glew.h"
#include "glfw3.h"
#include "framework.h"

#endif //PCH_H