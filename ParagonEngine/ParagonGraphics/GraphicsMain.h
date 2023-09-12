#pragma once
#include "../ParagonGraphics/GraphicsDLLExporter.h"
#include "../ParagonCore/IGraphics.h"

namespace Pg::Graphics
{
	class GraphicsMain : public Pg::Core::IGraphics
	{
	public:
		PARAGON_GRAPHICS_DLL void Initialize();
		PARAGON_GRAPHICS_DLL void Update();
		PARAGON_GRAPHICS_DLL void Finalize();
	};
}


