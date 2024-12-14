#include "SystemShader.h"
#include "LowDX11Storage.h"

namespace Pg::Graphics
{
	SystemShader::SystemShader(const std::wstring& wFilePath) :
		_DXStorage(LowDX11Storage::GetInstance()), _byteCode(nullptr), _filePath(wFilePath.begin(), wFilePath.end())
	{
		//Blob���� D3D ������ �о�´�. (Load)
		HR(D3DReadFileToBlob(wFilePath.c_str(), &_byteCode));
	}

	SystemShader::~SystemShader()
	{

	}

}