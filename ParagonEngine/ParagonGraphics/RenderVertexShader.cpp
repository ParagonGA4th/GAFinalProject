#include "RenderVertexShader.h"
#include "LowDX11Storage.h"

namespace Pg::Graphics
{
	using Pg::Data::Enums::eAssetDefine;
	
	RenderVertexShader::RenderVertexShader(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}
	
	RenderVertexShader::~RenderVertexShader()
	{
		//
	}

	void RenderVertexShader::InternalLoad()
	{
		
	}

	void RenderVertexShader::InternalUnload()
	{

	}


}