#pragma once

///ParagonEditor¿« DLL export

#ifdef _EXPORT_EDITOR_DLL
#define PARAGON_EDITOR_DLL __declspec(dllexport)
#else
#define PARAGON_EDITOR_DLL __declspec(dllimport)
#endif //_EXPORT_API