#include "RenderMaterial.h"

namespace Pg::Graphics
{
	using Pg::Core::Resources::GraphicsResource;

	RenderMaterial::RenderMaterial(Pg::Core::Enums::eAssetDefine define, const std::string& filePath) : 
		GraphicsResource(define, typeid(this).name(), filePath)
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