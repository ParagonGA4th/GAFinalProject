#pragma once

///ParagonGameEngine¿« DLL export

#ifdef _EXPORT_ENGINE_DLL
#define PARAGON_ENGINE_DLL __declspec(dllexport)
#else
#define PARAGON_ENGINE_DLL __declspec(dllimport)
#endif //_EXPORT_API