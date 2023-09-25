#include "RenderShader.h"

#include "LowDX11Storage.h"
#include "ConstantBuffer.h"

namespace Pg::Graphics
{
	HRESULT hr = NULL;

	RenderShader::RenderShader(LowDX11Storage* storage, std::wstring CSOFilePath)
		:_DXStorage(storage)
	{
		hr = CompileShader(CSOFilePath);
	}

	HRESULT RenderShader::CompileShader(std::wstring CSOFilePath)
	{
		return D3DReadFileToBlob(CSOFilePath.c_str(), &(_byteCode));
	}
}