#include "RenderShader.h"

#include "LowDX11Storage.h"
#include "ConstantBuffer.h"

namespace Pg::Graphics
{
	HRESULT hr = NULL;

	RenderShader::RenderShader(std::wstring CSOFilePath)
		:_DXStorage(LowDX11Storage::GetInstance())
	{
		hr = LoadShader(CSOFilePath);
	}

	HRESULT RenderShader::LoadShader(std::wstring CSOFilePath)
	{
		return D3DReadFileToBlob(CSOFilePath.c_str(), &(_byteCode));
	}
}