#include "RenderShader.h"

#include "LowDX11Storage.h"
#include "ConstantBuffer.h"

namespace Pg::Graphics
{
	using Pg::Data::Enums::eAssetDefine;
	HRESULT hr = NULL;

	//엔진 리소스 로직이 들어가며 통합되어야 한다! (==밑의 생성자를 활용해야 한다!)
	RenderShader::RenderShader(std::wstring CSOFilePath) :
		GraphicsResource(eAssetDefine::_RENDERSHADER, typeid(this).name(), "NotInControl"), _DXStorage(LowDX11Storage::GetInstance())
	{
		hr = LoadShader(CSOFilePath);
	}

	RenderShader::RenderShader(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		
	}
	
	HRESULT RenderShader::LoadShader(std::wstring CSOFilePath)
	{
		return D3DReadFileToBlob(CSOFilePath.c_str(), &(_byteCode));
	}

	void RenderShader::InternalLoad()
	{

	}

	void RenderShader::InternalUnload()
	{

	}

}