#pragma once

///ParagonGraphics¿« DLL export

#ifdef _EXPORT_GRAPHICS_DLL
#define PARAGON_GRAPHICS_DLL __declspec(dllexport)
#else
#define PARAGON_GRAPHICS_DLL __declspec(dllimport)
#endif //_EXPORT_API