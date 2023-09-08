#pragma once
#include "../ParagonGraphics/GraphicsDLLExporter.h"

namespace Pg::Graphics
{
	class GraphicsMain
	{
	public:
		PARAGON_GRAPHICS_DLL void Initialize();
		PARAGON_GRAPHICS_DLL void Finalize();
	};
}


