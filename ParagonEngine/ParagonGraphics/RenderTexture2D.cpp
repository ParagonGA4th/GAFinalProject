#include "RenderTexture2D.h"
#include <typeinfo>

namespace Pg::Graphics
{
	RenderTexture2D::RenderTexture2D(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) : 
		GraphicsResource(define, typeid(this).name(), filePath)
	{

	}

	RenderTexture2D::~RenderTexture2D()
	{

	}

	void RenderTexture2D::InternalLoad()
	{

	}

	void RenderTexture2D::InternalUnload()
	{

	}

}