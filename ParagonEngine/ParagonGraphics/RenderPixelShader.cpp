#include "RenderPixelShader.h"
#include "LowDX11Storage.h"

namespace Pg::Graphics
{
	using Pg::Data::Enums::eAssetDefine;

	RenderPixelShader::RenderPixelShader(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	RenderPixelShader::~RenderPixelShader()
	{
		//
	}

	void RenderPixelShader::InternalLoad()
	{

	}

	void RenderPixelShader::InternalUnload()
	{

	}
}
