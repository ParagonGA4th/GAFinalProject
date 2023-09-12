#pragma once
#include "../ParagonGraphics/GraphicsDLLExporter.h"
#include "../ParagonCore/IEngine.h"

#include <windows.h>
//#include "DX11Headers.h"


namespace Pg::Graphics
{
	class LowDX11Logic;
	class LowDX11Storage;

	class GraphicsMain : public Pg::Core::IEngine
	{
	public:
		PARAGON_GRAPHICS_DLL GraphicsMain();

	public:
		PARAGON_GRAPHICS_DLL void Initialize();
		PARAGON_GRAPHICS_DLL void Update();
		PARAGON_GRAPHICS_DLL void Finalize();

	private:
		LowDX11Logic* _DXLogic;
		LowDX11Storage* _DXStorage;
	};
}


