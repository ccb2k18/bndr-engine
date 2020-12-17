#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// define the api macro for exports and imports
#ifdef BNDR_WIN32
#ifdef BNDR_BUILD_DLL
#define BNDR_API __declspec(dllexport)
#else
#define BNDR_API __declspec(dllimport)

#endif // BNDR_BULD_DLL
#else
#error BNDR Engine is only compatible with Windows x86 and x64 systems

#endif // BNDR_WIN32
