#pragma once
#include "../ParagonGraphics/GraphicsDLLExporter.h"
#include "../ParagonCore/IGraphics.h"

#include <windows.h>
//#include "DX11Headers.h"

namespace Pg::Graphics
{
	class LowDX11Logic;
	class LowDX11Storage;

	class GraphicsMain : public Pg::Core::IGraphics
	{
	public:
		PARAGON_GRAPHICS_DLL GraphicsMain();

	public:
		PARAGON_GRAPHICS_DLL void Initialize(HWND hWnd);
		PARAGON_GRAPHICS_DLL void Update();
		PARAGON_GRAPHICS_DLL void BeginRender();
		PARAGON_GRAPHICS_DLL void Render();
		PARAGON_GRAPHICS_DLL void EndRender();
		PARAGON_GRAPHICS_DLL void Finalize();

	private:
		LowDX11Logic* _DXLogic;
		LowDX11Storage* _DXStorage;

	private:
		HRESULT hr;
	};
}


