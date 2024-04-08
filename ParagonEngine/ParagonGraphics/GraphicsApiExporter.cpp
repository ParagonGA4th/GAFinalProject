#include "GraphicsApiExporter.h"
#include "GraphicsMain.h"

#include "../ParagonUtil/Log.h"

Pg::Graphics::GraphicsApiExporter::GraphicsApiExporter()
{
	
}

Pg::Graphics::GraphicsApiExporter::~GraphicsApiExporter()
{

}

void Pg::Graphics::GraphicsApiExporter::Initialize(GraphicsMain* graphicsMain)
{
	_graphicsMain = graphicsMain;
}

void Pg::Graphics::GraphicsApiExporter::SetLoadCubemapIndex(const std::string& cubemapName, unsigned int index)
{
	PG_WARN("아직 구현되지 않음");
}

void Pg::Graphics::GraphicsApiExporter::SetRenderedCubemapIndex(unsigned int index)
{
	PG_WARN("아직 구현되지 않음");
}


