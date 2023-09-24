#include "RenderMaterial.h"

namespace Pg::Graphics
{
	using Pg::Core::Resources::GraphicsResource;

	RenderMaterial::RenderMaterial(const std::string& filePath) : GraphicsResource(filePath)
	{
		//
	}

	RenderMaterial::~RenderMaterial()
	{

	}

	void RenderMaterial::InternalLoad()
	{
		//Internal Load Logic, TBA.
	}

	void RenderMaterial::InternalUnload()
	{
		//Internal Unload Logic, TBA.
	}

}