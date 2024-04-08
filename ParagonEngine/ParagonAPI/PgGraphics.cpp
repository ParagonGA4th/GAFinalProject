#include "PgGraphics.h"
#include "../ParagonGraphics/GraphicsApiExporter.h"

#include <singleton-cpp/singleton.h>

namespace Pg::API::Graphics
{
	PgGraphics::PgGraphics()
	{

	}

	PgGraphics::~PgGraphics()
	{

	}

	void PgGraphics::Initialize()
	{
		auto& tSystem = singleton<Pg::Graphics::GraphicsApiExporter>();
		_graphicsApiExporter = &tSystem;
	}
}