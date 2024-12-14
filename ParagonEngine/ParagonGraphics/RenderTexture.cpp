#include "RenderTexture.h"
#include <d3d11.h>
#include <cassert>

namespace Pg::Graphics
{
	RenderTexture::RenderTexture(Pg::Data::Enums::eAssetDefine define, const std::string& typeInfoStr, const std::string& filePath) :
		GraphicsResource(define, typeInfoStr, filePath)
	{
		//
	}

	RenderTexture::~RenderTexture()
	{

	}

	ID3D11ShaderResourceView*& RenderTexture::GetSRV()
	{
		return _textureSRV;
	}

	ID3D11Resource*& RenderTexture::GetResource()
	{
		return _textureBuffer;
	}

	std::wstring RenderTexture::GetFilePathWide()
	{
		std::wstring _wstring;
		_wstring.assign(_filePath.begin(), _filePath.end());

		return _wstring;
	}

	void RenderTexture::InternalLoad()
	{
		assert(false && "���°� ���ǵ��� ���� RenderTexture�� ��������� �ȵ�.");
	}

	void RenderTexture::InternalUnload()
	{

	}


}