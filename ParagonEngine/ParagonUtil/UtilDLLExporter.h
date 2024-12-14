#pragma once

///ParagonUtil¿« DLL export

#ifdef _EXPORT_UTIL_DLL
#define PARAGON_UTIL_DLL __declspec(dllexport)
#else
#define PARAGON_UTIL_DLL __declspec(dllimport)
#endif //_EXPORT_API