#include "RenderShader.h"

#include "LowDX11Storage.h"

namespace Pg::Graphics
{
	using Pg::Data::Enums::eAssetDefine;
	HRESULT hr = NULL;

	RenderShader::RenderShader(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		hr = LoadShader(filePath);
	}
	
	HRESULT RenderShader::LoadShader(const std::string& filePath)
	{
		return D3DReadFileToBlob(std::wstring().assign(filePath.begin(), filePath.end()).c_str(), &(_byteCode));
	}

	void RenderShader::InternalLoad()
	{
		
	}

	void RenderShader::InternalUnload()
	{

	}

}